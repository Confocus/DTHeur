#include "ScanResultManager.h"

extern double dTotalEntryEntropyValue;
extern int nEntropyItem;
extern int nBlockItem;
extern ULONGLONG dwTotalRiskValue;
int g_nHitCombRule = 0;
extern int g_nWhite;
extern int g_nBlack;
extern int g_nPharse1;
extern int g_nPharse2;
extern int g_nPharse3;
extern int g_nPharse4;
extern int g_nPharse5;
extern int g_nPharse6;
extern int g_nPharse7;
extern int g_nPharse8;
extern int g_nPharse9;
extern int g_nPharse10;

double g_dExtraEntropy = 0.0;
int g_nExtraCnt = 0;

CScanResultManager::CScanResultManager(void):
m_nScanCount(0),
m_pRuleValueMapper(new CRuleValueMapper()),//todo smtprt
m_pMemoryManager(new CMemoryManager())
{
	m_pMemoryManager->SetZero(m_szHitRules, MAX_HEUR_RULE_COUNT * sizeof(int) + 1);//0 ~ MAX_HEUR_RULE_COUNTÊÇMAX_HEUR_RULE_COUNT+1¸ö
	m_pRuleValueMapper->Init();
}

CScanResultManager::~CScanResultManager(void)
{
	SAFE_DELETE_POINTER(m_pRuleValueMapper);
	SAFE_DELETE_POINTER(m_pMemoryManager);
	m_pRuleValueMapper->Uninit();
}

void CScanResultManager::SaveResult(CResult* pResult)
{
	m_vecResult.push_back(pResult);
}

int CScanResultManager::GetScanCount()
{
	return m_nScanCount;
}

void CScanResultManager::SetScanCount(int nCount)
{
	m_nScanCount = nCount;
}

void CScanResultManager::Release()
{
	for(std::vector<CResult*>::iterator it = m_vecResult.begin(); it != m_vecResult.end(); it++)
	{
		if(!(*it))
		{
			delete(*it);
		}
	}
}

void CScanResultManager::GetScanResult(std::vector<CResult*> &vecResult)
{
	vecResult = m_vecResult;
}

void CScanResultManager::SaveHitRules()
{
	for(std::vector<CResult*>::iterator it = m_vecResult.begin(); it != m_vecResult.end(); it++)
	{
		char szHitRule[HR_NAME_SIZE] = {0};
		char *p = szHitRule;
		p = (*it)->GetHitRules();
		char* pPos = NULL;
		pPos = strchr(p, ';');
		while(pPos)
		{
			*pPos = 0;
			if(HR_NONE == m_pRuleValueMapper->find(p))
			{
				continue;
			}
			m_szHitRules[m_pRuleValueMapper->find(p)]++;
			pPos++;
			p = pPos;
			pPos = strchr(p, ';');
		}
	}
}

void CScanResultManager::ShowHitRuleStatic()
{
	DEBUG_PRINT("ShowHitRuleStatic..\n");
	SaveHitRules();
	for(int i = 1; i < MAX_HEUR_RULE_COUNT; i++)
	{
		if(0 == m_szHitRules[i])
		{
			continue;
		}

		printf("Rule %d : %d  %f\t", i, m_szHitRules[i], ((double)m_szHitRules[i])/((double)m_nScanCount));
		if(0 == i % 3)
		{
			printf("\n");
		}
	}
	DEBUG_PRINT("CombRule:%f\t\n", ((double)g_nHitCombRule)/((double)m_nScanCount));
	/*DEBUG_PRINT("BlackRatio:%f\t\n", ((double)g_nBlack)/((double)m_nScanCount));
	DEBUG_PRINT("WhiteRatio:%f\t\n", ((double)g_nWhite)/((double)m_nScanCount));*/
	DEBUG_PRINT("Pharse1:%f\t\n", ((double)g_nPharse1)/((double)m_nScanCount));
	DEBUG_PRINT("Pharse2:%f\t\n", ((double)g_nPharse2)/((double)m_nScanCount));
	DEBUG_PRINT("Pharse3:%f\t\n", ((double)g_nPharse3)/((double)m_nScanCount));
	DEBUG_PRINT("Pharse4:%f\t\n", ((double)g_nPharse4)/((double)m_nScanCount));
	DEBUG_PRINT("Pharse5:%f\t\n", ((double)g_nPharse5)/((double)m_nScanCount));
	DEBUG_PRINT("Pharse6:%f\t\n", ((double)g_nPharse6)/((double)m_nScanCount));
	DEBUG_PRINT("Pharse7:%f\t\n", ((double)g_nPharse7)/((double)m_nScanCount));
	DEBUG_PRINT("Pharse8:%f\t\n", ((double)g_nPharse8)/((double)m_nScanCount));
	DEBUG_PRINT("Pharse9:%f\t\n", ((double)g_nPharse9)/((double)m_nScanCount));
	DEBUG_PRINT("Pharse10:%f\t\n", ((double)g_nPharse10)/((double)m_nScanCount));
	DEBUG_PRINT("Average risk value is %lf\n", (double)dwTotalRiskValue/(double)m_nScanCount);
	DEBUG_PRINT("Average entropy value is %lf\n", (double)g_dExtraEntropy/(double)g_nExtraCnt);

	//printf("\nEntry section entropy value is %lf, scan count is %d, average value is %lf\n", dTotalEntryEntropyValue, nEntropyItem, dTotalEntryEntropyValue/nEntropyItem);
	printf("\n");
}
