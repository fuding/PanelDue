/*
 * Vector.hpp
 *
 * Created: 09/11/2014 09:50:41
 *  Author: David
 */ 


#ifndef VECTOR_H_
#define VECTOR_H_

#include "ecv.h"
#include <cstddef>		// for size_t
#include <cstdarg>
#include <cstring>
#undef printf
#undef scanf
void printf();			// to keep gcc happy when we include cstdio
void scanf();			// to keep gcc happy when we include cstdio
#include <cstdio>

// Bounded vector class
template<class T, size_t N> class Vector
{
public:
	Vector() : filled(0) { }
		
	bool full() const { return filled == N; }

	size_t capacity() const { return N; }

	size_t size() const { return filled; }

	bool isEmpty() const { return filled == 0; }

	const T& operator[](size_t index) const pre(index < N) { return storage[index]; }

	T& operator[](size_t index) pre(index < N) { return storage[index]; }

	void add(const T& x) pre(filled < N) { storage[filled++] = x; }

	void add(const T* array p, size_t n) pre(filled + n <= N);

	void clear() { filled = 0; }

	const T* array c_ptr() { return storage; }

	void sort(bool (*sortfunc)(T, T));

protected:
	T storage[N];
	size_t filled;	
};

template<class T, size_t N> void Vector<T, N>::add(const T* array p, size_t n)
{
	while (n != 0 && filled != N)
	{
		storage[filled++] = *p++;
		--n;
	}
}

template<class T, size_t N> void Vector<T, N>::sort(bool (*sortfunc)(T, T))
{
	for (size_t i = 1; i < filled; ++i)
	{
		for (size_t j = 0; j < i; ++j)
		{
			if ((*sortfunc)(storage[j], storage[i]))
			{
				T temp = storage[i];
				// Insert element i just before element j
				for (size_t k = i; k > j; --k)
				{
					storage[k] = storage[k - 1];
				}
				storage[j] = temp;
			}
		}
	}
}

template<size_t N> class String : public Vector<char, N + 1>
{
public:
	String() : Vector<char, N + 1>()
	{
		this->clear();
	}

	// Redefine 'full' so as to make room for a null terminator
	bool full() const { return this->filled == N; }
		
	// Redefine 'add' to add a null terminator
	void add(char x) pre(this->filled < N)
	{
		this->storage[this->filled++] = x; 
		this->storage[this->filled] = '\0';
	}
		
	const char* array c_str() const { return this->storage; }
		
	void clear()
	{
		this->filled = 0;
		this->storage[0] = '\0'; 
	}
	
	void catFrom(const char* s)
	{
		while (*s != '\0' && this->filled < N)
		{
			this->storage[this->filled++] = *s++;
		}
		this->storage[this->filled] = '\0';
	}
	
	void copyFrom(const char* s)
	{
		this->clear();
		this->catFrom(s);
	}
	
	int sprintf(const char *fmt, ...);
	
	int scatf(const char *fmt, ...);
	
	bool equals(const char* s) const
	{
		return strcmp(s, this->storage) == 0;
	}
	
	bool equalsIgnoreCase(const char* s) const
	{
		return stricmp(s, this->storage) == 0;
	}
};

template<size_t N> int String<N>::sprintf(const char *fmt, ...)
{
	va_list vargs;
	va_start(vargs, fmt);
	int ret = vsnprintf(this->storage, N + 1, fmt, vargs);
	va_end(vargs);

	if (ret < 0)
	{
		this->filled = 0;
		this->storage[0] = 0;
	}
	else if (ret < N)
	{
		this->filled = ret;
	}
	else
	{
		this->filled = N;
	}
	return ret;
}

template<size_t N> int String<N>::scatf(const char *fmt, ...)
{
	va_list vargs;
	va_start(vargs, fmt);
	int ret = vsnprintf(this->storage + this->filled, N + 1 - this->filled, fmt, vargs);
	va_end(vargs);
	
	if (ret < 0)
	{
		this->storage[this->filled] = 0;		// in case snprintf messed it up
	}
	else if (this->filled + ret < N)
	{
		this->filled += ret;
	}
	else
	{
		this->filled = N;
	}
	return ret;
}

#endif /* VECTOR_H_ */