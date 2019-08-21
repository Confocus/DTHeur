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
	* ��������ʽ������
	*
	* @return 
	*/
	BOOL LoadHSign();

	/**
	* ��������ֵ����ȡ����
	*
	* @param nIndex������ֵ
	* @param szSign : ���ص�����
	* @return 
	*/
	BOOL GetHSign(int nIndex, char* szSign);
	BOOL GetHSign(int nIndex, char* szSign, int* pLen);
	BOOL GetHSignAttr(int nIndex, int* pAttr);
	BOOL GetHSignLength(int nIndex);
	BOOL GetHSignUnicode(int nIndex);
	/**
	* ж������ʽ������
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
