#pragma once

#include <utility>

template<typename Fn_T>
struct ScopeExitAction
{
	explicit ScopeExitAction(auto&& fn) : m_fn{ std::move(fn) } {}

	ScopeExitAction(const ScopeExitAction&) = delete;
	ScopeExitAction& operator=(const ScopeExitAction&) = delete;

	~ScopeExitAction() { m_fn(); }

	Fn_T m_fn;
};

template<typename Fn_T>
ScopeExitAction<Fn_T> make_scope_exit_action(Fn_T&& fn) {
	return ScopeExitAction<Fn_T>{std::forward<Fn_T>(fn)};
}


ScopeExitAction<void(*)(void)> make_scope_exit_action(void(*fn)(void)) {
	return ScopeExitAction<void(*)(void)>{fn};
}
