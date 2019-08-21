#pragma once
#include "stdafx.h"
#include "HeurRuleMacro.h"
#include "ACMatcher.h"
#include <set>

class MethodFuncManager
{
public:
	MethodFuncManager(void);
	~MethodFuncManager(void);

public:
	/**
	* ��������1����ȷƥ�������ļ��е�value��Ӧ��ֵ
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodAccurateMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* ��������2���ж�ĳ������ֵ�Ƿ�����ĳ����Χ
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodWithinRange(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* ��������3��
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodSignDbStringMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);


	/**
	* ��������4���ж������Ƿ����������Ӧ���������г��ֹ�
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodSignDbAnyofMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* ��������5���ж��������������Ӧ���������г��ֵı���
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodSignDbRatioMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* ��������6���ж��������������Ӧ���������г��ֵĸ���
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	//BOOL MethodSignDbCountMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
	//{
	//	return TRUE;
	//}

	/**
	* ��������6�����ַ�ƥ���㷨ȥƥ��ĳ��Buf���Ƿ����ĳ���������е�һ��
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodACSearchMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);
	
	/**
	* ��������7�����ַ�ƥ���㷨ȥƥ��ĳ��Buf���Ƿ����ĳ�������Լ��������ı���
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodACSearchMatchRatio(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* ��������8�����ַ�ƥ���㷨ȥƥ��ĳ��Buf���Ƿ����ĳ���������е�����������
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodACSearchMatchNum(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* ��������9�����ַ�ƥ���㷨ȥƥ��ĳ��Buf���Ƿ����ĳ�������Լ��������Ĵ���
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodACSearchMatchCount(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* ��������10����������ĳ��ָ���Ƿ���ڣ�֧��ģ��ƥ��
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodInstSearch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* ��������11����������ĳ��ָ����ֵĴ�����֧��ģ��ƥ��
	*
	* @param PMETHOD_FUNC_IN_PARAM : ���������õ�������
	* @param PEXT_SIGN �����������hr.ini���õ�������ֵ
	* @param PBOOL�ȽϽ��
	*
	* @return 
	*/
	static BOOL MethodInstSearchRatio(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	static BOOL Init(CHeurSignParser*);
	static BOOL Release();
private:
	
	/**
	* ������[,]��(,)����ʽ��ʾ��Χ,[,](,)(,][,) �Ƿ��������ƥ��
	*
	* @param result : ����Ľ��ֵ
	* @param szValue�����ַ�����ʽ��ʾ��ƥ�䷶Χ
	* @param PBOOL���ȽϽ��
	*
	* @return 
	*/
	template<typename T>
	static BOOL MethodParseWithinRangeExpression(T result, char* szValue, PBOOL pMatch);

	static BOOL CreateUnicodeBuff(PMETHOD_PARAM ,MEMREF*);

	static BOOL FreeUnicodeBuff(MEMREF*);
	
	static int MethodGetSignNum(char* szSigns);

	static void InitSignPath();

public:
	static std::set<int> m_Set;

private:
	static char m_szHSignPath[MAX_SIZE];
	static CACMatcher* m_pACMatcher;
	static ACISM_ACTION* m_pResultCb;
	//static int m_nCount;
};
