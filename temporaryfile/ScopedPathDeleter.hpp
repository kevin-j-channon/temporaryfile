#pragma once

#include <filesystem>
#include <type_traits>
#include <functional>

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

template<typename Fn_T>
struct ScopeExitRunner
{
	explicit ScopeExitRunner(Fn_T&& fn) : m_fn{ std::move(fn) } {}
	
	ScopeExitRunner(const ScopeExitRunner&) = delete;
	ScopeExitRunner& operator=(const ScopeExitRunner&) = delete;

	~ScopeExitRunner() { m_fn(); }
	
	Fn_T m_fn;
};

template<typename T>
ScopeExitRunner<T> make_scope_exit_runner(T&& fn) {
	return ScopeExitRunner<T>{std::forward<T>(fn)};
}

#define SCOPED_PATH_REMOVER(the_path) \
	make_scope_exit_runner(\
		[&the_path](){ \
			if (std::filesystem::exists(the_path))\
				std::filesystem::remove_all(the_path);\
		} \
	)

#define AUTO_REMOVE_PATH(the_path) \
	const auto __auto_path_remover##__LINE__##__ = make_scope_exit_runner(\
		[&the_path](){ \
			if (std::filesystem::exists(the_path))\
				std::filesystem::remove_all(the_path);\
		} \
	)
