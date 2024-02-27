#pragma once
//インクルード
#include "Engine/Model.h"
#include "Engine/GameObject.h"

class Button : public GameObject
{
private:
	int hModel_;
	int hPushModel_;
	int pushed_;
	XMFLOAT3 center_;	//ボタンの中心座標(画面座標)
	XMFLOAT3 size_;		//ボタンの画像サイズ
public:
	Button(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetPosition(int _x, int _y);		//ボタン表示位置
	void Push(bool _pushed);
	bool MouseInArea(XMFLOAT3 _mousePos);	//マウスがボタン内に入っているか
};

