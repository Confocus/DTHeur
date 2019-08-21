#include "HeurScanner.h"
#include "HScannerMarco.h"
#include "PeObj.h"
#include "ExtFuncManager.h"

double dTotalEntryEntropyValue = 0.0;
int nEntropyItem = 0;
int nBlockItem  =0;
ULONGLONG dwTotalRiskValue = 0;
#define THRESHOLD_VALUE	1200
int g_nWhite = 0;
int g_nBlack = 0;
int g_nPharse1 = 0;
int g_nPharse2 = 0;
int g_nPharse3 = 0;
int g_nPharse4 = 0;
int g_nPharse5 = 0;
int g_nPharse6 = 0;
int g_nPharse7 = 0;
int g_nPharse8 = 0;
int g_nPharse9 = 0;
int g_nPharse10 = 0;

BOOL cbResultFunc(CScanResultManager* pResultManager)
{
	std::vector<CResult*> vecResults;
	pResultManager->GetScanResult(vecResults);
	for(std::vector<CResult*>::iterator it = vecResults.begin(); it != vecResults.end(); it++)
	{
		if((*it)->GetRiskValue() < 200)
		{
			g_nPharse1++;
			printf("Object:%s\t, Risk:%d\t, HitRule:%s\n", (*it)->GetObjName(), (*it)->GetRiskValue(), (*it)->GetHitRules());
		}
		else if((*it)->GetRiskValue() < 400)
		{
			g_nPharse2++;
		}
		else if((*it)->GetRiskValue() < 600)
		{
			g_nPharse3++;
		}
		else if((*it)->GetRiskValue() < 800)
		{
			g_nPharse4++;
		}
		else if((*it)->GetRiskValue() < 1000)
		{
			g_nPharse5++;
		}
		else if((*it)->GetRiskValue() < 1200)
		{
			g_nPharse6++;
		}
		else if((*it)->GetRiskValue() < 1400)
		{
			g_nPharse7++;
		}
		else if((*it)->GetRiskValue() < 1600)
		{
			g_nPharse8++;
		}
		else if((*it)->GetRiskValue() < 1800)
		{
			g_nPharse9++;
		}
		else
		{
			g_nPharse10++;
			printf("Object:%s\t, Risk:%d\t, HitRule:%s\n", (*it)->GetObjName(), (*it)->GetRiskValue(), (*it)->GetHitRules());
		}

		/*if((*it)->GetRiskValue() < THRESHOLD_VALUE)
		{
			g_nWhite++;
		}
		else
		{
			g_nBlack++;
		}*/
	}
	pResultManager->ShowHitRuleStatic();

	return TRUE;
}

//todo 支持多个规则组合，比如代码段在最后一个节且节名异常
void main(int argc, char *argv[])
{
	CHeurScanner HeurScanner;
	HeurScanner.Init();
	
	HeurScanner.SetResultCbFunc(cbResultFunc);//设置结果处理回调函数
	//todo 后期这里改为可以Set多个待扫描对象
	//或者这里改成for循环
	//把SetObj和Scan封装在一起
	HeurScanner.Scan(argv[1]);
	//HeurScanner.Scan("F:\\10\\");//\\70_6062487e3aa998687ee1ed6b99199b20
	//F:\\sample1\\;D:\\Test\\unresolve\\;F:\\10\\;F:\\wsample\\;F:\\crash\\;D:\\Test\\infectsample\\identify\\;
	HeurScanner.Estimate();

	HeurScanner.UnInit();
	getchar();
	exit(0);
}