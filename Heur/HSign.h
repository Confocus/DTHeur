#pragma once
#include "HeurRuleMacro.h"

class CHSign
{
public:
	CHSign(void);
	~CHSign(void);

	void SetSign(char* pSign);
	void GetSign(char* szSign);
	void GetSign(char* szSign, int* pLen);
	void SetIndex(int nIndex);

	/**
	* 设置这条特征的属性，是黑特征还是白特征
	*
	* @param nAttr : 这条特征的属性
	* @return 
	*/
	void SetSignAttr(int nAttr);

	void SetSignUnicode(int);

	int GetSignUnicode();

	/**
	* 获取这条特征的属性，是黑特征还是白特征
	*
	* @return 这条特征的属性
	*/
	int GetSignAttr();

	int GetIndex();

private:
	int m_nAttr;//0表示白特征，1表示黑特征
	int m_nIndex;
	int m_nUnicode;
	CMemoryManager* m_pMemoryManager;
	char m_szSign[MAX_SIGN_SIZE];
};
