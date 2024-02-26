#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "PlayScene.h"
#include "Stage.h"
#include "Item.h"
#include "Engine/ImGui/imgui.h"

PlayScene::PlayScene(GameObject* _pParent)
	:GameObject(_pParent, "PlayScene")
{

}

void PlayScene::Initialize()
{
	Instantiate<Item>(this);
	Instantiate<Stage>(this);
	for (int i = 0u; i <= 1; i++)
	{
		pPlayer_[i] = Instantiate<Player>(this);
		camVec_[i].x = 0;
		camVec_[i].y = 5;
		camVec_[i].z = -10;
	}
	pPlayer_[0]->SetObjectName("PlayerFirst");
	pPlayer_[1]->SetObjectName("PlayerSeconds");
	//pSky_ = Instantiate<Sky>(this);
	//pSky_->SetObjectName("SkyFirst");
	//pCamera_ = new Camera;
	XMFLOAT3 FPlayerPos = { 3,0,0 };
	XMFLOAT3 SPlayerPos = { -3,0,0 };
	XMFLOAT3 skyRot = { 0,150,90 };
	XMFLOAT3 skyPos = { 0, -100, 0 };
	//pSky_->SetRotate(skyRot);
	//pSky_->SetPosition(skyPos);
	pPlayer_[0]->SetPosition(FPlayerPos);
	pPlayer_[1]->SetPosition(SPlayerPos);

}

void PlayScene::Update()
{
	for (int i = 0u; i <= 1; i++)
	{
		XMVECTOR vPos[2] = {};
		XMFLOAT3 mouse = {};
		XMFLOAT3 controller = {};
		XMFLOAT3 rDir = { 0,0,1 };
		XMVECTOR Dir[2] = {};
		float sigmaRotY[2] = {};
		float sigmaRotX[2] = {};
		vPos[i] = pPlayer_[i]->GetVecPos();
		mouse = Input::GetMouseMove();
		controller = Input::GetPadStickR();
		ImGui::Text("mouse.x=%f", mouse.x);
		ImGui::Text("mouse.y=%f", mouse.y);
		ImGui::Text("mouse.z=%f", mouse.z);
		ImGui::Text("controller.x=%f", controller.x);
		ImGui::Text("controller.y=%f", controller.y);
		ImGui::Text("controller.z=%f", controller.z);

		const float mouseSens = 400;
		const float controllerSens = 100;
		static float RotationX[2] = {};
		static float RotationY[2] = {};
		static float vecLength[2] = {};
		static float prevLen[2] = {};
		static float prevRotX[2] = {};

		RotationX[0] = controller.x;
		RotationY[0] = controller.y;
		RotationX[1] = mouse.x;
		RotationY[1] = mouse.y;
		vecLength[1] -= (mouse.z) / 50;

		Dir[i] = XMLoadFloat3(&rDir);

		//Dir = Dir * (pPlayer_[i]->GetRotate().x + RotationX[i]) * (pPlayer_[i]->GetRotate().y + RotationY[i]);
		//Dir = Dir + (vecLength * 2);
		camVec_[1].y += RotationY[1] / mouseSens;
		camVec_[1].x += RotationX[1] / mouseSens;
		camVec_[0].y += RotationY[0] / controllerSens;
		camVec_[0].x += RotationX[0] / controllerSens;
		//camVec_[i].z += vecLength[i]  + 10;

		sigmaRotY[i] = camVec_[i].y;// +pPlayer_[i]->GetRotate().y;
		sigmaRotY[i] = -camVec_[i].x;// + EasingX[i]; +pPlayer_[i]->GetRotate().x;
		//vecLength[i] = camVec_[i].z;

		if (sigmaRotX[i] > 0 * (3.14 / 180))
		{
			sigmaRotX[i] = 0;
			camVec_[1].x -= RotationY[1] / mouseSens;
			camVec_[0].x -= RotationY[0];// / mouseSens;
		}
		if (sigmaRotX[i] < -88 * (3.14 / 180))
		{
			sigmaRotX[i] = -87.9 * (3.14 / 180);
			camVec_[1].x -= RotationY[1] / mouseSens;
			camVec_[0].x -= RotationY[0];// / mouseSens;
		}

		prevRotX[i] = sigmaRotX[i];

		XMMATRIX mxRotX[2] = {};
		mxRotX[i] = XMMatrixRotationX(sigmaRotX[i]);
		XMMATRIX mxRotY[2] = {};
		mxRotY[i] = XMMatrixRotationY(sigmaRotY[i]);

		XMMATRIX rot[2] = {};
		rot[i] = mxRotX[i] * mxRotY[i];
		XMFLOAT3 playerPos[2] = {};
		playerPos[i] = pPlayer_[i]->GetPosition();

		Dir[i] = XMVector3Transform(Dir[i], rot[i]);
		Dir[i] = XMVector3Normalize(Dir[i]);
		Dir[i] = Dir[i] * (vecLength[i] + 10);
		Dir[i] += XMLoadFloat3(&playerPos[i]);
		//camVec_ = pPlayer_[j]->GetPosition();
		//camVec_.y += 5;
		//camVec_.z += -10;
		//auto a = pPlayer_[j]->GetPosition();

		//pCamera_->SetPosition(Dir, i);
		//pCamera_->SetTarget(pPlayer_[i]->GetPosition(), i);
		XMFLOAT3 floatDir[2] = {};
		XMStoreFloat3(&floatDir[i], Dir[i]);
		Camera::SetPosition(floatDir[i], i);
		Camera::SetTarget(pPlayer_[i]->GetPosition(), i);

		prevLen[i] = vecLength[i];
	}
}

void PlayScene::Draw()
{

}

void PlayScene::Release()
{
}
