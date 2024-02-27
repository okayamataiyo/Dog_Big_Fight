#include "ObjectManager.h"
#include "Floor.h"
void ObjectManager::CreateObject(OBJECTSTATE _objectState,XMFLOAT3 _pos, XMFLOAT3 _rotate, XMFLOAT3 _scale)
{
	pObjectBase_ = nullptr;
	switch (_objectState)
	{
	case OBJECTSTATE::FLOOR:
		pObjectBase_ = Instantiate<Floor>(pParent_);
		break;
	}
	pObjectBase_->SetPosition(_pos);
	pObjectBase_->SetRotate(_rotate);
	pObjectBase_->SetScale(_scale);
}

void ObjectManager::RotateObject(XMFLOAT3 _rotate)
{
	pObjectBase_->SetRotate(_rotate);
}

ObjectManager::ObjectManager(GameObject* _parent)
	:pParent_(_parent)
{
}

ObjectManager::~ObjectManager()
{
}
