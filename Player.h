#pragma once
//�C���N���[�h
#include "Engine/GameObject.h"
#include "Engine/Camera.h"

/// <summary>
/// �v���C���[���Ǘ�����N���X
/// </summary>
class Player : public GameObject
{
public:
	int hModel_;	//���f���ԍ�
public:

	/// <summary>
	/// �R���X�g���N�^�֐�
	/// </summary>
	/// <param name="_parent">�e�̖��O</param>
	Player(GameObject* _pParent);

	/// <summary>
	/// �f�X�g���N�^�֐�
	/// </summary>
	~Player();

	/// <summary>
	/// �������֐�
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V�֐�
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �J���֐�
	/// </summary>
	void Release() override;
};

