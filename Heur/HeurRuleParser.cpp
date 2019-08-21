#include "HeurRuleParser.h"
#include "HeurRuleMacro.h"
#include "HRuler.h"
#include "ExtFuncManager.h"
#include <assert.h>
#include "SafeArray.h"
#include "MethodFuncManager.h"

typedef struct HRuleItem
{
	BOOL bEnable;
	HRule_Method eMethod;
	int nValue;
	int nIndex;
	int nWeight;
}HRule_Item, *PHRule_Item;

CHeurRuleParser::CHeurRuleParser(void):
m_nHRuleCount(0),
m_nCombRuleCnt(0),
m_pHRulers(NULL),//todo��smartptr
m_pCombRules(NULL),//todo
m_pHRuleExtFuncs(NULL),//todo��smartptr
m_pHRuleMethodFuncs(NULL),//todo��smartptr
m_pHRuleCustFuncs(NULL),
m_pHSignParser(NULL),//todo��smartptr
m_pMemManager(new CMemoryManager()),//todo��smartptr
m_pRuleValueMapper(new CRuleValueMapper())//todo��smartptr
{
	//��ʼ�������·��
	InitHeurRuleDbPath();

	//��ʼ���ӹ������������ŵ�map
	InitHeurRuleNameMap();

	//��ȡ����ʽ��������
	if(!GetIniSecCount(m_szHRulePath, &m_nHRuleCount) || 0 == m_nHRuleCount)
	{
		return;
	}

	if(m_nHRuleCount > MAX_HEUR_RULE_COUNT)
	{
		printf("����Ĺ�����Ŀ����������%d\n", MAX_HEUR_RULE_COUNT);
	}
}

CHeurRuleParser::~CHeurRuleParser(void)
{
	SAFE_DELETE_POINTER(m_pMemManager);
	m_pRuleValueMapper->Uninit();
	ExtFuncManager::Release();
}

BOOL CHeurRuleParser::LoadHRule()
{
	RETURN_FALSE_IF_FAILED(InitHeurRulerArray());
	RETURN_FALSE_IF_FAILED(InitCombineRulerArray());
	RETURN_FALSE_IF_FAILED(InitCustFuncArray());
	RETURN_FALSE_IF_FAILED(InitMethodFuncArray());
	RETURN_FALSE_IF_FAILED(InitExtFuncArray());
	RETURN_FALSE_IF_FAILED(GenHRulerInstances());
	RETURN_FALSE_IF_FAILED(GetCombRuleInstances());

	return TRUE;
}

BOOL CHeurRuleParser::UnLoadHRule()
{
	for(int i = 0; i <= m_nHRuleCount; i++)//todo �Ƿ�����Ҫ�ĳ�С�ڵ��ڵĵط�
	{
		SAFE_DELETE_POINTER((*m_pHRulers)[i]);
	}

	for(int i = 0; i <= m_nCombRuleCnt; i++)//todo �Ƿ�����Ҫ�ĳ�С�ڵ��ڵĵط�
	{
		SAFE_DELETE_POINTER((*m_pCombRules)[i]);
	}

	SAFE_DELETE_POINTER(m_pHRulers);
	SAFE_DELETE_POINTER(m_pCombRules);
	SAFE_DELETE_POINTER(m_pHRuleMethodFuncs);
	SAFE_DELETE_POINTER(m_pHRuleExtFuncs);
	SAFE_DELETE_POINTER(m_pHRuleCustFuncs);

	return TRUE;
}

BOOL CHeurRuleParser::GetHRulesList(CSafeArray<CHRuler*>** ppHRule, int* pHRuleNum)
{
	RETURN_FALSE_IF_FAILED(ppHRule);
	RETURN_FALSE_IF_FAILED(pHRuleNum);
	//*ppHRule = m_pHRulers->GetBuff();
	*ppHRule = m_pHRulers;
	*pHRuleNum = m_nHRuleCount;

	return TRUE;
}

