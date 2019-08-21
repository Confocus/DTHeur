#include "HeurSignParser.h"

CHeurSignParser::CHeurSignParser(void):
m_pHSigns(NULL),
m_pMemManager(new CMemoryManager())//todo
{
	InitHSignDbPath();
	m_pHSigns = new CSafeArray<CHSign*>(MAX_SIGN_CNT);
}

CHeurSignParser::~CHeurSignParser(void)
{
	SAFE_DELETE_POINTER(m_pMemManager);
}

BOOL CHeurSignParser::LoadHSign()
{
	FILE* fp = NULL;
	int nErr = fopen_s(&fp, m_szHSignPath, "rb");
	RETURN_FALSE_IF_FAILED(fp);

	while(!feof(fp))
	{
		CHSign *pHSign = new CHSign();
		RETURN_FALSE_IF_FAILED(pHSign);
		char szSign[MAX_SIGN_SIZE] = {0};
		char* pSign = szSign;
		int nIndex = 0;
		fgets(szSign, MAX_SIGN_SIZE, fp); // 包含了换行符
		if(10 == szSign[strlen(szSign) - 1] && 13 == szSign[strlen(szSign) - 2])
		{
			szSign[strlen(szSign) - 1] = 0;
			szSign[strlen(szSign) - 1] = 0;
		}

		int nRes = ParseSignItem(&pSign);
		if(-1 == nRes)
		{
			delete pSign;
			continue;
		}
		nIndex = nRes;
		pHSign->SetIndex(nRes);

		nRes = ParseSignItem(&pSign);
		if(-1 == nRes)
		{
			delete pSign;
			continue;
		}
		pHSign->SetSignAttr(nRes);

		nRes = ParseSignItem(&pSign);
		if(-1 == nRes)
		{
			delete pSign;
			continue;
		}
		pHSign->SetSignUnicode(nRes);
	
		//配置特征的内容
		pHSign->SetSign(pSign);
		(*m_pHSigns)[nIndex] = pHSign;
	}

	fclose(fp);

	return TRUE;
}

int CHeurSignParser::ParseSignItem(char** ppSign)
{
	int ret = -1;
	char* pPos = strchr(*ppSign, '|');
	if(!pPos)
	{
		return -1;
	}
	*pPos = 0;
	ret = atoi(*ppSign);
	(*ppSign) = pPos + 1;

	return ret;
}

BOOL CHeurSignParser::GetHSign(int nIndex, char* szSign)
{
	RETURN_FALSE_IF_FAILED((*m_pHSigns)[nIndex]);
	((*m_pHSigns)[nIndex])->GetSign(szSign);
	strlwr(szSign);

	return TRUE;
}


int CHeurSignParser::GetHSignLength(int nIndex)
{
	char szSign[HR_SIGN_SIZE] = {0};
	GetHSign(nIndex, szSign);
	return strlen(szSign);
}


BOOL CHeurSignParser::GetHSign(int nIndex, char* szSign, int* pLen)
{
	RETURN_FALSE_IF_FAILED((*m_pHSigns)[nIndex]);
	((*m_pHSigns)[nIndex])->GetSign(szSign, pLen);
	strlwr(szSign);

	return TRUE;
}

BOOL CHeurSignParser::GetHSignAttr(int nIndex, int* pAttr)
{
	RETURN_FALSE_IF_FAILED((*m_pHSigns)[nIndex]);
	//m_szCHSigns[nIndex]->GetSign(szSign);
	*pAttr = ((*m_pHSigns)[nIndex])->GetSignAttr();

	return TRUE;
}

BOOL CHeurSignParser::UnloadHSign()
{
	for(int i = 0; i < MAX_SIGN_CNT - 1; i++)
	{
		if((*m_pHSigns)[i])
		{
			SAFE_DELETE_POINTER((*m_pHSigns)[i]);	
		}
	}

	SAFE_DELETE_POINTER(m_pHSigns);

	return TRUE;
}

void CHeurSignParser::InitHSignDbPath()
{
	char *pPos = NULL;
	//获取规则库路径
	char szPathBuf[MAX_SIZE] = {0};
	memset(m_szHSignPath, 0, MAX_SIZE);
	GetModuleFileName(NULL, szPathBuf, MAX_SIZE);
	pPos = strrchr(szPathBuf, '\\');
	*pPos = 0;
	sprintf_s(m_szHSignPath, MAX_SIZE, "%s%s", szPathBuf, "\\hs.dat");
}

BOOL CHeurSignParser::GetHSignUnicode(int nIndex)
{
	if(nIndex < 0)
	{
		return FALSE;
	}
	
	return (*m_pHSigns)[nIndex]->GetSignUnicode();
}
