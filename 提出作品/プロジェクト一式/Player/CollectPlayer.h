#pragma once
//インクルード
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
/// 収集するプレイヤーを管理するクラス
/// </summary>
class CollectPlayer : public PlayerBase
{
private:
	int hModel_;					//モデル番号
	int hSound_[5];					//サウンド番号
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
	/// コンストラクタ関数
	/// </summary>
	/// <param name="_parent">親の名前</param>
	CollectPlayer(GameObject* _pParent);

	/// <summary>
	/// デストラクタ関数
	/// </summary>
	~CollectPlayer();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	void UpdateReady();
	void UpdatePlay();
	void UpdateGameOver();
	/// <summary>
	/// プレイヤーをスタン(行動不能)にする処理
	/// </summary>
	/// <param name="_timeLimit">_timeLimit秒まで、動かせない</param>
	void Stun(int _timeLimit = 60) override;

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 開放関数
	/// </summary>
	void Release() override;

	/// <summary>
	/// 何かに当たった時の関数
	/// </summary>
	/// <param name="_pTarget">当たった相手</param>
	void OnCollision(GameObject* _pTarget) override;

	/// <summary>
	/// プレイヤーの移動関数
	/// </summary>
	void PlayerMove() override;

	/// <summary>
	/// プレイヤーのジャンプ関数
	/// </summary>
	void PlayerJump() override;

	void PushJump();

	void PlayerKnockback() override;

	/// <summary>
	/// プレイヤーのレイ関数
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

