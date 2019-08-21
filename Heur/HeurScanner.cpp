#include "HeurScanner.h"
#include "HRuler.h"
#include "HeurRuleMacro.h"
#include "PEParser.h"
#include "ExtFuncManager.h"
#include "Result.h"
#include "MethodFuncManager.h"

extern ULONGLONG dwTotalRiskValue;
extern int g_nHitCombRule;

CHeurScanner::CHeurScanner(void)://智能指针的初始化
m_pHRuleParser(new CHeurRuleParser()),//todo 改成智能指针
m_pHSignParser(new CHeurSignParser()),//todo 改成智能指针
m_pMemoryManager(new CMemoryManager()),//todo 改成智能指针
m_pScanResultManager(new CScanResultManager()),//todo 改成智能指针
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
	RETURN_FALSE_IF_FAILED(AssociateHRuleAndHSign(m_pHRuleParser, m_pHSignParser));//这里要先关联数据库与特征库，然后再Load数据库与特征库
	RETURN_FALSE_IF_FAILED(m_pHRuleParser->LoadHRule());
	RETURN_FALSE_IF_FAILED(m_pHSignParser->LoadHSign());
	RETURN_FALSE_IF_FAILED(m_pHRuleParser->GetHRulesList(&m_pScanHRRulers, &m_nHRuleCount));//拿到HRuler列表
	RETURN_FALSE_IF_FAILED(m_pHRuleParser->GetCombRulesList(&m_pCombRulers, &m_nCombRuleCnt));//拿到HRuler列表
	MethodFuncManager::Init(m_pHSignParser);

	return TRUE;
}

BOOL CHeurScanner::ScanPE()
{
	m_nRiskValue = 0;//每扫描一个PE之前重置一下
	int szHitRuleFlag[HIT_RULE_FLAG_SIZE] = {0};//保存有哪些规则命中了

	//todo 测试阶段，暂时保存所有样本的结果，后期只需要保留黑样本的结果即可
	//todo 生成一个Match结构体，每个对象最终对应一个Match
	std::vector<char*> vecHRName;

	//todo：
	//从Paresr中拿到待扫描对象列表
	//给PEParser配置这个CPeObj
	//启动Match，其实就是在调用Match前再多一次循环
	//形如：while(!PEParser.GetObj)

	//依次用每个HRuler提取规则并做匹配
	for(int i = 0; i <= m_nHRuleCount; i++)
	{	
		if((*m_pScanHRRulers)[i] != NULL)
		{	
			BOOL bMatch = FALSE;
			if(!((*m_pScanHRRulers)[i])->Match(&bMatch))//里边取到必要的扫描结果的内容
			{
				continue;
			}

			//计算风险值
			m_nRiskValue += bMatch * (*m_pScanHRRulers)[i]->GetHRuleWeight();
			if(bMatch)
			{
				//保存命中了哪条规则
				vecHRName.push_back(((*m_pScanHRRulers)[i])->GetHRuleName());
				SetHitRuleFlag(szHitRuleFlag, ((*m_pScanHRRulers)[i])->GetHRuleNameNum());
			}
		}
	}

	CalcLollapalooza(szHitRuleFlag, &m_nRiskValue);
	dwTotalRiskValue += m_nRiskValue;

	if(m_nRiskValue >= 0)//todo 这里后续可以改为可配置的，现在暂时为0，表示是否为黑白都保存
	{
		CResult *pResult = new CResult();
		RETURN_FALSE_IF_FAILED(pResult);
		pResult->SaveHRName(&vecHRName);
		pResult->SetScannedObj(m_szScannedObj);
		pResult->SetRiskValue(m_nRiskValue);
		//把所有的CResult类统一到ResultManager中管理
		m_pScanResultManager->SaveResult(pResult);//将在UnLoad中释放
		m_pScanResultManager->SetScanCount(m_nScanCount);
	}

	//todo 扫描完之后重新调用SetObj或者取CPeObj列表中的下一个，继续扫描
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
		if(szPath[strlen(szPath) - 1] != '\\')//格式化路径
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
	//路径列表将来保存在PEParser中吧
	//todo  添加init接口
	m_pMemoryManager->SetZero(m_szScannedObj, MAX_SIZE);
	m_pMemoryManager->Copy(m_szScannedObj, MAX_SIZE, szPath, MAX_SIZE);
	RETURN_FALSE_IF_FAILED(ExtFuncManager::SetObj(szPath));

	return TRUE;
}

BOOL CHeurScanner::Estimate()
{
	DEBUG_PRINT("Estimate start..\n");
	RETURN_FALSE_IF_FAILED(m_pResultCbFunc);
	RETURN_FALSE_IF_FAILED(m_pResultCbFunc(m_pScanResultManager));//调用回调函数处理匹配的结果

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
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)    // 是否是目录 
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
