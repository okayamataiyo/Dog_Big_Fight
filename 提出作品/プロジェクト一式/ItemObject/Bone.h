#pragma once
//インクルード
#include "ItemObjectBase.h"

class PlayScene;
class SphereCollider;
class Stage;

namespace
{
	std::string boneName = "Bone";
}

/// <summary>
/// アイテムを管理するクラス
/// </summary>
class Bone : public ItemObjectBase
{
protected:
	int hModel_;
	float rayDist_;
	float positionRotate_;
	float boneInitPosY_;
	int decBoneCount_;
	PlayScene* pPlayScene_;
	SphereCollider* pCollision_;
	Stage* pStage_;
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