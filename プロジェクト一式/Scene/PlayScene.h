#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"
#include "../Object/ObjectManager.h"
#include <vector>

class AttackPlayer;
class CollectPlayer;
class Sky;
class Floor;

/// <summary>
/// �V�ׂ�V�[��
/// </summary>
class PlayScene : public GameObject
{
private:
    //�����o�ϐ�
    AttackPlayer* pAttackPlayer_;
    CollectPlayer* pCollectPlayer_;
    Sky* pSky_;
    ObjectManager* pObjectManager_;
    XMFLOAT3 camVec_[2];
    Transform floorPosition_[3];
    //�����̏����Ŏg�������o�ϐ�
    int boneCount_;         //�����鍜�̐�
    bool isCreateBone_;     //������������ǂ���
    //���ؔ��̏����Ŏg�������o�ϐ�
    int woodBoxCount_;      //������ؔ��̐�
    XMFLOAT3 attackPlayerPosition_;         //�A�^�b�N�v���C���[�̈ʒu
    XMVECTOR attackPlayerDirection_;   //�A�^�b�N�v���C���[�̌����Ă�ʒu
    float frontPosition_;              //�O�ɂǂ̂��炢�ړ������邩
    //�����o�Ŏg�������o�ϐ�
    int time_;                         //���Ԃ�\��
    int blockOrCollect_;               //�ז����鑤�����W���鑤��

    XMFLOAT3 Init = { 0.0f,0.0f,0.0f };
    XMVECTOR vPos[2] = {};
    XMFLOAT3 mouse = Init;
    XMFLOAT3 controller = {};
    XMFLOAT3 rDir = { 0.0f,0.0f,1.0f };
    XMVECTOR Dir[2] = {};
    float sigmaRotY[2] = {};
    float sigmaRotX[2] = {};
    XMMATRIX mxRotX[2] = {};
    XMMATRIX mxRotY[2] = {};
    XMMATRIX rot[2] = {};
    XMFLOAT3 playerPos[2] = {};
    XMFLOAT3 floatDir[2] = {};

    const float mouseSens = 400;
    const float controllerSens = 50;
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="pParent">�e�I�u�W�F�N�g</param>
    PlayScene(GameObject* _pParent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    std::vector<int> GetWoodBoxs() { return pObjectManager_->GetWoodBoxs(); }
    int GetBlockOrCollect() { return blockOrCollect_; }
    void AddBoneCount(int _boneCount) { boneCount_ += _boneCount; }
    void AddWoodBoxCount(int _woodBoxCount) { woodBoxCount_ += _woodBoxCount; }
};

