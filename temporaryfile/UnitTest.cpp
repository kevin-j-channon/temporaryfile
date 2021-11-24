#include "scoped_file_remover.hpp"
#include "scope_exit.h"

#include "CppUnitTest.h"

#include <filesystem>
#include <fstream>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace fs = std::filesystem;

namespace test_path_deleter
{
	TEST_CLASS(GeneralTests)
	{
	public:
		TEST_METHOD(ScopedFileDeleterDeletesOnScopeExit)
		{
			const auto path = fs::temp_directory_path() / "foo.txt";
			{
				auto _ = ScopedPathRemover(path);

				{ std::ifstream(path.string().c_str()); }
			}

			Assert::IsFalse(fs::exists(path));
		}

		TEST_METHOD(ScopedDirDeleterDeletesOnScopeExit)
		{
			const auto path = fs::temp_directory_path() / "foo";
			{
				auto _ = ScopedPathRemover(path);

				fs::create_directories(path);

				Assert::IsTrue(fs::exists(path));
				Assert::IsTrue(fs::is_directory(path));
			}

			Assert::IsFalse(fs::exists(path));
		}

		TEST_METHOD(NestedDirectoriesAreRemovedOnScopeExit)
		{
			const auto path = fs::temp_directory_path() / "foo" / "bar" / "baz1.txt";
			{
				auto _ = ScopedPathRemover{ fs::temp_directory_path() / "foo" };

				fs::create_directories(path.parent_path());
				std::ofstream f(path.string().c_str());

				Assert::IsTrue(fs::exists(path));
			}

			Assert::IsFalse(fs::exists(path));
			Assert::IsFalse(fs::exists(path.parent_path()));
			Assert::IsFalse(fs::exists(path.parent_path().parent_path()));
			Assert::IsTrue(fs::exists(path.parent_path().parent_path().parent_path()));
		}
	};

	TEST_CLASS(MacroTests)
	{
	public:

		TEST_METHOD(NestedDirectoriesAreRemovedOnScopeExit)
		{
			const auto path = fs::temp_directory_path() / "foo" / "bar" / "baz1.txt";
			{
				auto _ = ScopedPathRemover{ fs::temp_directory_path() / "foo" };

				fs::create_directories(path.parent_path());
				std::ofstream f(path.string().c_str());

				Assert::IsTrue(fs::exists(path));
			}

			Assert::IsFalse(fs::exists(path));
			Assert::IsFalse(fs::exists(path.parent_path()));
			Assert::IsFalse(fs::exists(path.parent_path().parent_path()));
			Assert::IsTrue(fs::exists(path.parent_path().parent_path().parent_path()));
		}
	};
}

void fn()
{
	Logger::WriteMessage("Stateless");
}

namespace test_scope_exit
{
	auto global_fn_was_run = false;

	TEST_CLASS(ScopeExitActionTests)
	{
	public:
		TEST_METHOD(ActionIsExecutedOnScopeExit)
		{
			auto fn_was_executed = false;

			{
				const auto _ = make_scope_exit_action([&fn_was_executed]() { fn_was_executed = true; });
			}

			Assert::IsTrue(fn_was_executed);
		}

		TEST_METHOD(UseStatelessOverload)
		{
			{
				const ScopeExitAction<void(*)(void)> exit([]() {Logger::WriteMessage("Stateless?"); });
				Assert::IsTrue(std::is_convertible_v<void(*)(void), decltype(exit)::Fn_t>);
			}

			// This fails...
			//{
			//	const auto exit = make_scope_exit_action([]() {Logger::WriteMessage("Stateless?"); });
			//	Assert::IsTrue(std::is_convertible_v<void(*)(void), decltype(exit)::Fn_t>);
			//}
		}
	};
}
