#pragma once
//�C���N���[�h
#include <string>
#include "Direct3D.h"

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
static XMFLOAT3 vecUp = { 0.0f,1.0f,0.0f };
static XMFLOAT3 vecDown = { 0.0f,-1.0f,0.0f };
static XMFLOAT3 vecFront = { 0.0f,0.0f,1.0f };
static XMFLOAT3 vecBack = { 0.0f,0.0f,-1.0f };
static XMFLOAT3 vecRight = { 1.0f,0.0f,0.0f };
static XMFLOAT3 vecLeft = { -1.0f,0.0f,0.0f };
static int initializeZero = 0;
static std::string soundFolderName = "Sound/";
static std::string soundModifierName = ".wav";
static std::string modelFolderName = "Model&Picture/";
static std::string modelModifierName = ".fbx";

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