#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Direct3D.h"
#include "SelectScene.h"
#include "../Sky.h"
SelectScene::SelectScene(GameObject* _pParent)
	:GameObject(_pParent, "SelectScene")
{

}

void SelectScene::Initialize()
{
	Direct3D::SetIsChangeView(1);
	pText_ = Instantiate<SolidText>(this);
	pText_->SetMode(1);
	//buttonStart_ = Instantiate<Button>(this);
	Instantiate<Sky>(this);
}

void SelectScene::Update()
{
	camPos_ = pText_->GetPosition();
	camPos_.y += 2;
	camPos_.z -= 15;
	Camera::SetPosition(camPos_, 0);
	Camera::SetTarget(pText_->GetPosition(), 0);
	XMFLOAT3 pos = Input::GetMousePosition();
//	if (buttonStart_->MouseInArea(pos))
	{

	}
	if (Input::IsKeyDown(DIK_G) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
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
