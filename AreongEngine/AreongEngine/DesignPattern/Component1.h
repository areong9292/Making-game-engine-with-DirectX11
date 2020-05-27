#pragma once

/// ������Ʈ ��ũ�� �߰�
/// ���� ����
/// �ش� ��ũ�θ� ���� GameObject���� ������ �ִ� ������Ʈ��
/// � Ÿ������ �ٷ� üũ����, �����Ϳ��� ó���ϱ� ����
/// ��� Component�� ����ϴ� ��ü�� ������Ͽ� BEGIN_COMPONENT_BASE�� �߰��ϰ�
/// �ҽ����Ͽ� BEGIN_COMPONENT_INIT(Ŭ������)�� �߰��ؾ��Ѵ�

// �θ� Ŭ������ friend ����� �ϰ�
// Ÿ�� �̸��� Ÿ�� �̸��� �����ϴ� �޼ҵ带 �����Ѵ�
#define BEGIN_COMPONENT_BASE							\
	friend class Component;								\
	private:											\
		static const char* _typeName;					\
														\
	public:												\
		virtual const char* getTypeName() override		\
		{												\
			return _typeName;							\
		}

// ������Ʈ ��ũ�ο��� �� ���ÿ� �ʿ��� �͵�
// Ŭ���� ���� ���ڿ��� �Ͽ� Ÿ�� ������ �����Ѵ�
#define BEGIN_COMPONENT_INIT(classType)					\
	const char* classType::_typeName = #classType

#include <iostream>
using namespace std;

class GameObject;
class Transform;
class Component
{
public:
	Component() = delete;
	Component(GameObject* gameObject);
	virtual ~Component();

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	void setOwner(GameObject* pOwner);
	GameObject* GetOwner() const;

	GameObject* const gameObject;
	Transform* const transform;

	// ��� ������Ʈ�� Update�� ������
	// �� �����Ӹ��� ������Ʈ�� ������ ó���Ѵ�
	void Update()
	{
		UpdateComponent();
	}

	virtual const char* getTypeName() { return nullptr; }

private:
	// ��� ������Ʈ update ó���ϴ� �����Լ�
	virtual void UpdateComponent() = 0;

	// �ش� ������Ʈ�� ������ �ִ� ���� ������Ʈ ����
	GameObject* m_pOwner;
};
