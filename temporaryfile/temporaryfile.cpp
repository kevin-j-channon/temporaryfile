#include "temporary_file.hpp"

#include "CppUnitTest.h"

#include <filesystem>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace fs = std::filesystem;

namespace temporaryfile
{
	TEST_CLASS(TempPathRemoverTests)
	{
	public:

		TEST_METHOD(ScopedFileDeleterDeletesOnScopeExit)
		{
			const auto path = fs::temp_directory_path() / "foo.txt";
			{
				auto _ = ScopedFileDeleter(path);

				{ std::ifstream(path.string().c_str()); }
			}

			Assert::IsFalse(fs::exists(path));
		}

		TEST_METHOD(ScopedDirDeleterDeletesOnScopeExit)
		{
			const auto path = fs::temp_directory_path() / "foo";
			{
				auto _ = ScopedDirectoryDeleter(path);

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
				auto _ = ScopedFileDeleter{ fs::temp_directory_path() / "foo" };

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
