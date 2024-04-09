//インクルード
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Image.h"
#include "../Engine/Audio.h"
#include "../Engine/Global.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/SolidText.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(GameObject* _pParent)
	:GameObject(_pParent, gameOverSceneName), hSound_{ -1 }, inputWait_{ 0 }, inputWaitTime_{ 60 }
	, pStageObjectManager_{nullptr}, pSceneManager_{nullptr}
{

}

void GameOverScene::Initialize()
{
	ShowCursor();
	//サウンドデータのロード
	std::string soundName = soundFolderName + soundGameOverSceneNmae + soundModifierName;
	hSound_ = Audio::Load(soundName);
	assert(hSound_ >= initializeZero);
	//画像データのロード
	pText_ = Instantiate<SolidText>(this);
	pText_->SetMode(0);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
}

void GameOverScene::Update()
{
	Audio::Play(hSound_, 0.1f);
	camPos_ = pText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	Camera::SetPosition(camPos_, attackPlayerNumber);
	Camera::SetTarget(pText_->GetPosition(), attackPlayerNumber);
	++inputWait_;
	if (inputWait_ >= 60)
	{
		if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown(0) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, 0), Input::IsPadButtonDown(XINPUT_GAMEPAD_A, 1))
		{
			Direct3D::SetIsChangeView(2);
			pSceneManager_->ChangeScene(SCENE_ID_GAMETITLE);
			inputWait_ = 0;
		}
	}
}

void GameOverScene::Draw()
{
}

void GameOverScene::Release()
{
}
