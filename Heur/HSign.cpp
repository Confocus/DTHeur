#include "HSign.h"

CHSign::CHSign(void):
m_nIndex(0),
m_nAttr(0),
m_nUnicode(0),
m_pMemoryManager(new CMemoryManager())//todo
{
	m_pMemoryManager->SetZero(m_szSign, MAX_SIGN_SIZE);
}

CHSign::~CHSign(void)
{
	SAFE_DELETE_POINTER(m_pMemoryManager);
}

void CHSign::GetSign(char* szSign)
{
	RETURN_IF_FAILED(szSign);
	m_pMemoryManager->SetZero(szSign, strlen(m_szSign));
	m_pMemoryManager->Copy(szSign, strlen(m_szSign), m_szSign, strlen(m_szSign));
}

void CHSign::GetSign(char* szSign, int* pLen)
{
	RETURN_IF_FAILED(szSign);
	RETURN_IF_FAILED(pLen);
	*pLen = strlen(m_szSign);
	m_pMemoryManager->Copy(szSign, strlen(m_szSign), m_szSign, strlen(m_szSign));
}

void CHSign::SetSign(char* pSign)
{
	m_pMemoryManager->Copy(m_szSign, strlen(pSign), pSign, strlen(pSign));
	if(m_nUnicode)
	{
		char szSign[HR_SIGN_SIZE] = {0};
		char* pPos = pSign + 1;//跳过第一个分号间隔符
		int i = 0;
		while(*pPos)
		{
			szSign[i] = *pPos;
			if(';' != *pPos)
			{
				szSign[++i] = 255;
			}
			i++;
			pPos++;
		}
		m_pMemoryManager->Copy(m_szSign + strlen(pSign), strlen(szSign), szSign, strlen(szSign));
	}
}

void CHSign::SetIndex(int nIndex)
{
	m_nIndex = nIndex;
}

int CHSign::GetIndex()
{
	return m_nIndex;
}

void CHSign::SetSignAttr(int nAttr)
{
	m_nAttr = nAttr;
}

void CHSign::SetSignUnicode(int nUnicode)
{
	m_nUnicode = nUnicode;
}

int CHSign::GetSignAttr()
{
	return m_nAttr;
}

int CHSign::GetSignUnicode()
{
	return m_nUnicode;
}
