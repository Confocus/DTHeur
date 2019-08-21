#include "MethodFuncManager.h"
#include "HeurSignParser.h"
#include "acism.h"

std::set<int> g_Set;

int on_match(int strnum, int textpos, MEMREF const *pattv)
{
	(void)strnum, (void)textpos, (void)pattv;
	printf("%9d %7s\n", textpos/* + 1024 * 1024 * nBlockCount*/, pattv[strnum].ptr/*, (int)pattv[strnum].len, pattv[strnum].ptr*/);
	g_Set.insert(strnum);
	return 0;
}

MethodFuncManager::MethodFuncManager(void)
{
}

MethodFuncManager::~MethodFuncManager(void)
{
}

BOOL MethodFuncManager::MethodAccurateMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	//printf("MethodAccurateMatch start..\n");
	RETURN_FALSE_IF_FAILED(pParam);
	RETURN_FALSE_IF_FAILED(pMatch);

	//与配置文件中的value进行匹配
	if(1 == pParam->nType)
	{
		int nValue = atoi((char*)pParam->szValue);
		if(nValue == pParam->nResult)
		{
			*pMatch = TRUE;
		}
		else
		{
			*pMatch = FALSE;
		}
		return TRUE;
	}

	if(2 == pParam->nType)
	{
		double dValue= atof((char*)pParam->szValue);
		if(fabs(dValue - pParam->dResult) < DOUBLE_DIFF_VALUE)
		{
			*pMatch = TRUE;
		}
		else
		{
			*pMatch = FALSE;
		}
		return TRUE;
	}

	return TRUE;
}

template<typename T>
BOOL MethodFuncManager::MethodParseWithinRangeExpression(T result, char* szValue, PBOOL pMatch)
{
	char* pPos = NULL;
	char* pFb = NULL;//front bracket
	char* pComma = NULL;
	char bFb = 0;
	char bRb = 0;
	char szUpper[10] = {0};
	char szLower[10] = {0};
	BOOL bAboveValue = FALSE;
	BOOL bBelowValue = FALSE;

	pPos = strrchr(szValue, '(');
	if(pPos)
	{
		bFb = '(';
	}
	else
	{
		bFb = '[';
	}

	pPos = strrchr(szValue, ')');
	if(pPos)
	{
		bRb = ')';
	}
	else
	{
		bRb = ']';
	}

	//解析范围表达式
	pFb = szValue + 1;
	pComma = strrchr(szValue, ',');
	szValue[strlen(szValue) - 1] = 0;
	*pComma = 0;
	pComma++;


	T dUpper = atof(pComma);
	T dLower = atof(pFb);

	//是否满足下界条件
	if(0 == strlen(pFb))
	{
		bAboveValue = TRUE;
	}
	else
	{
		if('(' == bFb && dLower < result)
		{	
			bAboveValue = TRUE;
		}
		if('[' == bFb && dLower <= result)
		{
			bAboveValue = TRUE;
		}
	}

	if(!bAboveValue)
	{
		*pMatch = FALSE;
		goto ret;
	}

	//是否满足上界条件
	if(0 == strlen(pComma))
	{
		bBelowValue = TRUE;
	}
	else
	{
		if(')' == bRb && result < dUpper)
		{	
			bBelowValue = TRUE;
		}
		if(']' == bRb && result <= dUpper)
		{
			bBelowValue = TRUE;
		}
	}

	if(!bBelowValue)
	{
		*pMatch = FALSE;
		goto ret;
	}

	*pMatch = TRUE;

ret:
	return TRUE;
}


BOOL MethodFuncManager::MethodWithinRange(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	//printf("MethodWithinRange start..\n");
	RETURN_FALSE_IF_FAILED(pParam);
	RETURN_FALSE_IF_FAILED(pExtSign);
	RETURN_FALSE_IF_FAILED(pMatch);
	char szValue[HR_VALUE_SIZE] = {0};
	memcpy_s(szValue, HR_VALUE_SIZE, pParam->szValue, HR_VALUE_SIZE);

	if(1 == pParam->nType)
	{
		MethodParseWithinRangeExpression(pParam->nResult, szValue, pMatch);
		return TRUE;
	}

	if(2 == pParam->nType)
	{
		MethodParseWithinRangeExpression(pParam->dResult, szValue, pMatch);
		return TRUE;
	}
}

