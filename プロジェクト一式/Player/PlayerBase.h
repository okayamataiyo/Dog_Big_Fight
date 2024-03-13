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
	};

	enum class GAMESTATE
	{
		READY = 0,
		PLAY,
		GAMEOVER,
	};

	//���Q�[���̉��o�Ŏg�������o�ϐ�
	struct DirectStruct
	{
		int timeCounter_;
		int score_;			//���_
	};
	//���ړ��Ŏg�������o�ϐ�
	struct MoveStruct
	{
		XMFLOAT3 positionPrev_;		//1�t���[���O�̈ʒu���W
		float controllerMoveSpeed_;
		float mouseMoveSpeed_;
		float positionY_;			//�v���C���[��Y���W�ɑ������l
		bool isDash_;				//�_�b�V�����Ă��邩�ǂ���
	};
	//�������ς��Ŏg�������o�ϐ�
	struct DirectionStruct
	{
		XMVECTOR vecMove_;
		XMVECTOR vecLength_;
		XMVECTOR vecFront_;
		XMVECTOR vecDot_;
		XMVECTOR vecCross_;
		float length_;
		float dot_;
		float angle_;
	};
	//���W�����v�Ŏg�������o�ϐ�
	struct JumpStruct
	{
		float positionTempY_;		//y���W��Prev�ɕۑ�����
		float positionPrevY_;		//1���I��y���W��ۑ����Ă���
		bool  isJump_;				//�W�����v���Ă邩���Ă��Ȃ���
	};
	//�����蔲�����Ŏg�������o�ϐ�
	struct FloorStruct
	{
		int isOnFloor_;		//���蔲�����Ƀ��C���΂��Ă��邩�ǂ���
	};
	//���ؔ��Ŏg�������o�ϐ�
	struct WoodBoxStruct
	{
		std::string woodBoxName_;
		std::string woodBoxNumber_;
		float dotProduct_;
		float angleDegrees_;
	};
	//���m�b�N�o�b�N�Ŏg�������o�ϐ�
	struct KnockbackStruct
	{
		int stunTimeCounter_;	//�X�^�����Ă���ǂ̂��炢�o������
		int stunLimit_;		//�X�^�����Ă��鎞��
		int isStun_;		//�X�^�����Ă��邩�ǂ���
		bool isKnockBack_;	//�m�b�N�o�b�N���Ă��邩�ǂ���
	};
	//�������Ŏg�������o�ϐ�

	//���ǔ���Ŏg�������o�ϐ�
	struct RayCastDistStruct
	{
		float rayStageDist_;		//�n�ʂƃv���C���[�̍�����
		float rayDownFloorDist_;	//���̂��蔲�����ƃv���C���[�̍�����
		float rayUpFloorDist_;		//��̂��蔲�����ƃv���C���[�̍�����
		float rayFrontDist_;		//�O�ǂƃv���C���[�̍�����
		float rayBackDist_;			//��ǂƃv���C���[�̍�����
		float rayLeftDist_;			//���ǂƃv���C���[�̍�����
		float rayRightDist_;		//�E�ǂƃv���C���[�̍�����
	};
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

