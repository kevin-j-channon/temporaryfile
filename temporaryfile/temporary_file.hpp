#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <memory>
#include <cassert>

class TemporaryPath
{
public:
	explicit TemporaryPath(const std::string& filename) {
		m_path = std::shared_ptr<std::filesystem::path>(new std::filesystem::path{ std::filesystem::temp_directory_path() / filename },
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
		std::ofstream _(m_path->string());
		return *this;
	}

private:
	std::shared_ptr<std::filesystem::path> m_path;
};

using TemporaryFile = TemporaryPath;