#pragma once

#include <filesystem>
#include <string>
#include <fstream>

class TemporaryFile
{
public:
	explicit TemporaryFile(const std::string& filename)
		: m_path{std::filesystem::temp_directory_path() / filename}
	{
	}

	const auto& path() const { return m_path; }

	TemporaryFile& create()
	{
		std::ofstream _(m_path.string());

		return *this;
	}

private:
	std::filesystem::path m_path;
};