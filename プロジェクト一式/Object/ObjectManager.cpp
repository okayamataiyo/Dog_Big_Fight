//インクルード
#include <random>
#include "ObjectManager.h"
#include "Floor.h"
#include "WoodBox.h"
#include "FrameBox.h"
#include "Bone.h"


void ObjectManager::CreateObject(OBJECTSTATE _objectState,XMFLOAT3 _pos, XMFLOAT3 _rotate, XMFLOAT3 _scale)
{
	std::string woodBoxName = "WoodBox";
	std::string woodBoxNumber = woodBoxName + std::to_string(number_);
	switch (_objectState)
	{
	case OBJECTSTATE::FLOOR:
		pObjectBase_ = Instantiate<Floor>(pParent_);
		floors_.push_back(pObjectBase_->GetModelHandle());
		break;
	case OBJECTSTATE::WOODBOX:
		pObjectBase_ = Instantiate<WoodBox>(pParent_);
		woodBoxs_.push_back(pObjectBase_->GetModelHandle());
		pObjectBase_->SetObjectName(woodBoxNumber);
		number_++;
		break;
	case OBJECTSTATE::FRAMEBOX:
		pObjectBase_ = Instantiate<FrameBox>(pParent_);
		break;
	case OBJECTSTATE::BONE:
		pObjectBase_ = Instantiate<Bone>(pParent_);
		break;
	}
	pObjectBase_->SetPosition(_pos);
	pObjectBase_->SetRotate(_rotate);
	pObjectBase_->SetScale(_scale);
}

void ObjectManager::CreateObject(OBJECTSTATE _objectState, float _minX, float _maxX, float _minZ, float _maxZ)
{
	switch (_objectState)
	{
	case OBJECTSTATE::BONE:
		float minX = _minX;
		float maxX = _maxX;
		float minY = 0;
		float maxY = 0;
		float minZ = _minZ;
		float maxZ = _maxZ;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> distX(minX, maxX);
		std::uniform_real_distribution<float> distY(minY, maxY);
		std::uniform_real_distribution<float> distZ(minZ, maxZ);
		XMFLOAT3 randomPos = { distX(gen), distY(gen), distZ(gen) };
		pObjectBase_ = Instantiate<Bone>(pParent_);
		pObjectBase_->SetPosition(randomPos);
		break;
	}
}

void ObjectManager::SetRotate(XMFLOAT3 _rotate)
{
	pObjectBase_->SetRotate(_rotate);
}

void ObjectManager::SetPosition(XMFLOAT3 _position)
{
	pObjectBase_->SetPosition(_position);
}

ObjectManager::ObjectManager(GameObject* _parent)
	:pParent_(_parent), pObjectBase_(nullptr),objectState_(OBJECTSTATE::FLOOR),number_(0)
{
}

ObjectManager::~ObjectManager()
{
}
