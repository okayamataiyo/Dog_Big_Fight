#pragma once
//インクルード
#include "ObjectBase.h"

enum class OBJECTSTATE
{
    FLOOR = 0
};

class ObjectManager
{
private:
    GameObject* pParent_;
    OBJECTSTATE objectState_;
    ObjectBase* pObjectBase_;
public:
    ObjectManager(GameObject* _parent);
    ~ObjectManager();
    void CreateObject(OBJECTSTATE _objectState,XMFLOAT3 _pos, XMFLOAT3 _rotate, XMFLOAT3 _scale);
    void RotateObject(XMFLOAT3 _rotate);
};

