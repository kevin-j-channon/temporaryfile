#pragma once

#include <utility>
#include <type_traits>

template<typename Fn_T>
struct ScopeExitAction
{
	using Fn_t = Fn_T;

	explicit ScopeExitAction(auto&& fn) : m_fn{ std::forward<Fn_t>(fn) } {}

	//explicit ScopeExitAction(ScopeExitAction&& other) noexcept
	//	: m_fn{std::move(other.m_fn)}
	//{}

	ScopeExitAction& operator=(ScopeExitAction&& other) noexcept
	{
		if constexpr (std::is_constructible_v<void(*)(void), Fn_t>)
		{
			m_fn = other.m_fn;
			other.m_fn = nullptr;
		}
		else {
			m_fn = std::move(other.m_fn);
		}

		return *this;
	}

	ScopeExitAction(const ScopeExitAction&) = delete;
	ScopeExitAction& operator=(const ScopeExitAction&) = delete;

	~ScopeExitAction() { m_fn(); }

	Fn_T m_fn;
};

template<typename Fn_T>
ScopeExitAction<Fn_T> make_scope_exit_action(Fn_T&& fn) {
	return ScopeExitAction<Fn_T>{std::forward<Fn_T>(fn)};
}
