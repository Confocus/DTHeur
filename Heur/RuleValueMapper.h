#pragma once
#include "stdafx.h"
#include "HeurRuleMacro.h"

class CRuleValueMapper
{
public:
	CRuleValueMapper(void);
	~CRuleValueMapper(void);
	
	BOOL Init();
	BOOL Uninit();
	HRule_NAME find(char*);

private:
	std::map<std::string, HRule_NAME> m_mapHRuleName;
};
