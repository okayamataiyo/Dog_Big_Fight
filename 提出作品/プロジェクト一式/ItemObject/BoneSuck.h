#pragma once
//�C���N���[�h
#include "ItemObjectBase.h"

class PlayScene;
class SphereCollider;
class Stage;
class CollectPlayer;
class Bone;

namespace
{
	std::string boneSuckName = "Bone";
}

/// <summary>
/// �A�C�e�����Ǘ�����N���X
/// </summary>
class BoneSuck : public ItemObjectBase
{
protected:
	int hModel_;
	float rayDist_;
	float positionRotate_;
	float BoneSuckInitPosY_;
	XMFLOAT3 pickUpBoneSuckScale_;
	PlayScene* pPlayScene_;
	SphereCollider* pCollision_;
	Stage* pStage_;
	CollectPlayer* pCollectPlayer_;
	Bone* pBone_;
public:
	/// <summary>
	/// �R���X�g���N�^�֐�
	/// </summary>
	/// <param name="_parent">�e�̖��O</param>
	BoneSuck(GameObject* _parent);

	/// <summary>
	/// �f�X�g���N�^�֐�
	/// </summary>
	~BoneSuck();

	/// <summary>
	/// �������֐�
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �J���֐�
	/// </summary>
	void Release() override;

	void PlayerSuckBoneSuck();

	void BoneSuckDeath();

	int GetModelHandle() { return hModel_; }

	/// <summary>
	/// �����ɓ����������̊֐�
	/// </summary>
	/// <param name="_pTarget">������������</param>
	void OnCollision(GameObject* _pTarget) override;


};