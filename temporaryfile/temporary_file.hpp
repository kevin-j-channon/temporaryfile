#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <memory>
#include <cassert>

enum class path_type
{
	file,
	directory
};


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

template<path_type PATH_TYPE>
class TemporaryPath
{
public:
	explicit TemporaryPath(const std::filesystem::path& rel_path) {
		m_path = std::shared_ptr<std::filesystem::path>(new std::filesystem::path{ std::filesystem::temp_directory_path() / rel_path },
			[this](auto* p) {
				std::error_code _;
				std::filesystem::remove_all(this->m_root_dir, _);

				delete p;
			});

		m_root_dir = _calculate_first_parent_directory(rel_path);

		assert(m_path);
	}

	const auto& path() const {
		assert(m_path);
		return *m_path;
	}

	TemporaryPath& create() {
		assert(m_path);

		if constexpr (PATH_TYPE == path_type::file) {
			if (!std::filesystem::exists(m_path->parent_path())) {
				std::filesystem::create_directories(m_path->parent_path());
			}

			std::ofstream _(m_path->string());
		}
		else if constexpr (PATH_TYPE == path_type::directory) {
			std::filesystem::create_directories(*m_path);
		}
		else {
			static_assert(false, "Invalid path type");
		}

		return *this;
	}

private:

	static std::filesystem::path _calculate_first_parent_directory(std::filesystem::path p) {
		auto first_parent = std::filesystem::path{};
		while (true) {
			p = p.parent_path();
			if (!p.string().empty()) {
				first_parent = p;
			}
			else {
				break;
			}
		}

		return first_parent;
	}

	std::filesystem::path m_root_dir;
	std::shared_ptr<std::filesystem::path> m_path;
};

using TemporaryFile = TemporaryPath<path_type::file>;
using TemporaryDirectory = TemporaryPath<path_type::directory>;