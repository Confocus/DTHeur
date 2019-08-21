#pragma once
#include "stdafx.h"
#include "HeurRuleMacro.h"
#include "HRuler.h"
#include <map>
#include <string>
#include "MemoryManager.h"
#include "PEParser.h"
#include "RuleValueMapper.h"
#include "SafeArray.h"
#include "CombRule.h"

class CHeurRuleParser
{
public:
	CHeurRuleParser(void);
	~CHeurRuleParser(void);

	/**
	* 加载启发式规则库
	*
	* @return 
	*/
	BOOL LoadHRule();

	/**
	* 拿到启发式规则列表
	*
	* @return 
	*/
	//BOOL GetHRulesList(CHRuler*** pppHRule, int* pHRuleNum);
	BOOL GetHRulesList(CSafeArray<CHRuler*>**, int* pHRuleNum);

	BOOL GetCombRulesList(CSafeArray<CCombRule*>**, int* pCombRuleCnt);

	/**
	* 卸载启发式规则库
	*
	* @return 
	*/
	BOOL UnLoadHRule();

	/**
	* 配置这个规则库关联的特征库
	*
	* @return 
	*/
	BOOL SetRelatedHeurSignDb(CHeurSignParser* pHSignParser);

private:
	/**
	* 获取某个启发式规则所对应的匹配方法
	*
	* @return 
	*/
	HRule_Method GetHRuleMethod();

	int GetHeurRuleCount();

	/**
	* 获取ini文件中节区的数量
	*
	* @param szIniPath : ini文件路径
	*
	* @return 返回节区的数量
	*/
	int GetIniSecCount(char* szIniPath, int* pSecCount);

	/**
	* 初始化从规则名到规则编号的map
	*
	* @return 
	*/
	BOOL InitHeurRuleNameMap();

	/**
	* 初始化从规则库路径
	*
	* @return 
	*/
	void InitHeurRuleDbPath();

	int InitCustFuncMap();

	/**
	* 初始化从规则库路径
	*
	* @return 
	*/
	BOOL InitHeurRulerArray();
	BOOL InitMethodFuncArray();
	BOOL InitExtFuncArray();
	BOOL InitCustFuncArray();
	BOOL GenHRulerInstances();
	BOOL InitCombineRulerArray();
	BOOL GetCombRuleInstances();

private:
	int m_nHRuleCount;//启发式规则的数目
	int m_nCombRuleCnt;
	CSafeArray<CHRuler*>* m_pHRulers;//构建一个CHRuler指针的数组，个数是配置文件中的规则的个数，不论是否enable
	CSafeArray<CCombRule*>* m_pCombRules;

	//PExtSignCbFunc* m_szHRuleExtFuncs;
	CSafeArray<PExtSignCbFunc>* m_pHRuleExtFuncs;
	CSafeArray<PMethodCbFunc>* m_pHRuleMethodFuncs;
	//PMethodCbFunc* m_szHRuleMethodFuncs;
	//PMethodCbFunc* m_szHRuleCustFuncs;
	CSafeArray<PMethodCbFunc> *m_pHRuleCustFuncs;
	CHeurSignParser* m_pHSignParser;//todo 这里将来改成数组
	CMemoryManager* m_pMemManager;
	CRuleValueMapper *m_pRuleValueMapper;
	char m_szHRulePath[MAX_SIZE];
};
