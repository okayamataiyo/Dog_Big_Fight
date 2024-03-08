#pragma once
#include "Direct3D.h"
#include <string>

struct ObjectName
{
	std::string woodBoxName = "WoodBox";
	std::string floorName = "Floor";
	std::string boneName = "Bone";
	std::string stageName = "Stage";
	std::string skyName = "Sky";
};

struct PlayerName
{
	std::string attackPlayer = "AttackPlayer";
	std::string collectPlayer = "CollectPlayer";
};

struct ModelName
{
	std::string fbx = ".fbx";
};

struct SceneName
{
	std::string sceneManager = "SceneManager";
	std::string playScene = "PlayScene";
	std::string gameTitleScene = "GameTitleScene";
	std::string gameOverScene = "GameOverScene";
	std::string selectScene = "SelectScene";
};



//���S�Ƀ��������J�����邽�߂̃}�N��
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

//0�ɏ�����
#define ZERO_INITIALIZE(p) { p = 0;}

#define ZERO_XMFLOAT3_INITIALIZE(p){p = {0,0,0};}

//1�ɏ�����
#define ONE_INITIALIZE(p) { p = 1;}

//�����ŏ�����
#define ARGUMENT_INITIALIZE(p,t){ p = t;}

// INI�t�@�C������float���擾����֐�
static float GetPrivateProfileFloat(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFileName)
{
	// �t�@�C����     
	TCHAR buffer[MAX_PATH];
	// ��������擾     
	DWORD result = GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buffer, sizeof(buffer), lpFileName);
	// �����񂪎擾�ł�����float�ɕϊ����Ēl��Ԃ�     
	if (result != 0) return static_cast<float>(atof(buffer));
	else return static_cast<float>(atof(lpDefault));
}