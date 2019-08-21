#pragma once
#include "stdafx.h"
#include "HeurRuleMacro.h"
#include "HSign.h"
#include "SafeArray.h"

class CHeurSignParser
{
public:
	CHeurSignParser(void);
	~CHeurSignParser(void);

	/**
	* 加载启发式特征库
	*
	* @return 
	*/
	BOOL LoadHSign();

	/**
	* 根据索引值，获取特征
	*
	* @param nIndex：索引值
	* @param szSign : 返回的特征
	* @return 
	*/
	BOOL GetHSign(int nIndex, char* szSign);
	BOOL GetHSign(int nIndex, char* szSign, int* pLen);
	BOOL GetHSignAttr(int nIndex, int* pAttr);
	BOOL GetHSignLength(int nIndex);
	BOOL GetHSignUnicode(int nIndex);
	/**
	* 卸载启发式特征库
	*
	* @return 
	*/
	BOOL UnloadHSign();

private:
	void InitHSignDbPath();
	int ParseSignItem(char**);

private:
	CMemoryManager* m_pMemManager;
	char m_szHSignPath[MAX_SIZE];
	CSafeArray<CHSign*> *m_pHSigns;
	//CHSign* m_szCHSigns[MAX_SIGN_CNT];
};
