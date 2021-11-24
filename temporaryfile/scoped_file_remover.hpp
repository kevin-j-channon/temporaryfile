#pragma once

#include "scope_exit.h"

#include <filesystem>
#include <functional>

struct ScopedPathRemover
{
	ScopeExitAction<std::function<void()>> m_exit;

	explicit ScopedPathRemover(const std::filesystem::path& path)
		: m_exit{ [path]() { if (std::filesystem::exists(path)) std::filesystem::remove_all(path); }}
	{}
};

#define AUTO_REMOVE_PATH(the_path) \
	const auto __auto_path_remover##__LINE__##__ = ScopedPathRemover(the_path)
