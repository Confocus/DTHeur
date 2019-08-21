#pragma once
#include "stdafx.h"
#include "HeurRuleMacro.h"
#include "MemoryManager.h"

//�����ͳһ����ÿ��ɨ��Ľ��

class CResult
{
public:
	CResult(void);
	~CResult(void);

	char* GetObjName();//CResult����ResultManager�����ͳһRelease�ģ��������return char*
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
