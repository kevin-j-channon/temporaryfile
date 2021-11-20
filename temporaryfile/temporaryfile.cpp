#include "temporary_file.hpp"

#include "CppUnitTest.h"

#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace temporaryfile
{
	TEST_CLASS(TestTemporaryFile)
	{
	public:

		TEST_METHOD(CreateTempFilePath)
		{
			const auto temp_file = TemporaryFile("foo.txt");

			Assert::AreEqual(std::string("foo.txt"), temp_file.path().filename().string());
		}

		TEST_METHOD(CreateTempFile)
		{
			const auto temp_file = TemporaryFile("foo.txt").create();
			Assert::IsTrue(std::filesystem::exists(temp_file.path()));
		}

		TEST_METHOD(TempFileIsDeletedWhenScopeEnds)
		{
			auto path = std::filesystem::path{};

			{
				const auto temp_file = TemporaryFile("foo.txt").create();
				path = temp_file.path();
			}

			Assert::IsFalse(std::filesystem::exists(path));
		}

		TEST_METHOD(CopyingTempFilesDoesntDeleteTheFilePrematurely)
		{
			const auto temp_1 = TemporaryFile("foo.txt").create();

			{
				const auto temp_2 = temp_1;
			}

			Assert::IsTrue(std::filesystem::exists(temp_1.path()));
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
			Assert::IsTrue(std::filesystem::exists(temp_dir.path()));
			Assert::IsTrue(std::filesystem::is_directory(temp_dir.path()));
			Assert::IsFalse(std::filesystem::is_regular_file(temp_dir.path()));
		}
	};
}
