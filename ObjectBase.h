#pragma once
//�C���N���[�h
#include "Engine/GameObject.h"

class ObjectBase : public GameObject
{
public:
    ObjectBase(GameObject* _parent,std::string _name)
        :GameObject(_parent,_name){}
    //������
    void Initialize()   override {};

    //�X�V
    void Update()       override {};

    //�`��
    void Draw()         override {};

    //�J��
    void Release()      override {};


};