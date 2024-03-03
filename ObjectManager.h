#pragma once
//インクルード
#include "ObjectBase.h"
#include <vector>
enum class OBJECTSTATE
{
    FLOOR = 0,
    WOODBOX,
};

class ObjectManager
{
private:
    GameObject* pParent_;
    OBJECTSTATE objectState_;
    ObjectBase* pObjectBase_;
    std::vector<int> woodBoxs_;
    std::vector<int> floors_;
    int number_;
public:
    ObjectManager(GameObject* _parent);
    ~ObjectManager();
    void CreateObject(OBJECTSTATE _objectState,XMFLOAT3 _pos, XMFLOAT3 _rotate, XMFLOAT3 _scale);
    void SetRotate(XMFLOAT3 _rotate);
    void SetPosition(XMFLOAT3 _position);
    std::vector<int> GetWoodBoxs() { return woodBoxs_; }
    std::vector<int> GetFloors() { return floors_; }
};

