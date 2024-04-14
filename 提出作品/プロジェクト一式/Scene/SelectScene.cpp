//インクルード
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/SolidText.h"
#include "../Player/AttackPlayer.h"
#include "../Player/CollectPlayer.h"
#include "SelectScene.h"

SelectScene::SelectScene(GameObject* _pParent)
	:GameObject(_pParent, selectSceneName),solidTextRotate_{0.3f}
	,pSceneManager_{nullptr}, pStageObjectManager_{nullptr}
{

}

void SelectScene::Initialize()
{
	ShowCursor();
	Direct3D::SetIsChangeView(1);
	pSolidText_ = Instantiate<SolidText>(this);
	pSolidText_->SetMode(1);
	pSceneManager_ = (SceneManager*)FindObject(sceneManagerName);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::SKY);
	camPos_ = pSolidText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
}

void SelectScene::Update()
{
	Camera::SetPosition(camPos_, attackPlayerNumber);
	Camera::SetTarget(pSolidText_->GetPosition(), attackPlayerNumber);
	Camera::SetPosition(camPos_, collectPlayerNumber);
	Camera::SetTarget(pSolidText_->GetPosition(), collectPlayerNumber);

	transform_.rotate_.y += solidTextRotate_;

	XMFLOAT3 pos = Input::GetMousePosition();
	if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown((int)MOUSESTATE::LEFTCLICK) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, attackPlayerNumber) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A, collectPlayerNumber))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFT_BOTHVIEW);
		pSceneManager_->ChangeScene(SCENE_ID_PLAY);
	}
	if (Input::IsKeyDown(DIK_R))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::RIGHT_BOTHVIEW);
	}
	if (Input::IsKeyDown(DIK_T))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::RIGHTVIEW);
	}
	if (Input::IsKeyDown(DIK_Y))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFT_BOTHVIEW);
	}
	if (Input::IsKeyDown(DIK_U))
	{
		Direct3D::SetIsChangeView((int)Direct3D::VIEWSTATE::LEFTVIEW);
	}
}

void SelectScene::Draw()
{

}

void SelectScene::Release()
{
}