BOOL CHeurRuleParser::GetCombRulesList(CSafeArray<CCombRule*>** ppCombRule, int* pCombRuleCnt)
{
	RETURN_FALSE_IF_FAILED(ppCombRule);
	RETURN_FALSE_IF_FAILED(pCombRuleCnt);
	*ppCombRule = m_pCombRules;
	*pCombRuleCnt = m_nCombRuleCnt;

	return TRUE;
}

BOOL CHeurRuleParser::GetIniSecCount(char* szIniPath, int *pSecCnt)
{
	char szSectionBuff[MAX_SIZE] = {0};
	char* pPos = NULL;
	pPos = szSectionBuff;
	GetPrivateProfileSectionNames(szSectionBuff, MAX_SIZE, szIniPath);

	*pSecCnt = 0;
	while(*pPos != 0)
	{
		(*pSecCnt)++;
		pPos = strchr(pPos, 0) + 1;
	}

	return TRUE;
}

BOOL CHeurRuleParser::InitHeurRuleNameMap()
{
	m_pRuleValueMapper->Init();
	return TRUE;
}

int CHeurRuleParser::GetHeurRuleCount()
{
	return m_nHRuleCount;
}

int CHeurRuleParser::InitCustFuncMap()
{
	return TRUE;
}

void CHeurRuleParser::InitHeurRuleDbPath()
{
	char *pPos = NULL;
	char szPathBuf[MAX_SIZE] = {0};
	memset(m_szHRulePath, 0, MAX_SIZE);
	GetModuleFileName(NULL, szPathBuf, MAX_SIZE);
	pPos = strrchr(szPathBuf, '\\');
	*pPos = 0;
	sprintf_s(m_szHRulePath, MAX_SIZE, "%s%s", szPathBuf, "\\hr.ini");
}

BOOL CHeurRuleParser::SetRelatedHeurSignDb(CHeurSignParser* pHSignParser)
{
	RETURN_FALSE_IF_FAILED(pHSignParser);
	m_pHSignParser = pHSignParser;
	return TRUE;
}

BOOL CHeurRuleParser::InitHeurRulerArray()
{
	RETURN_FALSE_IF_FAILED((m_pHRulers = new CSafeArray<CHRuler*>(m_nHRuleCount + 1)));

	return TRUE;
}


BOOL CHeurRuleParser::InitCombineRulerArray()
{
	RETURN_FALSE_IF_FAILED((m_pCombRules = new CSafeArray<CCombRule*>(MAX_COMB_RULE_CNT)));

	return TRUE;
}

BOOL CHeurRuleParser::InitMethodFuncArray()
{
	RETURN_FALSE_IF_FAILED(m_pHRuleCustFuncs);//����Ҫȷ��CustFuncs�ȱ���ʼ��
	RETURN_FALSE_IF_FAILED(m_pHRuleCustFuncs->GetBuff());//����Ҫȷ��CustFuncs�ȱ���ʼ��
	m_pHRuleMethodFuncs = new CSafeArray<PMethodCbFunc>(MAX_HR_METHOD_COUNT + 1);
	
	(*m_pHRuleMethodFuncs)[METHOD_ACCURATE_MATCH] = MethodFuncManager::MethodAccurateMatch;
	(*m_pHRuleMethodFuncs)[METHOD_WITHIN_RANGE] = MethodFuncManager::MethodWithinRange;
	(*m_pHRuleMethodFuncs)[METHOD_STRSEQ_MATCH] = MethodFuncManager::MethodSignDbStringMatch;
	(*m_pHRuleMethodFuncs)[METHOD_ANYOF] = MethodFuncManager::MethodSignDbAnyofMatch;
	(*m_pHRuleMethodFuncs)[METHOD_SIGNDB_RATIO] = MethodFuncManager::MethodSignDbRatioMatch;
	(*m_pHRuleMethodFuncs)[METHOD_AC_SEARCH] = MethodFuncManager::MethodACSearchMatch;
	(*m_pHRuleMethodFuncs)[METHOD_AC_SEARCH_RATIO] = MethodFuncManager::MethodACSearchMatchRatio;
	(*m_pHRuleMethodFuncs)[METHOD_AC_SEARCH_NUM] = MethodFuncManager::MethodACSearchMatchNum;
	(*m_pHRuleMethodFuncs)[METHOD_AC_SEARCH_CNT] = MethodFuncManager::MethodACSearchMatchCount;

	(*m_pHRuleMethodFuncs)[METHOD_CUST] = (PMethodCbFunc)m_pHRuleCustFuncs->GetBuff();//m_szHRuleMethodFuncs���һ����Ա����Customize�������б�ָ��

	return TRUE;
}

