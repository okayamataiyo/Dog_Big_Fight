#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "SelectScene.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/SolidText.h"

SelectScene::SelectScene(GameObject* _pParent)
	:GameObject(_pParent, "SelectScene"),pStageObjectManager_(nullptr)
{

}

void SelectScene::Initialize()
{
	Direct3D::SetIsChangeView(1);
	pText_ = Instantiate<SolidText>(this);
	pText_->SetMode(1);
	//buttonStart_ = Instantiate<Button>(this);
	pStageObjectManager_ = new StageObjectManager(this);
	pStageObjectManager_->CreateStageObjectOrigin(STAGEOBJECTSTATE::Sky);
}

void SelectScene::Update()
{
	camPos_ = pText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	Camera::SetPosition(camPos_, 0);
	Camera::SetTarget(pText_->GetPosition(), 0);
	Camera::SetPosition(camPos_, 1);
	Camera::SetTarget(pText_->GetPosition(), 1);

	transform_.rotate_.y += 0.3f;

	XMFLOAT3 pos = Input::GetMousePosition();
//	if (buttonStart_->MouseInArea(pos))
	{

	}
	if (Input::IsKeyDown(DIK_E) || Input::IsMouseButtonDown(0) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		Direct3D::SetIsChangeView(2);
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
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

void SelectScene::Draw()
{

}

void SelectScene::Release()
{
}
