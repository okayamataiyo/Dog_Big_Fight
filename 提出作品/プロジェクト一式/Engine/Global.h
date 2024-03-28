#pragma once
#include "Direct3D.h"

//安全にメモリを開放するためのマクロ
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

//0に初期化
#define ZERO_INITIALIZE(p) { p = 0;}

#define ZERO_XMFLOAT3_INITIALIZE(p){p = {0,0,0};}

//1に初期化
#define ONE_INITIALIZE(p) { p = 1;}

//引数で初期化
#define ARGUMENT_INITIALIZE(p,t){ p = t;}

// INIファイルからfloatを取得する関数
static float GetPrivateProfileFloat(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFileName)
{
	// ファイル名     
	TCHAR buffer[MAX_PATH];
	// 文字列を取得     
	DWORD result = GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, buffer, sizeof(buffer), lpFileName);
	// 文字列が取得できたらfloatに変換して値を返す     
	if (result != 0) return static_cast<float>(atof(buffer));
	else return static_cast<float>(atof(lpDefault));
}