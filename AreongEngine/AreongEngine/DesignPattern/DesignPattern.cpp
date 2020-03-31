#include "Singleton.h"
#include "Transform.h"
#include "GameObject.h"

void coutTransform(Transform* transform, int index)
{
	for (int i = 0; i < transform->getChildCount(); i++)
	{
		for (int j = 0; j < index; j++)
		{
			cout << '\t';
		}
		auto child = transform->getChild(i);
		cout << child->gameObject->getName() << endl;
		if (child->getChildCount() != 0)
		{
			coutTransform(child, index+1);
		}
	}
}
int main()
{
	GameObject* obj = new GameObject("obj");
	GameObject* obj1 = new GameObject("obj1");
	GameObject* obj2 = new GameObject("obj2");
	GameObject* obj3 = new GameObject("obj3");

	obj->transform->addChild(obj1->transform);
	obj1->transform->addChild(obj2->transform);
	obj2->transform->addChild(obj3->transform);

	cout << Transform::Root->gameObject->getName()<<endl;
	coutTransform(Transform::Root, 1);

	delete obj;
	delete obj1;
	delete obj2;
	delete obj3;
	return 0;
}