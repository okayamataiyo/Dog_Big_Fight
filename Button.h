#pragma once
//�C���N���[�h
#include "Engine/Model.h"
#include "Engine/GameObject.h"

class Button : public GameObject
{
private:
	int hModel_;
	int hPushModel_;
	int pushed_;
	XMFLOAT3 center_;	//�{�^���̒��S���W(��ʍ��W)
	XMFLOAT3 size_;		//�{�^���̉摜�T�C�Y
public:
	Button(GameObject* _pParent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void SetPosition(int _x, int _y);		//�{�^���\���ʒu
	void Push(bool _pushed);
	bool MouseInArea(XMFLOAT3 _mousePos);	//�}�E�X���{�^�����ɓ����Ă��邩
};

