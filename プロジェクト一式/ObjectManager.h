#pragma once
//インクルード
#include "ObjectBase.h"
#include <vector>
enum class OBJECTSTATE
{
    FLOOR = 0,
    WOODBOX,
    BONE
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
    void CreateObject(OBJECTSTATE _objectState,XMFLOAT3 _pos = XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3 _rotate = XMFLOAT3(0.0f,0.0f,0.0f), XMFLOAT3 _scale = XMFLOAT3(1.0f,1.0f,1.0f));
    void CreateObject(OBJECTSTATE _objectState, float _minX, float _maxX, float _minZ, float _maxZ);
    void SetRotate(XMFLOAT3 _rotate);
    void SetPosition(XMFLOAT3 _position);
    std::vector<int> GetWoodBoxs() { return woodBoxs_; }
    std::vector<int> GetFloors() { return floors_; }
};