BOOL CHeurRuleParser::InitExtFuncArray()
{
	//�������������������ţ����Բ��ܽ���m_nHRuleCount������ռ�
	m_pHRuleExtFuncs = new CSafeArray<PExtSignCbFunc>(MAX_HEUR_RULE_COUNT + 1);

	(*m_pHRuleExtFuncs)[HR_CODESECATTR] = ExtFuncManager::IsCodeSecAttrAbn;
	(*m_pHRuleExtFuncs)[HR_ENTRYATTR] = ExtFuncManager::IsEntrySecAttrAbn;
	(*m_pHRuleExtFuncs)[HR_ENTRYLASTSEC] = ExtFuncManager::IsEntryPointLastSection;
	(*m_pHRuleExtFuncs)[HR_ENTRYOUT] = ExtFuncManager::IsEntryPointOutOfSection;
	(*m_pHRuleExtFuncs)[HR_ENTRYNAME] = ExtFuncManager::GetEntryPointSectionName;
	(*m_pHRuleExtFuncs)[HR_BADSECNAME] = ExtFuncManager::GetSectionNames;
	(*m_pHRuleExtFuncs)[HR_IMPORTFUNCNUM] = ExtFuncManager::GetImpFuncNum;
	(*m_pHRuleExtFuncs)[HR_OVEROPHEADSIZE] = ExtFuncManager::GetOptionalHeadSize;
	(*m_pHRuleExtFuncs)[HR_TLSSEC] = ExtFuncManager::IsTlsFuncNonNull;
	(*m_pHRuleExtFuncs)[HR_SIZEOFCODEERR] = ExtFuncManager::IsErrorSizeOfCode;
	(*m_pHRuleExtFuncs)[HR_MULEXESEC] = ExtFuncManager::IsMultiExeSec;
	(*m_pHRuleExtFuncs)[HR_CHECKSUM_ERROR] = ExtFuncManager::IsCheckSumError;
	(*m_pHRuleExtFuncs)[HR_ZERO_RAWSIZE_SEC_NUM] = ExtFuncManager::GetZeroRawSizeSecNum;
	(*m_pHRuleExtFuncs)[HR_SUM_SEC_SIZE] = ExtFuncManager::IsSumSecSizeError;
	(*m_pHRuleExtFuncs)[HR_IMP_CORRUPT] = ExtFuncManager::IsImpCorrupt;
	(*m_pHRuleExtFuncs)[HR_SECRAWADDR_ALIGN_ERR] = ExtFuncManager::IsSecRawAddrAlignErr;
	(*m_pHRuleExtFuncs)[HR_RSRC_EXE] = ExtFuncManager::IsRsrcExe;
	(*m_pHRuleExtFuncs)[HR_POINT_SYM_TAB_ERR] = ExtFuncManager::GetPointSymTab;
	(*m_pHRuleExtFuncs)[HR_SYM_NUM_ERR] = ExtFuncManager::GetSymNum;
	(*m_pHRuleExtFuncs)[HR_IS_PACKED] = ExtFuncManager::IsPacked;
	(*m_pHRuleExtFuncs)[HR_ENTTRY_ENTROPY] = ExtFuncManager::GetEntryEntropy;
	(*m_pHRuleExtFuncs)[HR_FILE_ENTROPY] = ExtFuncManager::GetFileEntropy;
	(*m_pHRuleExtFuncs)[HR_HIGHEST_RSRC_ENTROPY] = ExtFuncManager::GetRsrcEntropy;
	(*m_pHRuleExtFuncs)[HR_RAW_SIZE_ABN] = ExtFuncManager::IsRawSizeAbn;
	(*m_pHRuleExtFuncs)[HR_RATIO_HIGH_ENTROPY_BLOCK] = ExtFuncManager::GetHighEntropyBlockRatio;
	(*m_pHRuleExtFuncs)[HR_MULITI_HIGH_ENTROPY_SEC] = ExtFuncManager::GetHighEntropySectionRatio;
	(*m_pHRuleExtFuncs)[HR_SEC_CNT] = ExtFuncManager::GetSecCnt;
	(*m_pHRuleExtFuncs)[HR_EXE_SEC_RATIO] = ExtFuncManager::GetExeSecRatio;
	(*m_pHRuleExtFuncs)[HR_TIME_STAMP_ERR] = ExtFuncManager::GetTimeStamp;
	(*m_pHRuleExtFuncs)[HR_ALL_UNEXE_SEC] = ExtFuncManager::IsAllUnexeSec;
	(*m_pHRuleExtFuncs)[HR_ZERO_DATADIR_NUM] = ExtFuncManager::GetZeroDataDirNum;
	(*m_pHRuleExtFuncs)[HR_INVALID_DATADIR_NUM] = ExtFuncManager::GetInvalidDataDirNum;
	(*m_pHRuleExtFuncs)[HR_BAD_SEC_RATIO] = ExtFuncManager::GetSectionNames;
	(*m_pHRuleExtFuncs)[HR_BYTE_LAST_PAGE] = ExtFuncManager::GetByteLastPage;
	(*m_pHRuleExtFuncs)[HR_PAGE_IN_FILE] = ExtFuncManager::GetPageInFile;
	(*m_pHRuleExtFuncs)[HR_IAT_NUM] = ExtFuncManager::GetIATNum;
	(*m_pHRuleExtFuncs)[HR_ENTRY_RELOC] = ExtFuncManager::IsEntryReloc;
	(*m_pHRuleExtFuncs)[HR_ENTRY_RSRC] = ExtFuncManager::IsEntryRsrc;
	(*m_pHRuleExtFuncs)[HR_LAST_SEC_NAME] = ExtFuncManager::GetLastSecName;
	(*m_pHRuleExtFuncs)[HR_ONLY_RSRC_VS] = ExtFuncManager::IsOnlyRsrcVirtualSize;
	(*m_pHRuleExtFuncs)[HR_REPEATED_SEC_NAME] = ExtFuncManager::HasRepeatedSecName;
	(*m_pHRuleExtFuncs)[HR_UNPRINT_SEC_NAME] = ExtFuncManager::GetUnprintableSecNameNum;
	(*m_pHRuleExtFuncs)[HR_WRITABLE_RDATA] = ExtFuncManager::IsRdataWritable;
	(*m_pHRuleExtFuncs)[HR_EXE_RDATA] = ExtFuncManager::IsRdataExe;
	(*m_pHRuleExtFuncs)[HR_ANTI_AV] = ExtFuncManager::IsAntiAV;
	(*m_pHRuleExtFuncs)[HR_CLIPBOARD_ACT] = ExtFuncManager::IsClipboardAct;
	(*m_pHRuleExtFuncs)[HR_SERVICE_ACT] = ExtFuncManager::IsServiceAct;
	(*m_pHRuleExtFuncs)[HR_HOOK_ACT] = ExtFuncManager::IsHookAct;
	(*m_pHRuleExtFuncs)[HR_REVERSE_VOLUME_ACT] = ExtFuncManager::IsReverseVolumeAct;
	(*m_pHRuleExtFuncs)[HR_ANTIDEBUG] = ExtFuncManager::IsAntiDebug;
	(*m_pHRuleExtFuncs)[HR_VIRTUAL_MEM_OP] = ExtFuncManager::IsVirtualMemOp;
	(*m_pHRuleExtFuncs)[HR_PRIVILEDGE_OP] = ExtFuncManager::IsPriviledgeOp;
	(*m_pHRuleExtFuncs)[HR_INTERNET_OP] = ExtFuncManager::IsInternetOp;
	(*m_pHRuleExtFuncs)[HR_SCAN_BUFF] = ExtFuncManager::ScanBuff;
	(*m_pHRuleExtFuncs)[HR_NET_ENUM] = ExtFuncManager::ScanBuff;
	(*m_pHRuleExtFuncs)[HR_QQ_SPY] = ExtFuncManager::ScanBuff;
	(*m_pHRuleExtFuncs)[HR_MIDI_OP] = ExtFuncManager::ScanBuff;
	(*m_pHRuleExtFuncs)[HR_WAVE_OP] = ExtFuncManager::ScanBuff;
	(*m_pHRuleExtFuncs)[HR_EXE_IN_EXE] = ExtFuncManager::ScanBuff;
	(*m_pHRuleExtFuncs)[HR_TOTAL_SCAN] = ExtFuncManager::ScanBuff;
	(*m_pHRuleExtFuncs)[HR_REMOTE_ACCOUNT] = ExtFuncManager::ScanBuff;
	(*m_pHRuleExtFuncs)[HR_FILESIZE_PESIZE_ABN] = ExtFuncManager::IsFileSizePESizeAbn;
	
	return TRUE;
}

