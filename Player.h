#pragma once
//インクルード
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "PlayScene.h"

class WoodBox;
class Text;
enum class PLAYERSTATE
{
	WAIT = 0,
	WALK,
	RUN,
	JUMP,
};

enum class GAMESTATE
{
	READY = 0,
	PLAY,
	GAMEOVER,
};

/// <summary>
/// プレイヤーを管理するクラス
/// </summary>
class Player : public GameObject
{
private:
	int hModel_;					//モデル番号
	int number_;
	Player* pPlayer_;
	GameObject* pParent_;
	PlayScene* pPlayScene_;
	SphereCollider* pCollision_;
	Text* pText_;
	std::string woodBoxName_;
	std::string woodBoxNumber_;
	PLAYERSTATE playerState_;
	PLAYERSTATE prevState_;			//前のプレイヤーの状態
	GAMESTATE gameState_;
	//▼ゲームの演出で使うメンバ関数
	int timeCounter_;
	int score_;			//得点
	//▼移動で使うメンバ変数
	float posY_;	//プレイヤーのY座標に代入する値
	float controllerMoveSpeed_;
	float mouseMoveSpeed_;
	bool isDash_;	//ダッシュしているかどうか
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
	bool  isJump_;				//ジャンプしてるかしていないか
	float rayStageDist_;		//地面とプレイヤーの差分
	float posYTemp_;	//y座標をPrevに保存する
	float posYPrev_;	//1時的にy座標を保存しておく
	//▼すり抜け床で使うメンバ変数
	int isOnFloor_;		//すり抜け床にレイを飛ばしているかどうか
	float rayUpDist_;	//上の物体とプレイヤーの差分
	float rayFloorDist_;	//下のすり抜けたい物体とプレイヤーの差分
	//▼木箱で使うメンバ変数
	float dotProduct_;
	float angleDegrees_;
	WoodBox* pWoodBox_;
	//▼ノックバックで使うメンバ変数
	bool isKnockBack_;
	int stunLimit_;		//スタンしている時間
	int isStun_;		//スタンしているかどうか
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
	/// プレイヤーをスタン(行動不能)にする処理
	/// </summary>
	/// <param name="_timeLimit">_timeLimit秒まで、動かせない</param>
	void Stun(int _timeLimit = 60);

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

	void PlayerKnockback();

	/// <summary>
	/// プレイヤーのレイ関数
	/// </summary>
	void PlayerRayCast();

	void SetVecPos(XMVECTOR _vecMove) { XMStoreFloat3(&transform_.position_, _vecMove); }

	void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f);

	XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }

	XMVECTOR GetVecMove() { return vecMove_; }

	PLAYERSTATE GetPlayerState() { return playerState_; }

	bool IsMoving();
};

