#include "HeurScanner.h"
#include "HRuler.h"
#include "HeurRuleMacro.h"
#include "PEParser.h"
#include "ExtFuncManager.h"
#include "Result.h"
#include "MethodFuncManager.h"

extern ULONGLONG dwTotalRiskValue;
extern int g_nHitCombRule;

CHeurScanner::CHeurScanner(void)://����ָ��ĳ�ʼ��
m_pHRuleParser(new CHeurRuleParser()),//todo �ĳ�����ָ��
m_pHSignParser(new CHeurSignParser()),//todo �ĳ�����ָ��
m_pMemoryManager(new CMemoryManager()),//todo �ĳ�����ָ��
m_pScanResultManager(new CScanResultManager()),//todo �ĳ�����ָ��
m_pResultCbFunc(NULL),
m_pScanHRRulers(NULL),
m_nRiskValue(0),
m_nHRuleCount(0),
m_nCombRuleCnt(0),
m_nScanCount(0)
{
	m_pMemoryManager->SetZero(m_szScannedObj, MAX_SIZE);
}

CHeurScanner::~CHeurScanner(void)
{
	m_pScanHRRulers = NULL;
	SAFE_DELETE_POINTER(m_pHRuleParser);
	SAFE_DELETE_POINTER(m_pHSignParser);
	SAFE_DELETE_POINTER(m_pMemoryManager);
	SAFE_DELETE_POINTER(m_pScanResultManager);
}

BOOL CHeurScanner::Init()
{
	RETURN_FALSE_IF_FAILED(AssociateHRuleAndHSign(m_pHRuleParser, m_pHSignParser));//����Ҫ�ȹ������ݿ��������⣬Ȼ����Load���ݿ���������
	RETURN_FALSE_IF_FAILED(m_pHRuleParser->LoadHRule());
	RETURN_FALSE_IF_FAILED(m_pHSignParser->LoadHSign());
	RETURN_FALSE_IF_FAILED(m_pHRuleParser->GetHRulesList(&m_pScanHRRulers, &m_nHRuleCount));//�õ�HRuler�б�
	RETURN_FALSE_IF_FAILED(m_pHRuleParser->GetCombRulesList(&m_pCombRulers, &m_nCombRuleCnt));//�õ�HRuler�б�
	MethodFuncManager::Init(m_pHSignParser);

	return TRUE;
}

BOOL CHeurScanner::ScanPE()
{
	m_nRiskValue = 0;//ÿɨ��һ��PE֮ǰ����һ��
	int szHitRuleFlag[HIT_RULE_FLAG_SIZE] = {0};//��������Щ����������

	//todo ���Խ׶Σ���ʱ�������������Ľ��������ֻ��Ҫ�����������Ľ������
	//todo ����һ��Match�ṹ�壬ÿ���������ն�Ӧһ��Match
	std::vector<char*> vecHRName;

	//todo��
	//��Paresr���õ���ɨ������б�
	//��PEParser�������CPeObj
	//����Match����ʵ�����ڵ���Matchǰ�ٶ�һ��ѭ��
	//���磺while(!PEParser.GetObj)

	//������ÿ��HRuler��ȡ������ƥ��
	for(int i = 0; i <= m_nHRuleCount; i++)
	{	
		if((*m_pScanHRRulers)[i] != NULL)
		{	
			BOOL bMatch = FALSE;
			if(!((*m_pScanHRRulers)[i])->Match(&bMatch))//���ȡ����Ҫ��ɨ����������
			{
				continue;
			}

			//�������ֵ
			m_nRiskValue += bMatch * (*m_pScanHRRulers)[i]->GetHRuleWeight();
			if(bMatch)
			{
				//������������������
				vecHRName.push_back(((*m_pScanHRRulers)[i])->GetHRuleName());
				SetHitRuleFlag(szHitRuleFlag, ((*m_pScanHRRulers)[i])->GetHRuleNameNum());
			}
		}
	}

	CalcLollapalooza(szHitRuleFlag, &m_nRiskValue);
	dwTotalRiskValue += m_nRiskValue;

	if(m_nRiskValue >= 0)//todo ����������Ը�Ϊ�����õģ�������ʱΪ0����ʾ�Ƿ�Ϊ�ڰ׶�����
	{
		CResult *pResult = new CResult();
		RETURN_FALSE_IF_FAILED(pResult);
		pResult->SaveHRName(&vecHRName);
		pResult->SetScannedObj(m_szScannedObj);
		pResult->SetRiskValue(m_nRiskValue);
		//�����е�CResult��ͳһ��ResultManager�й���
		m_pScanResultManager->SaveResult(pResult);//����UnLoad���ͷ�
		m_pScanResultManager->SetScanCount(m_nScanCount);
	}

	//todo ɨ����֮�����µ���SetObj����ȡCPeObj�б��е���һ��������ɨ��
	m_nScanCount++;
	return TRUE;
}

BOOL CHeurScanner::ScanPE(char* szPath)
{
	RETURN_FALSE_IF_FAILED(SetObj(szPath));
	BOOL bRet = ScanPE();
	ReleaseObj();

	return bRet;
}

