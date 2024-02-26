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
	enum PLYAERS
	{
		PlayerFirst = 0,
		PlayerSeconds
	};

	enum GAMESTATE
	{
		WAIT = 0,
		WALK,
		RUN,
		JUMP,

	}gameState_;
	GAMESTATE prevState_;

	enum STATE
	{
		READY = 0,
		PLAY,
		GAMEOVER,
	}state_;
	int hModel_;	//���f���ԍ�
	int camType_;	//��ʕ����̍��E�ǂ����̃J������(�E�A��)

	SphereCollider* pCollision_;
	//���Q�[���̉��o�Ŏg�������o�֐�
	int TimeCounter_;

	//���ړ��Ŏg�������o�ϐ�
	float posY_;	//�v���C���[��Y���W�ɑ������l
	//�������ς��Ŏg�������o�ϐ�
	XMVECTOR vecMove_[2];
	XMVECTOR vecLength_;
	XMVECTOR vecFront_;
	XMVECTOR vecDot_[2];
	XMVECTOR vecCross_;
	float length_;
	float dot_;
	float angle_[2];
	//���W�����v�Ŏg�������o�ϐ�
	bool  jumpFlg_;				//�W�����v���Ă邩���Ă��Ȃ���
	float rayGravityDist_;		//�n�ʂƃv���C���[�̍���
	float moveYTemp_;	//y���W��Prev�ɕۑ�����
	float moveYPrev_;	//y���W��ۑ����Ă���

	//�����蔲�����Ŏg�������o�ϐ�
	int isFloor_;		//���蔲�����Ƀ��C���΂��Ă��邩�ǂ���
	int prevIsFloor_;	//1�t���[���O��isFloor�t���O���ǂ��Ȃ��Ă�����
	float rayUpDist_;	//��̕��̂ƃv���C���[�̍���
	float rayFloorDist_;	//���̂��蔲���������̂ƃv���C���[�̍���

	//�������Ŏg�������o�ϐ�

	//���ǔ���Ŏg�������o�ϐ�
	float rayFrontDist_;		//�O�ǂ̈ʒu�Ǝ����̈ʒu�̍�����
	float rayBackDist_;			//���ǂ̈ʒu�Ǝ����̈ʒu�̍�����
	float rayLeftDist_;			//���ǂ̈ʒu�Ǝ����̈ʒu�̍�����
	float rayRightDist_;		//�E�ǂ̈ʒu�Ǝ����̈ʒu�̍�����
	XMFLOAT3 prevPosition_;		//1�t���[���O�̈ʒu���W

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

	void UpdateReady();
	void UpdatePlay();
	void UpdateGameOver();

	/// <summary>
	/// �`��֐�
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �J���֐�
	/// </summary>
	void Release() override;

	/// <summary>
	/// �����ɓ����������̊֐�
	/// </summary>
	/// <param name="_pTarget">������������</param>
	void OnCollision(GameObject* _pTarget) override;

	/// <summary>
	/// �v���C���[�̈ړ��֐�
	/// </summary>
	void PlayerMove();

	/// <summary>
	/// �v���C���[�̃W�����v�֐�
	/// </summary>
	void PlayerJump();

	/// <summary>
	/// �v���C���[�̃��C�֐�
	/// </summary>
	void PlayerRayCast();

	XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }

	bool IsMoving();
};
