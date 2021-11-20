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
	};
}