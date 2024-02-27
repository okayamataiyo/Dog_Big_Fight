#pragma once
//インクルード
#include "Engine/GameObject.h"

class ObjectBase : public GameObject
{
public:
    ObjectBase(GameObject* _parent,std::string _name)
        :GameObject(_parent,_name){}
    //初期化
    void Initialize()   override {};

    //更新
    void Update()       override {};

    //描画
    void Draw()         override {};

    //開放
    void Release()      override {};


};