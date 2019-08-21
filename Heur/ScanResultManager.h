#pragma once
#include "stdafx.h"
#include "Result.h"
#include "RuleValueMapper.h"

//���������ͳһ����Result
class CScanResultManager
{
public:
	CScanResultManager(void);
	~CScanResultManager(void);
	
	void SaveResult(CResult*);
	void GetScanResult(std::vector<CResult*>&);
	void Release();//����Map�����ͷŵ��ͷ�
	int GetScanCount();
	void SetScanCount(int);
	void ShowHitRuleStatic();

private:
	void SaveHitRules();
	
	//todo ����һ�����еĹ����������ͳ��
	//todo ����һ�����еĹ���ı��ʵ�ͳ��
	
private:
	//std::map<std::string, CResult*> m_mapResult;
	std::vector<CResult*> m_vecResult;
	int m_nScanCount;
	int m_szHitRules[MAX_HEUR_RULE_COUNT];
	CMemoryManager* m_pMemoryManager;
	CRuleValueMapper* m_pRuleValueMapper;
};
