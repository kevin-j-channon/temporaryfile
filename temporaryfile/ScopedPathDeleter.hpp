#pragma once

#include <filesystem>

class ScopedPathDeleter
{
public :
	explicit ScopedPathDeleter(std::filesystem::path path)
		: m_path{ std::move(path) }
	{}

	ScopedPathDeleter(const ScopedPathDeleter&) = delete;
	ScopedPathDeleter& operator=(const ScopedPathDeleter&) = delete;

	~ScopedPathDeleter()
	{
		if (std::filesystem::exists(m_path))
		{
			std::filesystem::remove_all(m_path);
		}
	}

private:
	std::filesystem::path m_path;
};
