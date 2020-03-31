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

			// ���α׷� ���� ���� ȣ�� �ݹ�
			// �޸� ������ �����Ѵ�
			atexit(DestroyInstance);
		}

		return instance;
	}

private:
	static T* instance;

};

template <typename T> T* Singleton<T>::instance = nullptr;
