#include "Camera.h"
#include "Direct3D.h"

XMFLOAT3 position_[2];
XMFLOAT3 target_[2];
XMMATRIX view_;
XMMATRIX proj_;
XMMATRIX billBoard_;


//�������i�v���W�F�N�V�����s��쐬�j
void Camera::Initialize()
{
	for (int i = 0u; i <= 1; i++)
	{
		position_[i] = XMFLOAT3(0, 3 * i, -20);	//�J�����̈ʒu
		target_[i] = XMFLOAT3(0, 0, 0);	//�J�����̏œ_
	}
	//�v���W�F�N�V�����s��
	proj_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)(Direct3D::screenWidth_/2) / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//�X�V�i�r���[�s��쐬�j
void Camera::Update(int _type)
{
	//�r���[�s��
	view_ = XMMatrixLookAtLH(XMVectorSet(position_[_type].x, position_[_type].y, position_[_type].z, 0),
		XMVectorSet(target_[_type].x, target_[_type].y, target_[_type].z, 0), XMVectorSet(0, 1, 0, 0));

	//�r���{�[�h�s��
	//�i��ɃJ�����̕��������悤�ɉ�]������s��B�p�[�e�B�N���ł����g��Ȃ��j
	billBoard_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMLoadFloat3(&target_[_type])- XMLoadFloat3(&position_[_type]), XMVectorSet(0, 1, 0, 0));
	billBoard_ = XMMatrixInverse(nullptr, billBoard_);
}

//�œ_��ݒ�
void Camera::SetTarget(XMFLOAT3 _target,int _type) { target_[_type] = _target; }

//�ʒu��ݒ�
void Camera::SetPosition(XMFLOAT3 _position, int _type) { position_[_type] = _position; }

//�œ_���擾
XMFLOAT3 Camera::GetTarget(int _type) { return target_[_type]; }

XMVECTOR Camera::VecGetTarget(int _type) { return XMLoadFloat3(&target_[_type]); }

//�ʒu���擾
XMFLOAT3 Camera::GetPosition(int _type) { return position_[_type]; }

XMVECTOR Camera::VecGetPosition(int _type) { return XMLoadFloat3(&position_[_type]); }

//�r���[�s����擾
XMMATRIX Camera::GetViewMatrix() { return view_; }

//�v���W�F�N�V�����s����擾
XMMATRIX Camera::GetProjectionMatrix() { return proj_; }

//�r���{�[�h�p��]�s����擾
XMMATRIX Camera::GetBillboardMatrix(){	return billBoard_; }
