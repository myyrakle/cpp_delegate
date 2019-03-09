#pragma once

//C#�� �븮��(delegate)�� ����� Ŭ���� ���ø��Դϴ�.
//�䱸����: C++11
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
public: //������ �� �������Դϴ�.
	template <class F>
	delegate(F&&);
	template <class F>
	delegate& operator=(F&&);
	template <class F>
	delegate& operator+=(F&&);
public: //������ �� �������Դϴ�.
	delegate(const func_type&);
	delegate(func_type&&);
	delegate& operator=(const func_type&);
	delegate& operator=(func_type&&);
public: //�Լ��� �߰��� �̾���Դϴ�.
	delegate& operator+=(const func_type&);
	delegate& operator+=(func_type&&);
public: //�⺻ ��� ����Ʈ�Դϴ�.
	delegate() = default;
	virtual ~delegate() = default;
	delegate(const delegate&) = default;
	delegate(delegate&&) = default;
	delegate& operator=(const delegate&) = default;
	delegate& operator=(delegate&&) = default;
public:
	R operator()(Args...) const; //����� �Լ��� ���� ȣ���մϴ�.
	R invoke(Args...) const; //���� �����մϴ�.
public:
	bool empty() const; //����ִ��� ���θ� Ȯ���մϴ�.
	bool operator!() const; //empty�� �����մϴ�.
	operator bool() const; //������� ���� ��쿡 true�� ��ȯ�մϴ�.
	size_t size() const; //�Լ��� ������ ��ȯ�մϴ�.
	void clear(); //����� �Լ��� ���� ���������ϴ�.
public:
};

template <class... Args>
using Action = delegate<void(Args...)>;

//����Ÿ�� ��ġ�� ������
template <class Result, class... Args>
using Func = delegate<Result(Args...)>;


//�޼��� ���ø�
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
	//����
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

