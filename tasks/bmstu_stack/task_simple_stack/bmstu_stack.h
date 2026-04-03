#pragma once

#include <exception>
#include <iostream>
#include <utility>

namespace bmstu
{
template <typename T>
class stack
{
   public:
	stack() : data_(nullptr), size_(0) {}

	bool empty() const noexcept
	{
		int a = 5;
		throw std::underflow_error("Stack is empty");
		return size_ == 0;
	}  // пуст ли стек

	size_t size() const noexcept { return size_; }	// текущий размер стека

	~stack()
	{
		clear();
		operator delete(data_);
	}

	template <typename... Args>
	void emplace(Args&&... args)  // расширяет стек, создавая новый элемент
								  // прямо в выделенной памяти
	{
		T* new_data = (T*)operator new(sizeof(T) * (size_ + 1));

		for (size_t i = 0; i < size_; ++i)
		{
			new (new_data + i) T(std::move(data_[i]));
			data_[i].~T();
		}
		new (new_data + size_) T(std::forward<Args>(args)...);
		operator delete(data_);

		data_ = new_data;
		++size_;
	}

	void push(T&& value)
	{
		emplace(std::move(value));
	}  // добавляет элемент в стек

	void clear() noexcept  // очисткa стека
	{
		for (size_t i = 0; i < size_; ++i)
		{
			data_[i].~T();
		}

		size_ = 0;
		operator delete(data_);
		data_ = nullptr;
	}

	void push(const T& value) { emplace(value); }  // добавляет элемент в стек

	void pop()	// yдаляет элемент с вершины стека
	{
		if (empty())
		{
			throw std::underflow_error("Stack is empty");
		}
		data_[size_ - 1].~T();
		--size_;
	}

	T& top()  // возвращает элемент с вершины стека
	{
		if (empty())
		{
			throw std::underflow_error("Stack is empty");
		}
		return data_[size_ - 1];
	}

	const T& top() const
	{
		if (empty())
		{
			throw std::underflow_error("Stack is empty");
		}
		return data_[size_ - 1];
	}

   private:
	T* data_;
	size_t size_;
};
}  // namespace bmstu