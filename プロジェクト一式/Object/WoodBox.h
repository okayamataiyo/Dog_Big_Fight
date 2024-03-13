#pragma once
//�C���N���[�h
#include <string>
#include "../Engine/Direct3D.h"
#include "ObjectBase.h"
#include "../Scene/PlayScene.h"
//WoodBox���Ǘ�����N���X
class WoodBox : public ObjectBase
{
private:
    int hModel_;            //���f��
    GameObject* pParent_;
    PlayScene* pPlayScene_;
    std::vector<int> woodBoxs_;

    //�����C�L���X�g�Ŏg�������o�ϐ�
    float posY_;            //�ؔ���Y���W�ɑ������l
    float posYPrev_;        //1�t���[���O��Y���W
    float posYTemp_;        //1���I�ɍ��W��ۑ�
    bool isJump_;           //���ł��邩
    int  isOnWoodBox_;      //�ؔ��̏�ɂ��邩
    float rayWoodBoxDist_;  //�ؔ��̃f�B�X�g
    float rayStageDist_;    //�X�e�[�W�̃f�B�X�g
public:
    //�R���X�g���N�^
    //����:parent �e�I�u�W�F�N�g(SceneManager)
    WoodBox(GameObject* _pParent);

    //�f�X�g���N�^
    ~WoodBox();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    void Move();

    void RayCast();

    void OnCollision(GameObject* _pTarget);

    int GetModelHandle() { return hModel_; }

    XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }
};