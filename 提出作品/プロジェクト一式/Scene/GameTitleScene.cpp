#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "../Engine/Image.h"
#include "../Engine/Audio.h"
#include "GameTitleScene.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/SolidText.h"

GameTitleScene::GameTitleScene(GameObject* _pParent)
	:GameObject(_pParent, "GameTitleScene"),pText_(nullptr),pStageObjectManager_(nullptr),hSound_(-1)
{

}

void GameTitleScene::Initialize()
{
	//サウンドデータのロード
	hSound_ = Audio::Load("Sound/TitleBGM.wav");
	assert(hSound_ >= 0);
	Direct3D::SetIsChangeView(1);
	pText_ = Instantiate<SolidText>(this);
	pText_->SetMode(2);
	XMFLOAT3 positionStage = { 0.0f,0.0f,120.0f };
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::Sky);
	pStageObjectManager_->CreateStageObject(STAGEOBJECTSTATE::Stage, positionStage);
}

void GameTitleScene::Update()
{
	Audio::Play(hSound_, 0.05f);
	camPos_ = pText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	Camera::SetPosition(camPos_, 0);
	Camera::SetTarget(pText_->GetPosition(), 0);
	if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown(0) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A,0) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A,1))
	{
		Direct3D::SetIsChangeView(2);
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_SELECT);
	}
	if (Input::IsKeyDown(DIK_R))
	{
		Direct3D::SetIsChangeView(4);
	}
	if (Input::IsKeyDown(DIK_T))
	{
		Direct3D::SetIsChangeView(3);
	}
	if (Input::IsKeyDown(DIK_Y))
	{
		Direct3D::SetIsChangeView(2);
	}
	if (Input::IsKeyDown(DIK_U))
	{
		Direct3D::SetIsChangeView(1);
	}
}

void GameTitleScene::Draw()
{
}

void GameTitleScene::Release()
{
}