BOOL CHeurScanner::Scan(char* szPath)
{
	DWORD dwAttr = GetFileAttributes(szPath);
	if(-1 == dwAttr)
	{
		DEBUG_PRINT("GetFileAttributes Error:%d\n", GetLastError());
		return FALSE;
	}

	if(dwAttr & FILE_ATTRIBUTE_DIRECTORY)
	{
		char szForamteScanPath[MAX_SIZE] = {0};
		if(szPath[strlen(szPath) - 1] != '\\')//��ʽ��·��
		{
			sprintf_s(szForamteScanPath, "%s\\", szPath);
			ScanDir(szForamteScanPath);
		}
		else
		{
			ScanDir(szPath);
		}

		return TRUE;
	}
	
	if(ScanPE(szPath))
	{
		return TRUE;
	}

	return TRUE;
}

BOOL CHeurScanner::UnInit()
{
	DEBUG_PRINT("UnInit..\n");
	m_pHRuleParser->UnLoadHRule();
	m_pHSignParser->UnloadHSign();
	m_pScanResultManager->Release();
	MethodFuncManager::Release();

	return TRUE;
}

void CHeurScanner::SetResultCbFunc(PResultCbFunc pResultCbFunc /*= NULL*/)
{
	m_pResultCbFunc = pResultCbFunc;
}

BOOL CHeurScanner::AssociateHRuleAndHSign(CHeurRuleParser* pHRuleParser, CHeurSignParser* pHSignParser)
{
	RETURN_FALSE_IF_FAILED(pHRuleParser);
	RETURN_FALSE_IF_FAILED(pHSignParser);
	RETURN_FALSE_IF_FAILED(pHRuleParser->SetRelatedHeurSignDb(pHSignParser));

	return TRUE;
}

BOOL CHeurScanner::SetObj(char* szPath)
{
	DEBUG_PRINT("Scanning %s\n", szPath);
	//·���б���������PEParser�а�
	//todo  ���init�ӿ�
	m_pMemoryManager->SetZero(m_szScannedObj, MAX_SIZE);
	m_pMemoryManager->Copy(m_szScannedObj, MAX_SIZE, szPath, MAX_SIZE);
	RETURN_FALSE_IF_FAILED(ExtFuncManager::SetObj(szPath));

	return TRUE;
}

BOOL CHeurScanner::Estimate()
{
	DEBUG_PRINT("Estimate start..\n");
	RETURN_FALSE_IF_FAILED(m_pResultCbFunc);
	RETURN_FALSE_IF_FAILED(m_pResultCbFunc(m_pScanResultManager));//���ûص���������ƥ��Ľ��

	return TRUE;
}

BOOL CHeurScanner::ScanDir(char* szPath)
{
	char szScanPath[MAX_SIZE] = {0};
	WIN32_FIND_DATA wfd = {0};
	sprintf_s(szScanPath, "%s*.*", szPath);

	HANDLE hFind = FindFirstFile(szScanPath, &wfd);
	if ( INVALID_HANDLE_VALUE == hFind )
	{
		return FALSE;
	}

	do 
	{
		if (strcmp(wfd.cFileName, ".") == 0 || strcmp(wfd.cFileName, "..") == 0)
		{
			continue;
		}
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)    // �Ƿ���Ŀ¼ 
		{
			char szScanDir[MAX_SIZE] = {0};
			sprintf_s(szScanDir, "%s%s\\", szPath, wfd.cFileName);
			ScanDir(szScanDir);
			continue;
		}
		else
		{
			char szScanPE[MAX_SIZE] = {0};
			sprintf_s(szScanPE, "%s%s", szPath, wfd.cFileName);

			if(ScanPE(szScanPE))
			{
				continue;
			}
		}
	} while (FindNextFile(hFind, &wfd));

	FindClose(hFind);

	return TRUE;
}

BOOL CHeurScanner::SetHitRuleFlag(int* szHitRuleFlag, int nRuleIndex)
{
	if(nRuleIndex >= HR_END)
	{
		return FALSE;
	}
	int nIndex = nRuleIndex / (sizeof(int) * 8);
	int nBitIndex = nRuleIndex % (sizeof(int) * 8);
	int nFlag = 1 << nBitIndex;
	szHitRuleFlag[nIndex] |= nFlag;

	return TRUE;
}

BOOL CHeurScanner::CalcLollapalooza(int* szHitRuleFlag, int* pRiskValue)
{
	for(int j = 0; j < m_nCombRuleCnt; j++)
	{
		BOOL bHitCombRule = TRUE;
		int* p = (*m_pCombRulers)[j]->GetCombRule();
		for(int i = 0; i < HIT_RULE_FLAG_SIZE; i++)
		{
			if((0 == szHitRuleFlag[i]) && 0 == p[i])
			{
				continue;
			}
			if(0 == (szHitRuleFlag[i] & p[i]))
			{
				bHitCombRule = FALSE;
				break;
			}
		}

		if(bHitCombRule)
		{
			*pRiskValue += (*m_pCombRulers)[j]->GetWeight();
			g_nHitCombRule++;
		}
	}
	
	return TRUE;
}

BOOL CHeurScanner::ReleaseObj()
{
	RETURN_FALSE_IF_FAILED(ExtFuncManager::ReleaseObj());

	return TRUE;
}
