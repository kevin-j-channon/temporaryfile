#pragma once

#include <filesystem>
#include <string>

class TemporaryFile
{
public:
	explicit TemporaryFile(const std::string& filename)
		: m_path{std::filesystem::temp_directory_path() / filename}
	{
	}

	const auto& path() const { return m_path; }

private:
	std::filesystem::path m_path;
};