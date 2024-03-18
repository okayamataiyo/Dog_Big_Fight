#pragma once
//�C���N���[�h
#include "../Engine/GameObject.h"

class  PlayerBase : public GameObject
{
public:
	enum class PLAYERSTATE
	{
		WAIT = 0,
		WALK,
		RUN,
		JUMP,
		STUN,
	};

	enum class GAMESTATE
	{
		READY = 0,
		PLAY,
		GAMEOVER,
	};

	//���Q�[���̉��o�Ŏg�������o�ϐ�
	int timeCounter_;
	int score_;			//���_
	//���ړ��Ŏg�������o�ϐ�
	XMFLOAT3 positionPrev_;		//1�t���[���O�̈ʒu���W
	float controllerMoveSpeed_;
	float mouseMoveSpeed_;
	float positionY_;			//�v���C���[��Y���W�ɑ������l
	bool isDash_;				//�_�b�V�����Ă��邩�ǂ���
	float isFling_;				//�n�ʂ��痣��Ă��邩
	//�������ς��Ŏg�������o�ϐ�
	XMVECTOR vecMove_;
	XMVECTOR vecLength_;
	XMVECTOR vecFront_;
	XMVECTOR vecDot_;
	XMVECTOR vecCross_;
	float length_;
	float dot_;
	float angle_;
	//���W�����v�Ŏg�������o�ϐ�
	float positionTempY_;		//y���W��Prev�ɕۑ�����
	float positionPrevY_;		//1���I��y���W��ۑ����Ă���
	bool  isJump_;				//�W�����v���Ă邩���Ă��Ȃ���
	//�����蔲�����Ŏg�������o�ϐ�
	int isOnFloor_;		//���蔲�����Ƀ��C���΂��Ă��邩�ǂ���
	//���ؔ��Ŏg�������o�ϐ�
	std::string woodBoxName_;
	std::string woodBoxNumber_;
	float dotProduct_;
	float angleDegrees_;
	//���m�b�N�o�b�N�Ŏg�������o�ϐ�
	int stunTimeCounter_;	//�X�^�����Ă���ǂ̂��炢�o������
	int stunLimit_;		//�X�^�����Ă��鎞��
	bool isStun_;		//�X�^�����Ă��邩�ǂ���
	bool isKnockBack_;	//�m�b�N�o�b�N���Ă��邩�ǂ���
	//�������Ŏg�������o�ϐ�

	//���ǔ���Ŏg�������o�ϐ�
	float rayFloorDistUp_;			//��̂��蔲�����ƃv���C���[�̍�����
	float rayFloorDistDown_;		//���̂��蔲�����ƃv���C���[�̍�����
	float rayStageDistDown_;		//�n�ʂƃv���C���[�̍�����
	float rayStageDistFront_;		//�O�ǂƃv���C���[�̍�����
	float rayStageDistBack_;		//��ǂƃv���C���[�̍�����
	float rayStageDistLeft_;		//���ǂƃv���C���[�̍�����
	float rayStageDistRight_;		//�E�ǂƃv���C���[�̍�����
protected:
	PlayerBase(GameObject* _parent, std::string _name)
		:GameObject(_parent, _name) {}
	//������
	void Initialize()   override {};

	//�X�V
	void Update()       override {};

	//�`��
	void Draw()         override {};

	//�J��
	void Release()      override {};

	virtual void UpdateReady() = 0;

	virtual void UpdatePlay() = 0;

	virtual void UpdateGameOver() = 0;

	virtual void PlayerMove() = 0;

	virtual void PlayerJump() = 0;

	virtual void PlayerKnockback() = 0;

	virtual void PlayerRayCast() = 0;

	virtual void Stun(int _timeLimit = 60) = 0;

	virtual void SetVecPos(XMVECTOR _vecMove) = 0;

	virtual void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f) = 0;

	virtual XMVECTOR GetVecPos() = 0;

	virtual PLAYERSTATE GetPlayerState() = 0;

	virtual bool IsMoving() = 0;

	virtual int GetModelHandle() = 0;
};

