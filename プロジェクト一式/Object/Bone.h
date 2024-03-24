#pragma once
//インクルード
#include "ObjectBase.h"

class PlayScene;

namespace
{
	std::string boneName = "Bone";
}

/// <summary>
/// アイテムを管理するクラス
/// </summary>
class Bone : public ObjectBase
{
protected:
	int hModel_;
	int hSound_;
	PlayScene* pPlayScene_;
	float rayDist_;
public:
	/// <summary>
	/// コンストラクタ関数
	/// </summary>
	/// <param name="_parent">親の名前</param>
	Bone(GameObject* _parent);

	/// <summary>
	/// デストラクタ関数
	/// </summary>
	~Bone();

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

	int GetModelHandle() { return hModel_; }

	/// <summary>
	/// 何かに当たった時の関数
	/// </summary>
	/// <param name="_pTarget">当たった相手</param>
	void OnCollision(GameObject* _pTarget) override;


};