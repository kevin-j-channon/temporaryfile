#include "temporary_file.hpp"

#include "CppUnitTest.h"

#include <filesystem>
#include <format>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace fs = std::filesystem;

namespace temporaryfile
{
	TEST_CLASS(TestTemporaryFile)
	{
	public:

		TEST_METHOD(CreateTempFilePath1)
		{
			const auto path = create_tmp_path("foo.txt");
			Assert::IsTrue(path.string().ends_with("foo.txt"));
		}

		TEST_METHOD(CreateScopedFileDeleter)
		{
			const auto path = create_tmp_path("foo.txt");
			{
				auto _ = ScopedFileDeleter(path);

				{ std::ifstream(path.string().c_str()); }
			}

			Assert::IsFalse(fs::exists(path));
		}

		TEST_METHOD(CreateTempFilePath)
		{
			const auto temp_file = TemporaryFile("foo.txt");

			Assert::AreEqual(std::string("foo.txt"), temp_file.path().filename().string());
		}

		TEST_METHOD(CreateTempFile)
		{
			const auto temp_file = TemporaryFile("foo.txt").create();
			Assert::IsTrue(fs::exists(temp_file.path()));
		}

		TEST_METHOD(CreateTempFileInSubdirectory)
		{
			const auto temp_file = TemporaryFile(fs::path{ "foo" } / "bar" / "baz.txt").create();
			Assert::IsTrue(fs::exists(temp_file.path()));
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(TempFileIsDeletedWhenScopeEnds)
			TEST_IGNORE()
			END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(TempFileIsDeletedWhenScopeEnds)
		{
			auto path = fs::path{};

			{
				const auto temp_file = TemporaryFile("foo.txt").create();
				path = temp_file.path();
			}

			Assert::IsFalse(fs::exists(path));
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(AllCreatedSubdirectoriesAreDeleteWhenScopeEnds)
			TEST_IGNORE()
		END_TEST_METHOD_ATTRIBUTE()
		TEST_METHOD(AllCreatedSubdirectoriesAreDeleteWhenScopeEnds)
		{
			auto path = fs::path{};
			{
				const auto temp_file = TemporaryFile(fs::path{ "foo" } / "bar" / "baz.txt").create();
				path = temp_file.path();
			}

			Assert::IsFalse(fs::exists(path));
			Assert::IsFalse(fs::exists(path.parent_path()));
			Assert::IsFalse(fs::exists(path.parent_path().parent_path()));
			Assert::IsTrue(fs::exists(path.parent_path().parent_path().parent_path()));
		}

		TEST_METHOD(CopyingTempFilesDoesntDeleteTheFilePrematurely)
		{
			const auto temp_1 = TemporaryFile("foo.txt").create();

			{
				const auto temp_2 = temp_1;
			}

			Assert::IsTrue(fs::exists(temp_1.path()));
		}
	};

	TEST_CLASS(TestTemporaryDirectory)
	{
		TEST_METHOD(CreateTempDirPath)
		{
			const auto temp_dir = TemporaryDirectory("foo");
			Assert::AreEqual(std::string("foo"), temp_dir.path().filename().string());
		}

		TEST_METHOD(CreateTempDir)
		{
			const auto temp_dir = TemporaryDirectory("foo").create();
			Assert::IsTrue(fs::exists(temp_dir.path()));
			Assert::IsTrue(fs::is_directory(temp_dir.path()));
			Assert::IsFalse(fs::is_regular_file(temp_dir.path()));
		}
	};
}
