#pragma once
//インクルード
#include "Engine/GameObject.h"
#include "Engine/Camera.h"

/// <summary>
/// プレイヤーを管理するクラス
/// </summary>
class Player : public GameObject
{
public:
	enum PLAYERSTATE
	{
		WAIT = 0,
		WALK,
		RUN,
		JUMP,
	}playerState_;
	PLAYERSTATE prevState_;	//前のプレイヤーの状態

	enum GAMESTATE
	{
		READY = 0,
		PLAY,
		GAMEOVER,
	}gameState_;
	int hModel_;	//モデル番号

	SphereCollider* pCollision_;
	//▼ゲームの演出で使うメンバ関数
	int TimeCounter_;

	//▼移動で使うメンバ変数
	float posY_;	//プレイヤーのY座標に代入する値
	bool isDash_;	//ダッシュしているかどうか
	//▼向き変えで使うメンバ変数
	XMVECTOR vecMove_[2];
	XMVECTOR vecLength_[2];
	XMVECTOR vecFront_;
	XMVECTOR vecDot_[2];
	XMVECTOR vecCross_;
	float length_[2];
	float dot_;
	float angle_[2];
	//▼ジャンプで使うメンバ変数
	bool  jumpFlg_;				//ジャンプしてるかしていないか
	float rayGravityDist_;		//地面とプレイヤーの差分
	float moveYTemp_;	//y座標をPrevに保存する
	float moveYPrev_;	//y座標を保存しておく

	//▼すり抜け床で使うメンバ変数
	int isFloor_;		//すり抜け床にレイを飛ばしているかどうか
	int prevIsFloor_;	//1フレーム前にisFloorフラグがどうなっていたか
	float rayUpDist_;	//上の物体とプレイヤーの差分
	float rayFloorDist_;	//下のすり抜けたい物体とプレイヤーの差分

	//▼慣性で使うメンバ変数

	//▼壁判定で使うメンバ変数
	float rayFrontDist_;		//前壁の位置と自分の位置の差距離
	float rayBackDist_;			//後ろ壁の位置と自分の位置の差距離
	float rayLeftDist_;			//左壁の位置と自分の位置の差距離
	float rayRightDist_;		//右壁の位置と自分の位置の差距離
	XMFLOAT3 prevPosition_;		//1フレーム前の位置座標

public:

	/// <summary>
	/// コンストラクタ関数
	/// </summary>
	/// <param name="_parent">親の名前</param>
	Player(GameObject* _pParent);

	/// <summary>
	/// デストラクタ関数
	/// </summary>
	~Player();

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
	void PlayerMove();

	/// <summary>
	/// プレイヤーのジャンプ関数
	/// </summary>
	void PlayerJump();

	/// <summary>
	/// プレイヤーのレイ関数
	/// </summary>
	void PlayerRayCast();

	XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }

	bool IsMoving();
};

