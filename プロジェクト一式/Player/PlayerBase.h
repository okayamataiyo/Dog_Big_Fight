#pragma once
//インクルード
#include "../Engine/GameObject.h"

class  PlayerBase : public GameObject
{
public:
	enum class PLAYERSTATE
	{
		WAIT = 0,
		WALK,
		RUN,
		JUMP,
		STUN,
	};

	enum class GAMESTATE
	{
		READY = 0,
		PLAY,
		GAMEOVER,
	};

	//▼ゲームの演出で使うメンバ変数
	int timeCounter_;
	int score_;			//得点
	//▼移動で使うメンバ変数
	XMFLOAT3 positionPrev_;		//1フレーム前の位置座標
	float controllerMoveSpeed_;
	float mouseMoveSpeed_;
	float positionY_;			//プレイヤーのY座標に代入する値
	bool isDash_;				//ダッシュしているかどうか
	float isFling_;				//地面から離れているか
	//▼向き変えで使うメンバ変数
	XMVECTOR vecMove_;
	XMVECTOR vecLength_;
	XMVECTOR vecFront_;
	XMVECTOR vecDot_;
	XMVECTOR vecCross_;
	float length_;
	float dot_;
	float angle_;
	//▼ジャンプで使うメンバ変数
	float positionTempY_;		//y座標をPrevに保存する
	float positionPrevY_;		//1時的にy座標を保存しておく
	bool  isJump_;				//ジャンプしてるかしていないか
	//▼すり抜け床で使うメンバ変数
	int isOnFloor_;		//すり抜け床にレイを飛ばしているかどうか
	//▼木箱で使うメンバ変数
	std::string woodBoxName_;
	std::string woodBoxNumber_;
	float dotProduct_;
	float angleDegrees_;
	//▼ノックバックで使うメンバ変数
	int stunTimeCounter_;	//スタンしてからどのくらい経ったか
	int stunLimit_;		//スタンしている時間
	bool isStun_;		//スタンしているかどうか
	bool isKnockBack_;	//ノックバックしているかどうか
	//▼慣性で使うメンバ変数

	//▼壁判定で使うメンバ変数
	float rayFloorDistUp_;			//上のすり抜け床とプレイヤーの差距離
	float rayFloorDistDown_;		//下のすり抜け床とプレイヤーの差距離
	float rayStageDistDown_;		//地面とプレイヤーの差距離
	float rayStageDistFront_;		//前壁とプレイヤーの差距離
	float rayStageDistBack_;		//後壁とプレイヤーの差距離
	float rayStageDistLeft_;		//左壁とプレイヤーの差距離
	float rayStageDistRight_;		//右壁とプレイヤーの差距離
protected:
	PlayerBase(GameObject* _parent, std::string _name)
		:GameObject(_parent, _name) {}
	//初期化
	void Initialize()   override {};

	//更新
	void Update()       override {};

	//描画
	void Draw()         override {};

	//開放
	void Release()      override {};

	virtual void UpdateReady() = 0;

	virtual void UpdatePlay() = 0;

	virtual void UpdateGameOver() = 0;

	virtual void PlayerMove() = 0;

	virtual void PlayerJump() = 0;

	virtual void PlayerKnockback() = 0;

	virtual void PlayerRayCast() = 0;

	virtual void Stun(int _timeLimit = 60) = 0;

	virtual void SetVecPos(XMVECTOR _vecMove) = 0;

	virtual void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) = 0;

	virtual XMVECTOR GetVecPos() = 0;

	virtual PLAYERSTATE GetPlayerState() = 0;

	virtual bool IsMoving() = 0;

	virtual int GetModelHandle() = 0;
};

