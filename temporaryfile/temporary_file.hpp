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

template<path_type PATH_TYPE>
class TemporaryPath
{
public:
	explicit TemporaryPath(const std::filesystem::path& rel_path) {
		m_path = std::shared_ptr<std::filesystem::path>(new std::filesystem::path{ std::filesystem::temp_directory_path() / rel_path },
			[](auto* p) {
				std::error_code _;
				std::filesystem::remove_all(*p, _);

				delete p;
			});

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

	std::shared_ptr<std::filesystem::path> m_path;
};

using TemporaryFile = TemporaryPath<path_type::file>;
using TemporaryDirectory = TemporaryPath<path_type::directory>;