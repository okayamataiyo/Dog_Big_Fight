#pragma once
//インクルード
#include "../Engine/GameObject.h"
#include "../Engine/Camera.h"
#include "../Button.h"

namespace
{
	std::string selectSceneName = "SelectScene";
}

class SceneManager;
class StageObjectManager;
class SolidText;


class SelectScene : public GameObject
{
private:

	enum class PADIDSTATE
	{
		FIRST = 0,
		SECONDS,
		THIRD,
		FOUR,
	};

	float solidTextRotate_;
	SceneManager* pSceneManager_;
	StageObjectManager* pStageObjectManager_;
	SolidText* pSolidText_;
	Button* buttonStart_;
	Button* buttonBack_;
	XMFLOAT3 camPos_;
public:
	SelectScene(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void ShowCursor() { while (::ShowCursor(true) < 0); }
};

