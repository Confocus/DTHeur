#include "HRuler.h"

extern int nBlockItem;

CHRuler::CHRuler(void):
m_nIndex(0),
m_nWeight(0),
m_nHRName(HR_NONE),
m_nHRMethod(METHOD_NONE),
m_pExtSignFunc(NULL),
m_pMatchCbFunc(NULL),
m_pMemoryManager(new CMemoryManager()),//todo
m_pHSignParser(NULL)//todo没有必要每个规则都生成Parser
{
	memset(m_szSign, 0, HR_SIGN_SIZE * sizeof(char));
	memset(m_szValue, 0, HR_VALUE_SIZE * sizeof(char));
	memset(m_szHRName, 0, HR_NAME_SIZE * sizeof(char));
}

CHRuler::~CHRuler(void)
{
	SAFE_DELETE_POINTER(m_pMemoryManager);
}

BOOL CHRuler::Match(BOOL* pMatch)
{
	RETURN_FALSE_IF_FAILED(pMatch);
	RETURN_FALSE_IF_FAILED(m_pExtSignFunc);
	RETURN_FALSE_IF_FAILED(m_pMatchCbFunc);

	//封装特征提取函数中可能用到的参数
	EXT_FUNC_PARAM ExtFuncParam = {0};
	ExtFuncParam.InParam.nIndex = m_nIndex;
	ExtFuncParam.InParam.nWeight = m_nWeight;
	ExtFuncParam.InParam.nMethod = m_nHRMethod;
	m_pMemoryManager->Copy(ExtFuncParam.InParam.szValue, HR_VALUE_SIZE, m_szValue, HR_VALUE_SIZE);
	
	//这一步封装可以提前到配置HRuler时
	//输入的是Ini中拿到的值或者从特征库中拿到的特征
	//todo：m_pHSignParser成员设置到MethodParam.pHSignParser，是不是ExtSign类就可以省去了？
	EXT_SIGN ExtSign;
	m_pMemoryManager->Copy((void*)ExtSign.szValue, HR_VALUE_SIZE, m_szValue, HR_VALUE_SIZE);
	m_pMemoryManager->Copy((void*)ExtSign.szSign, HR_SIGN_SIZE, m_szSign, HR_SIGN_SIZE);

	RETURN_FALSE_IF_FAILED(m_pExtSignFunc(&ExtFuncParam));//拿到该拿的特征、值
	
	//ExtFuncParam 转为 METHOD_PARAM 
	//输入的是拿到特征之后的结果
	METHOD_PARAM MethodParam;
	MethodParam.nResult = ExtFuncParam.OutParam.nResult;
	MethodParam.dResult = ExtFuncParam.OutParam.dResult;
	MethodParam.nType = ExtFuncParam.OutParam.nType;
	MethodParam.nIndex = m_nIndex;
	MethodParam.BufResult.ptr = ExtFuncParam.OutParam.BufResult.ptr;
	MethodParam.BufResult.len = ExtFuncParam.OutParam.BufResult.len;
	MethodParam.pHSignParser = m_pHSignParser;
	m_pMemoryManager->Copy(MethodParam.szResult, HR_SIGN_SIZE, ExtFuncParam.OutParam.szResult, HR_SIGN_SIZE);
	m_pMemoryManager->Copy(MethodParam.szValue, HR_VALUE_SIZE, m_szValue, HR_VALUE_SIZE);
	
	RETURN_FALSE_IF_FAILED(m_pMatchCbFunc(&MethodParam , &ExtSign, pMatch));//拿到是否命中这条规则

	return TRUE;
}

void CHRuler::SetHRuleMethod(HRule_Method nMethod)
{
	m_nHRMethod = nMethod;
	return ;
}

void CHRuler::SetHRuleIndex(int nIndex)
{
	m_nIndex = nIndex;
	return ;
}

void CHRuler::SetHRuleWeight(int nWeight)
{
	m_nWeight = nWeight;
	return ;
}

void CHRuler::SetHRuleValue(char* szValue)
{
	m_pMemoryManager->Copy(m_szValue, HR_VALUE_SIZE, szValue, strlen(szValue)/sizeof(szValue[0]));

	return;
}

void CHRuler::SetHRuleName(char* szName)
{
	m_pMemoryManager->Copy(m_szHRName, HR_NAME_SIZE, szName, strlen(szName)/sizeof(szName[0]));

	return;
}

void CHRuler::SetExtSignFunc(PExtSignCbFunc pExtSignFunc)
{
	if(pExtSignFunc)
	{
		m_pExtSignFunc = pExtSignFunc;
	}

	return;
}

void CHRuler::SetMethodFunc(PMethodCbFunc pMatchFunc)
{
	//todo 如果是定制化方法，则利用nCustFuncNum做特殊处理
	//todo 空指针返回定义成宏
	RETURN_IF_FAILED(pMatchFunc);
	
	if(m_nHRMethod == METHOD_CUST)
	{
		if(m_nIndex > MAX_HR_METHOD_COUNT)//防止特殊匹配函数列表访问越界
		{
			m_pMatchCbFunc = NULL;
		}
		else
		{
			m_pMatchCbFunc = *((PMethodCbFunc*)pMatchFunc + m_nIndex * sizeof(PMethodCbFunc));
		}
	}
	else
	{
		m_pMatchCbFunc = pMatchFunc;
	}

	return;
}

int CHRuler::GetHRuleNameNum()
{
	return m_nHRName;
}

char* CHRuler::GetHRuleName()
{
	return m_szHRName;
}

int CHRuler::GetHRuleWeight()
{
	return m_nWeight;
}

int CHRuler::GetHRuleMethodNum()
{
	return m_nHRMethod;
}

void CHRuler::SetHRuleNameNum(HRule_NAME nHRName)
{
	m_nHRName = nHRName;
}

BOOL CHRuler::SetHSignParser(CHeurSignParser* pHSignParser)
{
	if(!pHSignParser)
	{
		return FALSE;
	}
	m_pHSignParser = pHSignParser;

	return TRUE;
}
