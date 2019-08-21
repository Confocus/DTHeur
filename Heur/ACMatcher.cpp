#include "ACMatcher.h"

CACMatcher::CACMatcher(void):
m_hMod(0),
//m_pACLoadSign(NULL),
m_pACDump(NULL),
m_pACCreate(NULL),
m_pACMore(NULL),
m_pACDestroy(NULL),
m_pHeurSignParser(NULL)
{
	for(int i = 0; i < MAX_AUTOMATA_NUM; i++)
	{
		m_szAutoMata[i] = 0;
	}

	m_hMod = LoadLibrary("ACMatcher.dll");
	RETURN_IF_FAILED(m_hMod);
	/*m_pACLoadSign = (PACISM_LOAD_SIGN)GetProcAddress(m_hMod, "acism_load_sign");
	RETURN_IF_FAILED(m_pACLoadSign);*/
	m_pACDump = (PACISM_DUMP)GetProcAddress(m_hMod, "acism_dump");
	RETURN_IF_FAILED(m_pACDump);
	m_pACCreate = (PACISM_CREATE)GetProcAddress(m_hMod, "acism_create");
	RETURN_IF_FAILED(m_pACCreate);
	m_pACMore = (PACISM_MORE)GetProcAddress(m_hMod, "acism_more");
	RETURN_IF_FAILED(m_pACMore);
	m_pACDestroy = (PACISM_DESTROY)GetProcAddress(m_hMod, "acism_destroy");
	RETURN_IF_FAILED(m_pACDestroy);
}

CACMatcher::~CACMatcher(void)
{
	FreeLibrary(m_hMod);
	for(int i = 0; i < MAX_AUTOMATA_NUM; i++)
	{
		if(m_szAutoMata[i])
		{
			delete(m_szAutoMata[i]);
		}
	}
}

BOOL CACMatcher::ACLoadSign(const char *filename, MEMREF** ppRef, int* pSigNum, int nIndex)
{
	if(m_szAutoMata[nIndex])//如果已经初始化过这类特征的自动机了，就直接返回
	{
		return TRUE;
	}

	if(nIndex <= 0)
	{
		return FALSE;
	}
	
	RETURN_FALSE_IF_FAILED(filename);
	RETURN_FALSE_IF_FAILED(pSigNum);
	RETURN_FALSE_IF_FAILED(m_pHeurSignParser);

	MEM_BUF* pSigBuff = (MEM_BUF*)malloc(sizeof(MEM_BUF));
	int nLen = m_pHeurSignParser->GetHSignLength(nIndex);
	pSigBuff->ptr = (char*)malloc(nLen + 10);
	memset(pSigBuff->ptr, 0, nLen + 10);
	m_pHeurSignParser->GetHSign(nIndex, pSigBuff->ptr);
	pSigBuff->ptr[nLen - 1] = 0;
	pSigBuff->ptr++;
	if(!(*ppRef = SigSplit(pSigBuff->ptr, ';', pSigNum)))
	{
		return FALSE;
	}

	//free(SigBuff.ptr);
	////nBytes = read(fd, (*ppBuf)->ptr, (*ppBuf)->len);

	//m_pHeurSignParser->GetHSign(nIndex, (char*)(*ppRef)->ptr, (int*)&(*ppRef)->len);
	//return m_pACLoadSign(filename, ppRef, pSigNum, nIndex);
	return TRUE;
}

BOOL CACMatcher::ACDump(ACISM const* psp, PS_DUMP_TYPE pdt, FILE* out, MEMREF const* pattv)
{
	RETURN_FALSE_IF_FAILED(m_pACDump);
	return m_pACDump(psp, pdt, out, pattv);
}

ACISM* CACMatcher::ACCreate(MEMREF const *strv, int nstrs)
{
	RETURN_FALSE_IF_FAILED(m_pACCreate);
	return m_pACCreate(strv, nstrs);
}

int CACMatcher::ACMore(ACISM const* psp, MEMREF const text, ACISM_ACTION *fn, void *fndata, int *state, int* pMatchCnt)
{
	RETURN_FALSE_IF_FAILED(m_pACMore);
	RETURN_FALSE_IF_FAILED(psp);
	RETURN_FALSE_IF_FAILED(fndata);
	RETURN_FALSE_IF_FAILED(state);
	RETURN_FALSE_IF_FAILED(pMatchCnt);
	return m_pACMore(psp, text, fn, fndata, state, pMatchCnt);
}

void CACMatcher::ACDestroy(ACISM *psp)
{
	RETURN_IF_FAILED(m_pACDestroy);
	m_pACDestroy(psp);
}

//BOOL CACMatcher::IsRegistered(int nIndex)
//{
//	if(nIndex <= 0)
//	{
//		return FALSE;
//	}
//
//	return m_szAutoMata[nIndex] == 0;
//}

BOOL CACMatcher::Register(ACISM *psp, MEMREF* szSig, int nSigNum, int nIndex)
{
	RETURN_FALSE_IF_FAILED(psp);
	if(nIndex <= 0)
	{
		return FALSE;
	}

	PAC_AUTOMATA_INFO pAutoMataInfo = new AC_AUTOMATA_INFO();
	pAutoMataInfo->pAutoMata = psp;
	pAutoMataInfo->szSig = szSig;
	pAutoMataInfo->nSigNum = nSigNum;
	m_szAutoMata[nIndex] = pAutoMataInfo;
}

PAC_AUTOMATA_INFO CACMatcher::GetAutomata(int nIndex)
{
	if(nIndex <= 0)
	{
		return NULL;
	}

	return m_szAutoMata[nIndex];

}

void CACMatcher::ACDestroyAll()
{
	for(int i = 0; i < MAX_AUTOMATA_NUM; i++)
	{
		if(m_szAutoMata[i])
		{
			if(m_szAutoMata[i]->szSig && m_szAutoMata[i]->szSig->ptr)
			{
				free((void*)(--m_szAutoMata[i]->szSig->ptr));
				free((void*)m_szAutoMata[i]->szSig);
			}
			if(m_szAutoMata[i]->pAutoMata)
			{
				ACDestroy(m_szAutoMata[i]->pAutoMata);
			}
			free(m_szAutoMata[i]);
			m_szAutoMata[i] = 0;
		}
	}
}

void CACMatcher::SetHeurSignParser(CHeurSignParser* pHeurSignParser)
{
	m_pHeurSignParser = pHeurSignParser;
}

MEMREF* CACMatcher::SigSplit(char *pText, char sep, int *pCount)
{
	char	*cp = NULL;
	int		i, nStrs = 0;
	MEMREF      *strv = NULL;

	if (*pText) 
	{
		for (cp = pText, nStrs = 1; (cp = strchr(cp, sep)); ++cp) //有几个sep字符存在，说明有几个特征字符串
		{
			++nStrs;
		}

		strv = (MEMREF*)malloc(nStrs * sizeof(MEMREF));//strv指向一个MEMREF数组

		for (i = 0, cp = pText; (cp = (char*)strchr(strv[i].ptr = cp, sep)); ++i, ++cp) 
		{
			strv[i].len = cp - strv[i].ptr;
			*cp = 0;
		}

		strv[i].len = strlen(strv[i].ptr);
	}

	if (pCount) 
	{
		*pCount = nStrs;
	}

	return    strv;
}