BOOL MethodFuncManager::MethodSignDbStringMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	//printf("MethodSignDbMatch start..\n");
	RETURN_FALSE_IF_FAILED(pParam);
	RETURN_FALSE_IF_FAILED(pExtSign);
	RETURN_FALSE_IF_FAILED(pMatch);

	char szSign[HR_SIGN_SIZE] = {0};
	((CHeurSignParser*)(pParam->pHSignParser))->GetHSign(pParam->nIndex, szSign);
	if(1/*todo 特征串匹配逻辑*/)
	{
		*pMatch = TRUE;
	}
	*pMatch = FALSE;

	return TRUE;
}

BOOL MethodFuncManager::MethodSignDbAnyofMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	//printf("MethodSignDbAnyofMatch start..\n");
	RETURN_FALSE_IF_FAILED(pParam);
	RETURN_FALSE_IF_FAILED(pExtSign);
	RETURN_FALSE_IF_FAILED(pMatch);

	BOOL bResult = TRUE;
	char szSign[HR_SIGN_SIZE] = {0};
	char szSigns[HR_SIGN_SIZE] = {0};
	int nAttr = 0;
	RETURN_FALSE_IF_FAILED(((CHeurSignParser*)(pParam->pHSignParser))->GetHSignAttr(pParam->nIndex, &nAttr));
	RETURN_FALSE_IF_FAILED(((CHeurSignParser*)(pParam->pHSignParser))->GetHSign(pParam->nIndex, szSigns));//从特征库中拿到特征
	sprintf_s(szSign, ";%s;##", pParam->szResult);//多出两个字符，防止访问越界
	std::string sSigns = strlwr(szSigns);

	char* pStart = NULL;
	char* pEnd = NULL;
	pStart = szSign;
	pEnd = strchr(szSign + 1, ';');//从第二个分号开始找
	while(pEnd)
	{
		char szShortSign[HR_EACH_SIGN_SIZE] = {0};
		*pStart = 0;
		*pEnd = 0;
		sprintf_s(szShortSign, ";%s;", pStart + 1);
		std::string sShortSign = strlwr(szShortSign);
		if(-1 == sSigns.find(sShortSign))
		{
			bResult = FALSE;
			break;
		}

		pStart = pEnd;
		pEnd = strchr(pStart + 1, ';');
	}

	//此时的nValue表示特征库中保存的特征是白特征还是黑特征。value = 0表示特征库中是白特征
	if(bResult)
	{
		*pMatch = nAttr;
	}
	else
	{
		*pMatch = !nAttr;
	}

	return TRUE;
}

int MethodFuncManager::MethodGetSignNum(char* szSigns)
{
	int nSignNum = 0;
	char* pStart = szSigns;
	char* pPos = NULL;
	while((pPos = strchr(pStart + 1, ';')) && 0 != *pStart)
	{
		nSignNum++;
		pStart = pPos;
	}

	return nSignNum;
}

BOOL MethodFuncManager::MethodSignDbRatioMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	//DEBUG_PRINT("MethodSignDbRatioMatch start..\n");
	RETURN_FALSE_IF_FAILED(pParam);
	RETURN_FALSE_IF_FAILED(pExtSign);
	RETURN_FALSE_IF_FAILED(pMatch);

	int nBadSecNameNum = 0;
	double dBadSecNameRatio = 0.0;
	char szSign[HR_SIGN_SIZE] = {0};
	char szSigns[HR_SIGN_SIZE] = {0};
	int nSampleSecNum = 0;
	RETURN_FALSE_IF_FAILED(((CHeurSignParser*)(pParam->pHSignParser))->GetHSign(pParam->nIndex, szSigns));//从特征库中拿到特征
	sprintf_s(szSign, ";%s;", pParam->szResult);//多出两个字符，防止访问越界
	RETURN_FALSE_IF_FAILED((nSampleSecNum = MethodGetSignNum(szSign)));
	std::string sSigns = strlwr(szSigns);

	char* pStart = NULL;
	char* pEnd = NULL;
	pStart = szSign;
	pEnd = strchr(szSign + 1, ';');//从第二个分号开始找
	while(pEnd)
	{
		char szShortSign[HR_EACH_SIGN_SIZE] = {0};
		*pStart = 0;
		*pEnd = 0;
		sprintf_s(szShortSign, ";%s;", pStart + 1);
		std::string sShortSign = strlwr(szShortSign);
		if(-1 == sSigns.find(sShortSign))
		{
			nBadSecNameNum++;
		}

		pStart = pEnd;
		pEnd = strchr(pStart + 1, ';');
	}
	dBadSecNameRatio = (double)nBadSecNameNum/(double)nSampleSecNum;
	MethodParseWithinRangeExpression(dBadSecNameRatio, pParam->szValue, pMatch);

	return TRUE;
}

