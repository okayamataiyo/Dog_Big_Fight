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
    int hSound_;            //�T�E���h�f�[�^
    bool isBreak_;            //�ؔ����󂳂ꂽ��
    GameObject* pParent_;
    PlayScene* pPlayScene_;
    std::vector<int> woodBoxs_;

    //�����C�L���X�g�Ŏg�������o�ϐ�
    float positionY_;            //�ؔ���Y���W�ɑ������l
    float positionPrevY_;        //1�t���[���O��Y���W
    float positionTempY_;        //1���I�ɍ��W��ۑ�
    bool isJump_;           //���ł��邩
    int  isOnWoodBox_;      //�ؔ��̏�ɂ��邩
    float rayWoodBoxDist_;  //�ؔ��̃f�B�X�g
    float rayStageDistDown_;    //�X�e�[�W�̃f�B�X�g
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

    void SetWoodBoxBreak() { isBreak_ = true; }
};