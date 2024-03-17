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
class Floor;

namespace
{
	std::string collectPlayerName = "CollectPlayer";
}

/// <summary>
/// �v���C���[���Ǘ�����N���X
/// </summary>
class CollectPlayer : public PlayerBase
{
private:
	int hModel_;					//���f���ԍ�
	int hStageModel_;
	int hFloorModel_;
	int number_;
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

	bool IsMoving() override;

	int GetModelHandle() override { return hModel_; };
};

