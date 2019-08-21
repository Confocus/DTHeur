#pragma once
#include "stdafx.h"
#include "Result.h"
#include "RuleValueMapper.h"

//这个类用来统一管理Result
class CScanResultManager
{
public:
	CScanResultManager(void);
	~CScanResultManager(void);
	
	void SaveResult(CResult*);
	void GetScanResult(std::vector<CResult*>&);
	void Release();//遍历Map，该释放的释放
	int GetScanCount();
	void SetScanCount(int);
	void ShowHitRuleStatic();

private:
	void SaveHitRules();
	
	//todo 增加一个命中的规则的数量的统计
	//todo 增加一个命中的规则的比率的统计
	
private:
	//std::map<std::string, CResult*> m_mapResult;
	std::vector<CResult*> m_vecResult;
	int m_nScanCount;
	int m_szHitRules[MAX_HEUR_RULE_COUNT];
	CMemoryManager* m_pMemoryManager;
	CRuleValueMapper* m_pRuleValueMapper;
};
