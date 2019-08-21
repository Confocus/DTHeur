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
	* ��������ʽ�����
	*
	* @return 
	*/
	BOOL LoadHRule();

	/**
	* �õ�����ʽ�����б�
	*
	* @return 
	*/
	//BOOL GetHRulesList(CHRuler*** pppHRule, int* pHRuleNum);
	BOOL GetHRulesList(CSafeArray<CHRuler*>**, int* pHRuleNum);

	BOOL GetCombRulesList(CSafeArray<CCombRule*>**, int* pCombRuleCnt);

	/**
	* ж������ʽ�����
	*
	* @return 
	*/
	BOOL UnLoadHRule();

	/**
	* �����������������������
	*
	* @return 
	*/
	BOOL SetRelatedHeurSignDb(CHeurSignParser* pHSignParser);

private:
	/**
	* ��ȡĳ������ʽ��������Ӧ��ƥ�䷽��
	*
	* @return 
	*/
	HRule_Method GetHRuleMethod();

	int GetHeurRuleCount();

	/**
	* ��ȡini�ļ��н���������
	*
	* @param szIniPath : ini�ļ�·��
	*
	* @return ���ؽ���������
	*/
	int GetIniSecCount(char* szIniPath, int* pSecCount);

	/**
	* ��ʼ���ӹ������������ŵ�map
	*
	* @return 
	*/
	BOOL InitHeurRuleNameMap();

	/**
	* ��ʼ���ӹ����·��
	*
	* @return 
	*/
	void InitHeurRuleDbPath();

	int InitCustFuncMap();

	/**
	* ��ʼ���ӹ����·��
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
	int m_nHRuleCount;//����ʽ�������Ŀ
	int m_nCombRuleCnt;
	CSafeArray<CHRuler*>* m_pHRulers;//����һ��CHRulerָ������飬�����������ļ��еĹ���ĸ����������Ƿ�enable
	CSafeArray<CCombRule*>* m_pCombRules;

	//PExtSignCbFunc* m_szHRuleExtFuncs;
	CSafeArray<PExtSignCbFunc>* m_pHRuleExtFuncs;
	CSafeArray<PMethodCbFunc>* m_pHRuleMethodFuncs;
	//PMethodCbFunc* m_szHRuleMethodFuncs;
	//PMethodCbFunc* m_szHRuleCustFuncs;
	CSafeArray<PMethodCbFunc> *m_pHRuleCustFuncs;
	CHeurSignParser* m_pHSignParser;//todo ���ｫ���ĳ�����
	CMemoryManager* m_pMemManager;
	CRuleValueMapper *m_pRuleValueMapper;
	char m_szHRulePath[MAX_SIZE];
};
