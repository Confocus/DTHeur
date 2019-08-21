#include "Result.h"

CResult::CResult(void):
m_pMemoryManager(new CMemoryManager()),
m_bMatch(FALSE),
m_nRiskValue(0)
{
	m_pMemoryManager->SetZero(m_szHRName, HR_NAME_SIZE);
	m_pMemoryManager->SetZero(m_szScannedObj, MAX_SIZE);
}

CResult::~CResult(void)
{
	SAFE_DELETE_POINTER(m_pMemoryManager);
}

void CResult::SaveHRName(std::vector<char*> *p_vecHRName)
{
	char* p = m_szHRName;
	for(std::vector<char*>::iterator it = p_vecHRName->begin(); it != p_vecHRName->end(); it++)
	{
		m_pMemoryManager->Copy(p, HR_NAME_SIZE, *it, HR_NAME_SIZE);
		*(p += strlen(*it)) = ';';
		p++;
	}
}

void CResult::SetScannedObj(char* szPath)
{
	//todo 修改类似的拷贝长度
	m_pMemoryManager->Copy(m_szScannedObj, strlen(szPath) + 1, szPath, strlen(szPath) + 1);
}

void CResult::SetRiskValue(int nRisk)
{
	m_nRiskValue = nRisk;
}

int CResult::GetRiskValue()
{
	return m_nRiskValue;
}

char* CResult::GetObjName()
{
	return m_szScannedObj;
}

char* CResult::GetHitRules()
{
	return m_szHRName;
}