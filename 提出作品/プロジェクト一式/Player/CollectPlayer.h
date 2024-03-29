#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"
#include "PlayerBase.h"

class PlayScene;
class AttackPlayer;
class SphereCollider;
class WoodBox;
class Text;
class Stage;
class StageBlock;
class Floor;

namespace
{
	std::string collectPlayerName = "CollectPlayer";
}

/// <summary>
/// ���W����v���C���[���Ǘ�����N���X
/// </summary>
class CollectPlayer : public PlayerBase
{
private:
	int hModel_;					//���f���ԍ�
	int hSound_[5];					//�T�E���h�ԍ�
	int stageBlockHModel_;
	int stageHModel_;
	int floorHModel_;
	bool isBoneTatch_;
	int number_;
	int time_;
	int timeWait_;
	bool isPush_;
	bool isPushed_;
	int pushTime_;
	int pushTimeWait_;
	PLAYERSTATE playerState_;
	PLAYERSTATE playerStatePrev_;
	GAMESTATE gameState_;
	GameObject* pParent_;
	PlayScene* pPlayScene_;
	AttackPlayer* pAttackPlayer_;
	SphereCollider* pCollision_;
	WoodBox* pWoodBox_;
	Text* pText_;
	Stage* pStage_;
	StageBlock* pStageBlock_;
	Floor* pFloor_;
public:

	/// <summary>
	/// �R���X�g���N�^�֐�
	/// </summary>
	/// <param name="_parent">�e�̖��O</param>
	CollectPlayer(GameObject* _pParent);

	/// <summary>
	/// �f�X�g���N�^�֐�
	/// </summary>
	~CollectPlayer();

	/// <summary>
	/// �������֐�
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update() override;

	void UpdateReady();
	void UpdatePlay();
	void UpdateGameOver();
	/// <summary>
	/// �v���C���[���X�^��(�s���s�\)�ɂ��鏈��
	/// </summary>
	/// <param name="_timeLimit">_timeLimit�b�܂ŁA�������Ȃ�</param>
	void Stun(int _timeLimit = 60) override;

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �J���֐�
	/// </summary>
	void Release() override;

	/// <summary>
	/// �����ɓ����������̊֐�
	/// </summary>
	/// <param name="_pTarget">������������</param>
	void OnCollision(GameObject* _pTarget) override;

	/// <summary>
	/// �v���C���[�̈ړ��֐�
	/// </summary>
	void PlayerMove() override;

	/// <summary>
	/// �v���C���[�̃W�����v�֐�
	/// </summary>
	void PlayerJump() override;

	void PushJump();

	void PlayerKnockback() override;

	/// <summary>
	/// �v���C���[�̃��C�֐�
	/// </summary>
	void PlayerRayCast() override;

	void SetVecPos(XMVECTOR _vecMove) override { XMStoreFloat3(&transform_.position_, _vecMove); }

	void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) override;

	void SetAttackPlayer(AttackPlayer* _pAttackPlayer) { pAttackPlayer_ = _pAttackPlayer; }

	XMVECTOR GetVecPos() override { return XMLoadFloat3(&transform_.position_); }

	PLAYERSTATE GetPlayerState() override { return playerState_; }

	int GetPadID()override { return padID_; }

	int GetScore() { return score_; }

	bool IsMoving() override;

	int GetModelHandle() override { return hModel_; };
};