BOOL CHeurRuleParser::InitCustFuncArray()
{
	//ƥ�䷽�������ǹ̶��ģ�һ�㲻�ᶯ̬���ӣ�������Щ�ռ���Ҫ�������ϵģ���������ʹ��Ԥ���趨�õ�ֵMAX_HR_CUST_FUNC_COUNT�������Ƕ�̬��ȡ����
	//RETURN_FALSE_IF_FAILED((m_szHRuleCustFuncs = (PMethodCbFunc*)m_pMemManager->Malloc((MAX_HR_CUST_FUNC_COUNT + 1) * sizeof(PMethodCbFunc))));
	m_pHRuleCustFuncs = new CSafeArray<PMethodCbFunc>(MAX_HR_CUST_FUNC_COUNT + 1);
	//RETURN_FALSE_IF_FAILED((m_szHRuleCustFuncs = (PMethodCbFunc*)m_pMemManager->Malloc<PMethodCbFunc>(MAX_HR_CUST_FUNC_COUNT + 1)));

	for(int i = 1; i <= MAX_HR_CUST_FUNC_COUNT; i++)
	{
		(*m_pHRuleCustFuncs)[i] = NULL;
	}

	return TRUE;
}

BOOL CHeurRuleParser::GenHRulerInstances()
{
	int i = 1;
	char* pPos = NULL;
	char szHRuleName[MAX_SIZE] = {0};
	pPos = szHRuleName;
	GetPrivateProfileSectionNames(szHRuleName, MAX_SIZE, m_szHRulePath);

	while(*pPos != 0)
	{
		//1������ini�ļ�
		CHRuler* pHRuler = NULL;
		char szValue[HR_VALUE_SIZE] = {0};
		int nEnable = GetPrivateProfileInt((char*)pPos, "enable", 1, m_szHRulePath);
		if(0 == nEnable)
		{
			pPos = strchr(pPos, 0) + 1;
			continue;
		}

		int nMethod = GetPrivateProfileInt((char*)pPos, "method", 0, m_szHRulePath);
		if(nMethod <= METHOD_NONE || nMethod > MAX_HR_METHOD_COUNT)
		{
			DEBUG_PRINT("��ȡMethodֵʧ��\n");
			pPos = strchr(pPos, 0) + 1;
			continue;
		}
		int nIndex = GetPrivateProfileInt((char*)pPos, "index", 0, m_szHRulePath);
		if(nIndex < 0 || nIndex > MAX_INDEX_VALUE)
		{
			DEBUG_PRINT("��ȡIndexֵʧ��\n");
			pPos = strchr(pPos, 0) + 1;
			continue;
		}
		int nWeight = GetPrivateProfileInt((char*)pPos, "weight", 0, m_szHRulePath);
		GetPrivateProfileString((char*)pPos, "value", NULL, szValue, sizeof(szValue), m_szHRulePath);
		strlwr(pPos);

		//2������һ��HRuler
		pHRuler = new CHRuler();
		if(!pHRuler)
		{
			DEBUG_PRINT("����CHRuler����ʧ��\n");
			pPos = strchr(pPos, 0) + 1;
			continue;
		}
		pHRuler->SetHRuleIndex(nIndex);
		pHRuler->SetHRuleMethod((HRule_Method)nMethod);
		pHRuler->SetHRuleWeight(nWeight);
		pHRuler->SetHRuleValue(szValue);
		pHRuler->SetHRuleName(pPos);
		pHRuler->SetHSignParser(m_pHSignParser);

		if(INVALID_RETURN_VALUE != m_pRuleValueMapper->find(pPos))
		{
			pHRuler->SetHRuleNameNum(m_pRuleValueMapper->find(pPos));
		}
		pHRuler->SetExtSignFunc((*m_pHRuleExtFuncs)[pHRuler->GetHRuleNameNum()]);//����������ȡ����
		pHRuler->SetMethodFunc((*m_pHRuleMethodFuncs)[pHRuler->GetHRuleMethodNum()]);//����ƥ�䷽������

		//3���洢���HRuler�����ù����б�
		(*m_pHRulers)[i++] = pHRuler;
		pPos = strchr(pPos, 0) + 1;
	}

	return TRUE;
}

