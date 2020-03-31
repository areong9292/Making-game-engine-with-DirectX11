#pragma once

#include "stdlib.h"
#include <typeinfo>
template <typename T>
class Singleton
{
protected:
	Singleton() {}
	~Singleton() {}
	Singleton(const Singleton& s) {}

private:
	static void DestroyInstance()
	{
		if (instance != nullptr)
			delete instance;

		instance = nullptr;
	}

public:
	static T* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new T;

			// 프로그램 종료 직전 호출 콜백
			// 메모리 해제를 보장한다
			atexit(DestroyInstance);
		}

		return instance;
	}

private:
	static T* instance;

};

template <typename T> T* Singleton<T>::instance = nullptr;