void MethodFuncManager::InitSignPath()
{
	char *pPos = NULL;
	char szPathBuf[MAX_SIZE] = {0};
	memset(m_szHSignPath, 0, MAX_SIZE);
	GetModuleFileName(NULL, szPathBuf, MAX_SIZE);
	pPos = strrchr(szPathBuf, '\\');
	*pPos = 0;
	sprintf_s(m_szHSignPath, MAX_SIZE, "%s%s", szPathBuf, "\\hs.dat");
}


BOOL MethodFuncManager::CreateUnicodeBuff(PMETHOD_PARAM pParam,MEMREF* pTextBuff)
{
	RETURN_FALSE_IF_FAILED(pParam);
	RETURN_FALSE_IF_FAILED(pTextBuff);
	char* pBuff = (char*)malloc(pParam->BufResult.len + 1);
	RETURN_FALSE_IF_FAILED(pBuff);
	memset(pBuff, 0, pParam->BufResult.len + 1);
	memcpy_s(pBuff, pParam->BufResult.len, pParam->BufResult.ptr, pParam->BufResult.len);
	for(int i = 0; i < pParam->BufResult.len; i++)
	{
		if(pBuff[i] == 0)
		{
			pBuff[i] = 255;
		}
	}
	pTextBuff->ptr = pBuff;
	pTextBuff->len = pParam->BufResult.len;

	return TRUE;
}

BOOL MethodFuncManager::FreeUnicodeBuff(MEMREF* pTextBuff)
{
	RETURN_FALSE_IF_FAILED(pTextBuff);
	RETURN_FALSE_IF_FAILED(pTextBuff->ptr);
	free((void*)pTextBuff->ptr);
	return TRUE;
}

BOOL MethodFuncManager::MethodACSearchMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	ACISM *psp = NULL;
	MEMREF *szSig = NULL;//szSig是一个SigBuff数组
	FILE* fp = NULL;
	int nSigNum = 0;//读取的特征的数量
	static char buf[1024 * 1024] = {0};
	int	state = 0;
	MEMREF TextBuff = {NULL, 0};
	LONGLONG llSize = 0;
	int nMatchCnt = 0;

	RETURN_FALSE_IF_FAILED(m_pACMatcher);
	RETURN_FALSE_IF_FAILED(pParam);
	RETURN_FALSE_IF_FAILED(pExtSign);
	RETURN_FALSE_IF_FAILED(pMatch);

	if(pParam->nIndex <= 0)
	{
		return FALSE;
	}

	g_Set.clear();

	if(!m_pACMatcher->GetAutomata(pParam->nIndex))
	{
		RETURN_FALSE_IF_FAILED(m_pACMatcher->ACLoadSign(m_szHSignPath, &szSig, &nSigNum, pParam->nIndex));
		RETURN_FALSE_IF_FAILED((psp = m_pACMatcher->ACCreate(szSig, nSigNum)));
		m_pACMatcher->Register(psp, szSig, nSigNum, pParam->nIndex);
		if(0)
		{
			m_pACMatcher->ACDump(psp, PS_ALL, stderr, szSig);
		}
	}
	else
	{
		PAC_AUTOMATA_INFO pAutoMataInfo = NULL;
		pAutoMataInfo = m_pACMatcher->GetAutomata(pParam->nIndex);
		RETURN_FALSE_IF_FAILED(pAutoMataInfo);
		psp = pAutoMataInfo->pAutoMata;
		szSig = pAutoMataInfo->szSig;
		RETURN_FALSE_IF_FAILED(psp);
	}
	
	if(((CHeurSignParser*)(pParam->pHSignParser))->GetHSignUnicode(pParam->nIndex))
	{
		CreateUnicodeBuff(pParam, &TextBuff);
	}
	else
	{
		TextBuff.ptr = pParam->BufResult.ptr;
		TextBuff.len = pParam->BufResult.len;
	}

	m_pACMatcher->ACMore(psp, TextBuff, m_pResultCb, szSig, &state, &nMatchCnt);

	if(((CHeurSignParser*)(pParam->pHSignParser))->GetHSignUnicode(pParam->nIndex))
	{
		FreeUnicodeBuff(&TextBuff);
	}
	*pMatch = (nMatchCnt != 0);

	return TRUE;
}

