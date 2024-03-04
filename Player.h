#pragma once
//�C���N���[�h
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "PlayScene.h"

class WoodBox;

enum class PLAYERSTATE
{
	WAIT = 0,
	WALK,
	RUN,
	JUMP,
};

enum class GAMESTATE
{
	READY = 0,
	PLAY,
	GAMEOVER,
	FIRSTSTUN,
	SECONDSSTUN
};

/// <summary>
/// �v���C���[���Ǘ�����N���X
/// </summary>
class Player : public GameObject
{
private:
	int hModel_;					//���f���ԍ�
	int number_;
	Player* pPlayer_;
	GameObject* pParent_;
	PlayScene* pPlayScene_;
	std::string woodBoxName_;
	std::string woodBoxNumber_;
	SphereCollider* pCollision_;
	PLAYERSTATE playerState_;
	PLAYERSTATE prevState_;			//�O�̃v���C���[�̏��
	GAMESTATE gameState_;
	//���Q�[���̉��o�Ŏg�������o�֐�
	int TimeCounter_;
	int isStun_;		//�X�^�����Ă��邩�ǂ���

	//���ړ��Ŏg�������o�ϐ�
	float posY_;	//�v���C���[��Y���W�ɑ������l
	bool isDash_;	//�_�b�V�����Ă��邩�ǂ���
	//�������ς��Ŏg�������o�ϐ�
	XMVECTOR vecMove_[2];
	XMVECTOR vecLength_[2];
	XMVECTOR vecFront_;
	XMVECTOR vecDot_[2];
	XMVECTOR vecCross_;
	float length_[2];
	float dot_;
	float angle_[2];
	//���W�����v�Ŏg�������o�ϐ�
	bool  isJump_;				//�W�����v���Ă邩���Ă��Ȃ���
	float rayStageDist_;		//�n�ʂƃv���C���[�̍���
	float posYTemp_;	//y���W��Prev�ɕۑ�����
	float posYPrev_;	//1���I��y���W��ۑ����Ă���

	//�����蔲�����Ŏg�������o�ϐ�
	int isOnFloor_;		//���蔲�����Ƀ��C���΂��Ă��邩�ǂ���
	float rayUpDist_;	//��̕��̂ƃv���C���[�̍���
	float rayFloorDist_;	//���̂��蔲���������̂ƃv���C���[�̍���

	//���ؔ��Ŏg�������o�ϐ�
	float dotProduct_;
	float angleDegrees_;
	WoodBox* pWoodBox_;
	//���m�b�N�o�b�N�Ŏg�������o�ϐ�

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
	/// �v���C���[���X�^��(�s���s�\)�ɂ��鏈��
	/// </summary>
	/// <param name="_timeLimit">_timeLimit�b�܂ŁA�������Ȃ�</param>
	void UpdateStun(int _timeLimit = 60);

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

	XMVECTOR GetVecMove(int _type) { return vecMove_[_type]; }

	PLAYERSTATE GetPlayerState() { return playerState_; }

	bool IsMoving();
};

