#pragma once

#include <filesystem>


std::filesystem::path create_tmp_path(const std::filesystem::path& rel_path)
{
	return std::filesystem::temp_directory_path() / rel_path;
}

class ScopedFileDeleter
{
public :
	explicit ScopedFileDeleter(std::filesystem::path path)
		: m_path{ std::move(path) }
	{}

	ScopedFileDeleter(const ScopedFileDeleter&) = delete;
	ScopedFileDeleter& operator=(const ScopedFileDeleter&) = delete;

	~ScopedFileDeleter()
	{
		if (std::filesystem::exists(m_path))
		{
			std::filesystem::remove_all(m_path);
		}
	}

private:
	std::filesystem::path m_path;
};

class ScopedDirectoryDeleter
{
public:
	explicit ScopedDirectoryDeleter(std::filesystem::path path)
		: m_path{ std::move(path) }
	{}

	ScopedDirectoryDeleter(const ScopedDirectoryDeleter&) = delete;
	ScopedDirectoryDeleter& operator=(const ScopedDirectoryDeleter&) = delete;

	~ScopedDirectoryDeleter()
	{
		if (std::filesystem::exists(m_path))
		{
			std::filesystem::remove_all(m_path);
		}
	}

private:
	std::filesystem::path m_path;
};
