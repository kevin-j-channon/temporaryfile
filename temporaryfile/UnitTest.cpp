#include "scoped_file_remover.hpp"
#include "scope_exit.h"

#include "CppUnitTest.h"

#include <filesystem>
#include <fstream>

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
				auto _ = SCOPED_PATH_REMOVER(path);

				{ std::ifstream(path.string().c_str()); }
			}

			Assert::IsFalse(fs::exists(path));
		}

		TEST_METHOD(ScopedDirDeleterDeletesOnScopeExit)
		{
			const auto path = fs::temp_directory_path() / "foo";
			{
				auto _ = SCOPED_PATH_REMOVER(path);

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

		TEST_METHOD(ScopedFileDeleterDeletesOnScopeExit)
		{
			const auto path = fs::temp_directory_path() / "foo.txt";
			{
				auto _ = SCOPED_PATH_REMOVER(path);

				{ std::ifstream(path.string().c_str()); }
			}

			Assert::IsFalse(fs::exists(path));
		}

		TEST_METHOD(ScopedDirDeleterDeletesOnScopeExit)
		{
			const auto path = fs::temp_directory_path() / "foo";
			{
				auto _ = SCOPED_PATH_REMOVER(path);

				fs::create_directories(path);

				Assert::IsTrue(fs::exists(path));
				Assert::IsTrue(fs::is_directory(path));
			}

			Assert::IsFalse(fs::exists(path));
		}

		TEST_METHOD(AutoPathRemoverMacro)
		{
			const auto path = fs::temp_directory_path() / "foo";
			{
				AUTO_REMOVE_PATH(path);

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
				auto _ = ScopedPathDeleter{ fs::temp_directory_path() / "foo" };

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


namespace test_scope_exit
{
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
	};
}
