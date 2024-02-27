#pragma once
//インクルード
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "Text.h"
#include "Button.h"
class SelectScene : public GameObject
{
	Text* pText_;
	Button* buttonStart_;
	Button* buttonBack_;
	XMFLOAT3 camPos_;
public:
	SelectScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

