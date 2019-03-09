#pragma once

//C#의 대리자(delegate)를 모방한 클래스 템플릿입니다.
//요구사항: C++11
#include <vector>
#include <functional>

template<class T>
class delegate;

template<class R, class... Args>
class delegate<R(Args...)>
{
public:
	using func_type = std::function<R(Args...)>;
private:
	std::vector<func_type> functions;
public: //생성자 및 대입자입니다.
	template <class F>
	delegate(F&&);
	template <class F>
	delegate& operator=(F&&);
	template <class F>
	delegate& operator+=(F&&);
public: //생성자 및 대입자입니다.
	delegate(const func_type&);
	delegate(func_type&&);
	delegate& operator=(const func_type&);
	delegate& operator=(func_type&&);
public: //함수를 추가로 이어붙입니다.
	delegate& operator+=(const func_type&);
	delegate& operator+=(func_type&&);
public: //기본 멤버 리스트입니다.
	delegate() = default;
	virtual ~delegate() = default;
	delegate(const delegate&) = default;
	delegate(delegate&&) = default;
	delegate& operator=(const delegate&) = default;
	delegate& operator=(delegate&&) = default;
public:
	R operator()(Args...) const; //저장된 함수를 전부 호출합니다.
	R invoke(Args...) const; //위와 동일합니다.
public:
	bool empty() const; //비어있는지 여부를 확인합니다.
	bool operator!() const; //empty와 동일합니다.
	operator bool() const; //비어있지 않을 경우에 true를 반환합니다.
	size_t size() const; //함수의 개수를 반환합니다.
	void clear(); //저장된 함수를 전부 날려버립니다.
public:
};

template <class... Args>
using Action = delegate<void(Args...)>;

//리턴타입 위치가 뒤집힘
template <class Result, class... Args>
using Func = delegate<Result(Args...)>;


//메서드 템플릿
template<class R, class... Args>
template<class F>
delegate<R(Args...)>::delegate(F&& f)
	: functions(1, std::function<R(Args...)>(std::move(f)))
{}
template<class R, class... Args>
template<class F>
delegate<R(Args...)>& delegate<R(Args...)>::operator=(F&& f)
{
	functions.clear();
	functions.emplace_back(std::move(f));
	return *this;
}
template<class R, class... Args>
template<class F>
delegate<R(Args...)>& delegate<R(Args...)>::operator+=(F&& f)
{
	functions.emplace_back(std::move(f));
	return *this;
}


template<class R, class... Args>
delegate<R(Args...)>::delegate(const func_type& f)
	: functions(1, f)
{}
template<class R, class... Args>
delegate<R(Args...)>::delegate(func_type&& f)
	: functions(1, std::move(f))
{}

template<class R, class... Args>
delegate<R(Args...)>& delegate<R(Args...)>::operator=(const func_type& f)
{
	functions.clear();
	functions.push_back(f);
	return *this;
}
template<class R, class... Args>
delegate<R(Args...)>& delegate<R(Args...)>::operator=(func_type&& f)
{
	functions.clear();
	functions.push_back(std::move(f));
	return *this;
}

template<class R, class... Args>
delegate<R(Args...)>& delegate<R(Args...)>::operator+=(const func_type& f)
{
	functions.push_back(f);
	return *this;
}
template<class R, class... Args>
delegate<R(Args...)>& delegate<R(Args...)>::operator+=(func_type&& f)
{
	functions.push_back(std::move(f));
	return *this;
}


template<class R, class... Args>
R delegate<R(Args...)>::operator()(Args... args) const
{
	if (functions.empty());

	else
		for (int i = 0; true; ++i)
			if ((i + 1) == functions.size())
				return functions[i](args...);
			else
				functions[i](args...);
}
template<class R, class... Args>
R delegate<R(Args...)>::invoke(Args... args) const
{
	//위임
	return this->operator()(args...);
}
template<class R, class ...Args>
delegate<R(Args...)>::operator bool() const
{
	return !functions.empty();
}

template<class R, class ...Args>
bool delegate<R(Args...)>::empty() const
{
	return functions.empty();
}



template<class R, class ...Args>
bool delegate<R(Args...)>::operator!() const
{
	return this->empty();
}

template<class R, class ...Args>
size_t delegate<R(Args...)>::size() const
{
	return functions.size();
}

template<class R, class ...Args>
void delegate<R(Args...)>::clear()
{
	functions.clear();
}


//template <class T, class Result>
//using Func = delegate<Result(T)>;
//template <class T, class T2, class Result>
//using Func = delegate<Result(T, T2)>;
//template <class T, class T2, class T3, class Result>
//using Func = delegate<Result(T, T2, T3)>;
//template <class T, class T2, class T3, class T4, class Result>
//using Func = delegate<Result(T, T2, T3,T4)>;

