#pragma once

#include <utility>
#include <type_traits>

template<typename Fn_T>
struct ScopeExitAction
{
	using Fn_t = Fn_T;

	explicit ScopeExitAction(auto&& fn) : m_fn{ std::forward<Fn_t>(fn) } {}

	~ScopeExitAction() { m_fn(); }

	Fn_T m_fn;
};

template<typename Fn_T>
ScopeExitAction<Fn_T> make_scope_exit_action(Fn_T&& fn) {
	return ScopeExitAction<Fn_T>{std::forward<Fn_T>(fn)};
}
