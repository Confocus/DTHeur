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
	* �����������������ԣ��Ǻ��������ǰ�����
	*
	* @param nAttr : ��������������
	* @return 
	*/
	void SetSignAttr(int nAttr);

	void SetSignUnicode(int);

	int GetSignUnicode();

	/**
	* ��ȡ�������������ԣ��Ǻ��������ǰ�����
	*
	* @return ��������������
	*/
	int GetSignAttr();

	int GetIndex();

private:
	int m_nAttr;//0��ʾ��������1��ʾ������
	int m_nIndex;
	int m_nUnicode;
	CMemoryManager* m_pMemoryManager;
	char m_szSign[MAX_SIGN_SIZE];
};
