#pragma once

#include "scope_exit.h"

#include <filesystem>
#include <type_traits>
#include <functional>

struct ScopedPathRemover
{
	ScopeExitAction<std::function<void()>> m_exit;

	explicit ScopedPathRemover(const std::filesystem::path& path)
		: m_exit{ [path]() { if (std::filesystem::exists(path)) std::filesystem::remove_all(path); }}
	{}
};

#define SCOPED_PATH_REMOVER(the_path) \
	make_scope_exit_action(\
		[&the_path](){ \
			if (std::filesystem::exists(the_path))\
				std::filesystem::remove_all(the_path);\
		} \
	)

#define AUTO_REMOVE_PATH(the_path) \
	const auto __auto_path_remover##__LINE__##__ = SCOPED_PATH_REMOVER(the_path)
