#pragma once
#include "stdafx.h"
#include "HeurRuleMacro.h"
#include "MemoryManager.h"

//这个类统一管理每个扫描的结果

class CResult
{
public:
	CResult(void);
	~CResult(void);

	char* GetObjName();//CResult是有ResultManager在最后统一Release的，这里可以return char*
	void SaveHRName(std::vector<char*> *);
	char* GetHitRules();
	void SetScannedObj(char*);
	void SetRiskValue(int);
	int GetRiskValue();

private:
	BOOL m_bMatch;
	CMemoryManager *m_pMemoryManager;
	int m_nRiskValue;
	char m_szHRName[HR_NAME_SIZE];
	char m_szScannedObj[MAX_SIZE];
	std::vector<char*> m_vecHRName;
};
