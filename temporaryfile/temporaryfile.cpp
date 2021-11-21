#include "temporary_file.hpp"

#include "CppUnitTest.h"

#include <filesystem>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace fs = std::filesystem;

namespace temporaryfile
{
	TEST_CLASS(FreeFunctions)
	{
	public:

		TEST_METHOD(CreateTempFilePath)
		{
			const auto path = create_tmp_path("foo.txt");
			Assert::IsTrue(path.string().ends_with("foo.txt"));
		}

		TEST_METHOD(ScopedFileDeleterDeletesOnScopeExit)
		{
			const auto path = create_tmp_path("foo.txt");
			{
				auto _ = ScopedFileDeleter(path);

				{ std::ifstream(path.string().c_str()); }
			}

			Assert::IsFalse(fs::exists(path));
		}

		TEST_METHOD(ScopedDirDeleterDeletesOnScopeExit)
		{
			const auto path = create_tmp_path("foo");
			{
				auto _ = ScopedDirectoryDeleter(path);

				fs::create_directories(path);

				Assert::IsTrue(fs::exists(path));
				Assert::IsTrue(fs::is_directory(path));
			}

			Assert::IsFalse(fs::exists(path));
		}
	};
}
