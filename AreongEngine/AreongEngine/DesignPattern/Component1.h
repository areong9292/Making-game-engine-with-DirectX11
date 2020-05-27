#pragma once

/// 컴포넌트 매크로 추가
/// 만든 목적
/// 해당 매크로를 통해 GameObject에서 가지고 있는 컴포넌트가
/// 어떤 타입인지 바로 체크가능, 에디터에서 처리하기 편함
/// 모든 Component를 상속하는 객체는 헤더파일에 BEGIN_COMPONENT_BASE를 추가하고
/// 소스파일에 BEGIN_COMPONENT_INIT(클래스명)을 추가해야한다

// 부모 클래스를 friend 관계로 하고
// 타입 이름과 타입 이름을 리턴하는 메소드를 지정한다
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

// 컴포넌트 매크로에서 값 셋팅에 필요한 것들
// 클래스 명을 문자열로 하여 타입 변수에 저장한다
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

	// 모든 컴포넌트는 Update를 가지며
	// 매 프레임마다 컴포넌트의 동작을 처리한다
	void Update()
	{
		UpdateComponent();
	}

	virtual const char* getTypeName() { return nullptr; }

private:
	// 모든 컴포넌트 update 처리하는 가상함수
	virtual void UpdateComponent() = 0;

	// 해당 컴포넌트를 가지고 있는 게임 오브젝트 정보
	GameObject* m_pOwner;
};
