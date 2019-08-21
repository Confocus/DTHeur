#pragma once
#include "HeurRuleMacro.h"

class CCombRule
{
public:
	CCombRule(void);
	~CCombRule(void);

	void SetIndex(int);
	void SetWeight(int);
	void SetCombRule(int);
	int* GetCombRule();
	int GetWeight();

private:
	int m_szCombRule[HIT_RULE_FLAG_SIZE];
	int m_nIndex;
	int m_nWieight;
};