BOOL MethodFuncManager::MethodACSearchMatchRatio(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	ACISM *psp = NULL;
	MEMREF *szSig = NULL;//szSig是一个SigBuff数组
	int nSigNum = 0;//读取的特征的数量
	FILE* fp = NULL;
	static char buf[1024 * 1024] = {0};
	int	state = 0;
	MEMREF TextBuff = {NULL, 0};
	LONGLONG llSize = 0;
	int nMatchCnt = 0;

	RETURN_FALSE_IF_FAILED(m_pACMatcher);
	RETURN_FALSE_IF_FAILED(pParam);
	RETURN_FALSE_IF_FAILED(pExtSign);
	RETURN_FALSE_IF_FAILED(pMatch);

	if(pParam->nIndex <= 0)
	{
		return FALSE;
	}

	g_Set.clear();

	if(!m_pACMatcher->GetAutomata(pParam->nIndex))
	{
		RETURN_FALSE_IF_FAILED(m_pACMatcher->ACLoadSign(m_szHSignPath, &szSig, &nSigNum, pParam->nIndex));
		RETURN_FALSE_IF_FAILED((psp = m_pACMatcher->ACCreate(szSig, nSigNum)));
		m_pACMatcher->Register(psp, szSig, nSigNum, pParam->nIndex);//todo改为结构体指针传入
		if(0)
		{
			m_pACMatcher->ACDump(psp, PS_ALL, stderr, szSig);
		}
	}
	else
	{
		PAC_AUTOMATA_INFO pAutoMataInfo = NULL;
		pAutoMataInfo = m_pACMatcher->GetAutomata(pParam->nIndex);
		RETURN_FALSE_IF_FAILED(pAutoMataInfo);
		psp = pAutoMataInfo->pAutoMata;
		szSig = pAutoMataInfo->szSig;
		nSigNum = pAutoMataInfo->nSigNum;
		RETURN_FALSE_IF_FAILED(psp);
	}

	if(((CHeurSignParser*)(pParam->pHSignParser))->GetHSignUnicode(pParam->nIndex))
	{
		CreateUnicodeBuff(pParam, &TextBuff);
	}
	else
	{
		TextBuff.ptr = pParam->BufResult.ptr;
		TextBuff.len = pParam->BufResult.len;
	}

	m_pACMatcher->ACMore(psp, TextBuff, m_pResultCb, szSig, &state, &nMatchCnt);

	if(((CHeurSignParser*)(pParam->pHSignParser))->GetHSignUnicode(pParam->nIndex))
	{
		FreeUnicodeBuff(&TextBuff);
	}

	*pMatch = (nMatchCnt != 0);

	double dRatio = (double)g_Set.size()/(double)nSigNum;
	MethodParseWithinRangeExpression(dRatio, pParam->szValue, pMatch);
	
	return TRUE;
}

BOOL MethodFuncManager::MethodACSearchMatchNum(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	ACISM *psp = NULL;
	MEMREF *szSig = NULL;//szSig是一个SigBuff数组
	int nSigNum = 0;//读取的特征的数量
	FILE* fp = NULL;
	static char buf[1024 * 1024] = {0};
	int	state = 0;
	MEMREF TextBuff = {NULL, 0};
	LONGLONG llSize = 0;
	int nMatchCnt = 0;

	RETURN_FALSE_IF_FAILED(m_pACMatcher);
	RETURN_FALSE_IF_FAILED(pParam);
	RETURN_FALSE_IF_FAILED(pExtSign);
	RETURN_FALSE_IF_FAILED(pMatch);

	if(pParam->nIndex <= 0)
	{
		return FALSE;
	}

	g_Set.clear();

	if(!m_pACMatcher->GetAutomata(pParam->nIndex))
	{
		RETURN_FALSE_IF_FAILED(m_pACMatcher->ACLoadSign(m_szHSignPath, &szSig, &nSigNum, pParam->nIndex));
		RETURN_FALSE_IF_FAILED((psp = m_pACMatcher->ACCreate(szSig, nSigNum)));
		m_pACMatcher->Register(psp, szSig, nSigNum, pParam->nIndex);//todo改为结构体指针传入
		if(0)
		{
			m_pACMatcher->ACDump(psp, PS_ALL, stderr, szSig);
		}
	}
	else
	{
		PAC_AUTOMATA_INFO pAutoMataInfo = NULL;
		pAutoMataInfo = m_pACMatcher->GetAutomata(pParam->nIndex);
		RETURN_FALSE_IF_FAILED(pAutoMataInfo);
		psp = pAutoMataInfo->pAutoMata;
		szSig = pAutoMataInfo->szSig;
		nSigNum = pAutoMataInfo->nSigNum;
		RETURN_FALSE_IF_FAILED(psp);
	}

	if(((CHeurSignParser*)(pParam->pHSignParser))->GetHSignUnicode(pParam->nIndex))
	{
		CreateUnicodeBuff(pParam, &TextBuff);
	}
	else
	{
		TextBuff.ptr = pParam->BufResult.ptr;
		TextBuff.len = pParam->BufResult.len;
	}

	m_pACMatcher->ACMore(psp, TextBuff, m_pResultCb, szSig, &state, &nMatchCnt);

	if(((CHeurSignParser*)(pParam->pHSignParser))->GetHSignUnicode(pParam->nIndex))
	{
		FreeUnicodeBuff(&TextBuff);
	}

	MethodParseWithinRangeExpression(g_Set.size(), pParam->szValue, pMatch);
}

