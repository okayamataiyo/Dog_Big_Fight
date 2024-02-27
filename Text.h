#pragma once
//インクルード
#include "Engine/Model.h"
#include "Engine/GameObject.h"

class Text : public GameObject
{
private:

	enum TEXTSTATE
	{
		GameOver = 0,
		Select,
	}textState_;
	int hModel_[2];
public:
	Text(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetMode(int _mode);
};

