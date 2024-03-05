#pragma once
//�C���N���[�h
#include "Engine/GameObject.h"
#include "Engine/Camera.h"
#include "PlayScene.h"

class WoodBox;
class Text;
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
	SphereCollider* pCollision_;
	Text* pText_;
	std::string woodBoxName_;
	std::string woodBoxNumber_;
	PLAYERSTATE playerState_;
	PLAYERSTATE prevState_;			//�O�̃v���C���[�̏��
	GAMESTATE gameState_;
	//���Q�[���̉��o�Ŏg�������o�֐�
	int timeCounter_;
	int score_;			//���_
	//���ړ��Ŏg�������o�ϐ�
	float posY_;	//�v���C���[��Y���W�ɑ������l
	float controllerMoveSpeed_;
	float mouseMoveSpeed_;
	bool isDash_;	//�_�b�V�����Ă��邩�ǂ���
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
	bool isKnockBack_;
	int stunLimit_;		//�X�^�����Ă��鎞��
	int isStun_;		//�X�^�����Ă��邩�ǂ���
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
	void Stun(int _timeLimit = 60);

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

	void PlayerKnockback();

	/// <summary>
	/// �v���C���[�̃��C�֐�
	/// </summary>
	void PlayerRayCast();

	void SetVecPos(XMVECTOR _vecMove) { XMStoreFloat3(&transform_.position_, _vecMove); }

	void SetKnockback(XMVECTOR _vecKnockbackDirection, float _knockbackSpeed = 0.5f);

	XMVECTOR GetVecPos() { return XMLoadFloat3(&transform_.position_); }

	XMVECTOR GetVecMove() { return vecMove_; }

	PLAYERSTATE GetPlayerState() { return playerState_; }

	bool IsMoving();
};

