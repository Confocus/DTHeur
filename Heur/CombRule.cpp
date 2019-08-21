#include "CombRule.h"

CCombRule::CCombRule(void):
m_nWieight(0),
m_nIndex(0)
{
	for(int i = 0; i < HIT_RULE_FLAG_SIZE; i++)
	{
		m_szCombRule[i] = 0;
	}
}

CCombRule::~CCombRule(void)
{
}

void CCombRule::SetIndex(int nIndex)
{
	m_nIndex = nIndex;
}

void CCombRule::SetCombRule(int nRuleIndex)
{
	if(nRuleIndex >= HR_END)
	{
		return;
	}
	int nIndex = nRuleIndex / (sizeof(int) * 8);
	int nBitIndex = nRuleIndex % (sizeof(int) * 8);
	int nFlag = 1 << nBitIndex;
	m_szCombRule[nIndex] |= nFlag;

	return ;
}

void CCombRule::SetWeight(int nWeight)
{
	m_nWieight = nWeight;
}

int* CCombRule::GetCombRule()
{
	return m_szCombRule;
}

int CCombRule::GetWeight()
{
	return m_nWieight;
}