BOOL MethodFuncManager::MethodACSearchMatchCount(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	ACISM *psp = NULL;
	MEMREF *szSig = NULL;//szSig是一个SigBuff数组
	int nSigNum = 0;//读取的特征的数量
	FILE* fp = NULL;
	static char buf[1024 * 1024] = {0};
	int	state = 0;
	MEMREF TextBuff = {NULL, 0};
	LONGLONG llSize = 0;
	int nMatchCnt = 0;

	RETURN_FALSE_IF_FAILED(m_pACMatcher);
	RETURN_FALSE_IF_FAILED(pParam);
	RETURN_FALSE_IF_FAILED(pExtSign);
	RETURN_FALSE_IF_FAILED(pMatch);

	if(pParam->nIndex <= 0)
	{
		return FALSE;
	}

	g_Set.clear();

	if(!m_pACMatcher->GetAutomata(pParam->nIndex))
	{
		RETURN_FALSE_IF_FAILED(m_pACMatcher->ACLoadSign(m_szHSignPath, &szSig, &nSigNum, pParam->nIndex));
		RETURN_FALSE_IF_FAILED((psp = m_pACMatcher->ACCreate(szSig, nSigNum)));
		m_pACMatcher->Register(psp, szSig, nSigNum, pParam->nIndex);//todo改为结构体指针传入
		if(0)
		{
			m_pACMatcher->ACDump(psp, PS_ALL, stderr, szSig);
		}
	}
	else
	{
		PAC_AUTOMATA_INFO pAutoMataInfo = NULL;
		pAutoMataInfo = m_pACMatcher->GetAutomata(pParam->nIndex);
		RETURN_FALSE_IF_FAILED(pAutoMataInfo);
		psp = pAutoMataInfo->pAutoMata;
		szSig = pAutoMataInfo->szSig;
		nSigNum = pAutoMataInfo->nSigNum;
		RETURN_FALSE_IF_FAILED(psp);
	}

	if(((CHeurSignParser*)(pParam->pHSignParser))->GetHSignUnicode(pParam->nIndex))
	{
		CreateUnicodeBuff(pParam, &TextBuff);
	}
	else
	{
		TextBuff.ptr = pParam->BufResult.ptr;
		TextBuff.len = pParam->BufResult.len;
	}

	m_pACMatcher->ACMore(psp, TextBuff, m_pResultCb, szSig, &state, &nMatchCnt);

	if(((CHeurSignParser*)(pParam->pHSignParser))->GetHSignUnicode(pParam->nIndex))
	{
		FreeUnicodeBuff(&TextBuff);
	}

	MethodParseWithinRangeExpression(nMatchCnt, pParam->szValue, pMatch);
	return TRUE;
}

BOOL MethodFuncManager::MethodInstSearch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	return TRUE;
}

BOOL MethodFuncManager::MethodInstSearchRatio(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
{
	return TRUE;
}

BOOL MethodFuncManager::Init(CHeurSignParser* pHeurSignParser)
{
	InitSignPath();
	m_pACMatcher->SetHeurSignParser(pHeurSignParser);
	return TRUE;
}

BOOL MethodFuncManager::Release()
{
	m_pACMatcher->ACDestroyAll();
	SAFE_DELETE_POINTER(m_pACMatcher);
	return TRUE;
}

ACISM_ACTION* MethodFuncManager::m_pResultCb = (ACISM_ACTION*)on_match;//TODO 增加对回调函数注册的接口
CACMatcher* MethodFuncManager::m_pACMatcher = new CACMatcher();
char MethodFuncManager::m_szHSignPath[MAX_SIZE] = {0};

//int MethodFuncManager::m_nCount = 0;