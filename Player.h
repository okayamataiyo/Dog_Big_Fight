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
	int hModel_;	//モデル番号
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

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 開放関数
	/// </summary>
	void Release() override;
};

