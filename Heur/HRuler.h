#pragma once
#include "HeurRuleMacro.h"
#include "HeurSignParser.h"
#include "MemoryManager.h"



class CHRuler
{
public:
	CHRuler(void);
	~CHRuler(void);
public:

	/**
	* 设置启发式规则的匹配方式
	*
	* @param nMethod : 匹配方式所对应的值
	* @return 
	*/
	void SetHRuleMethod(HRule_Method nMethod);

	/**
	* 设置启发式规则查找某条特征时所用到的索引值
	*
	* @param nIndex : 启发式规则查找某条特征时所用到的索引值
	* @return 
	*/
	void SetHRuleIndex(int nIndex);

	/**
	* 设置启发式规则的权重
	*
	* @param nWeight : 启发式规则的权重
	* @return 
	*/
	void SetHRuleWeight(int nWeight);

	/**
	* 设置启发式规则所要匹配的值
	*
	* @param szValue : 启发式规则所要匹配的值
	* @return 
	*/
	void SetHRuleValue(char* szValue);

	/**
	* 设置启发式规则的名称
	*
	* @param szName : 启发式规则的名称
	* @return 
	*/
	void SetHRuleName(char* szName);

	void SetHRuleNameNum(HRule_NAME nHRName);

	/**
	* 设置启发式规则的提取特征回调函数
	*
	* @param pExtSignFunc : 启发式规则的提取特征回调函数
	* @return 
	*/
	void SetExtSignFunc(PExtSignCbFunc pExtSignFunc);

	/**
	* 设置启发式规则的匹配方式的回调函数
	*
	* @param pMatchFunc : 启发式规则的匹配方式的回调函数
	* @param CustFuncNum : 特殊匹配方式的回调函数的索引值
	* @return 
	*/
	void SetMethodFunc(PMethodCbFunc pMatchFunc);

	BOOL SetHSignParser(CHeurSignParser* pHSignParser);//todo HRuler是否需要关联到特征解析
	
	/**
	* 获取启发式规则的对应的编号
	*
	* @return 启发式规则的对应的编号
	*/
	int GetHRuleNameNum();

	/**
	* 获取启发式规则对应的权重
	*
	* @return 启发式规则对应的权重
	*/
	int GetHRuleWeight();

	/**
	* 获取启发式规则对应的名称
	*
	* @param szHRName : 启发式规则对应的名称
	* @return 
	*/
	char* GetHRuleName();

	/**
	* 获取启发式规则的匹配方法的编号
	*
	* @return 启发式规则的匹配方法的编号
	*/
	int GetHRuleMethodNum();

	/**
	* 匹配
	*
	* @param bMatch : 是否匹配到这条规则
	*
	* @return 
	*/
	BOOL Match(BOOL *bMatch);//这里利用两个回调进行判断

private:
	//BOOL QueryHSign(int nIndex, char* szSign);

private:
	//int m_nSecNum;//这个HRule对应的编号
	HRule_Method m_nHRMethod;
	HRule_NAME	m_nHRName;
	int m_nWeight;
	int m_nIndex;
	PExtSignCbFunc m_pExtSignFunc;
	PMethodCbFunc m_pMatchCbFunc;
	//PCustFunc m_pCustFunc;
	char m_szValue[HR_VALUE_SIZE];
	char m_szHRName[HR_NAME_SIZE];
	char m_szSign[HR_SIGN_SIZE];//从特征库中查到的值
	CHeurSignParser* m_pHSignParser; //todo HRuler是否需要关联到特征解析
	CMemoryManager* m_pMemoryManager;
};
