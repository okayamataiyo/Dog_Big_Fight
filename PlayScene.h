#pragma once
//�C���N���[�h
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "ObjectManager.h"
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
    Transform floorPos_[3];
    //�����̏����Ŏg�������o�ϐ�
    int boneCount_;         //�����鍜�̐�
    bool isCreateBone_;     //������������ǂ���
    //���ؔ��̏����Ŏg�������o�ϐ�
    int playerFirstCreatWoodBoxNum_;   //�v���C���[1�����ؔ����������
    int playerSecondsCreatWoodBoxNum_; //�v���C���[2�����ؔ����������
    //�����o�Ŏg�������o�ϐ�
    int time_;                         //���Ԃ�\��
    int blockOrCollect_;               //�ז����鑤�����W���鑤��
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
    void AddPlayerFirstWoodBoxNum(int _woodBoxNum) { playerFirstCreatWoodBoxNum_ += _woodBoxNum; }
    void AddPlayerSecondsWoodBoxNum(int _woodBoxNum) { playerSecondsCreatWoodBoxNum_ += _woodBoxNum; }
};