BOOL CHeurRuleParser::GetCombRuleInstances()
{
	int i = 0;
	char *pPos = NULL;
	char szCombRulePath[MAX_SIZE] = {0};
	char szPathBuf[MAX_SIZE] = {0};

	GetModuleFileName(NULL, szPathBuf, MAX_SIZE);
	pPos = strrchr(szPathBuf, '\\');
	RETURN_FALSE_IF_FAILED(pPos);
	*pPos = 0;
	sprintf_s(szCombRulePath, MAX_SIZE, "%s%s", szPathBuf, "\\cr.dat");

	FILE* fp = NULL;
	int nErr = fopen_s(&fp, szCombRulePath, "r");
	RETURN_FALSE_IF_FAILED(fp);

	while(!feof(fp))
	{
		m_nCombRuleCnt++;
		CCombRule *pCombRule = new CCombRule();
		RETURN_FALSE_IF_FAILED(pCombRule);
		char szRuleContent[MAX_COMB_RULE_SIZE] = {0};
		fgets(szRuleContent, MAX_COMB_RULE_SIZE, fp); // �����˻��з�

		//��������������
		char* pPos = strchr(szRuleContent, '|');
		if(!pPos)
		{
			continue;
		}
		*pPos = 0;
		int nIndex = atoi(szRuleContent);
		pCombRule->SetIndex(nIndex);

		//���������ĺڰ�����
		char *pPos2 = pPos + 1;
		pPos = strchr(pPos2, '|');
		if(pPos)//������������������ԣ���or��
		{
			*pPos = 0;
			int nWeight = atoi(pPos2);
			pCombRule->SetWeight(nWeight);
		}
		
		char* pStart = NULL;
		char* pEnd = NULL;
		pStart = pPos + 1;
		strlwr(pStart);
		pEnd = strchr(pStart + 1, ';');//�ӵڶ����ֺſ�ʼ��
		while(pEnd)
		{
			char szSignleRule[HR_EACH_SIGN_SIZE] = {0};
			*pStart = 0;
			*pEnd = 0;
			sprintf_s(szSignleRule, "%s", pStart + 1);
			pCombRule->SetCombRule(m_pRuleValueMapper->find(szSignleRule));
			pStart = pEnd;
			pEnd = strchr(pStart + 1, ';');
		}
		(*m_pCombRules)[i++] = pCombRule;
	}

	fclose(fp);

	return TRUE;
}