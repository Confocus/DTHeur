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
	* ��������ʽ�����ƥ�䷽ʽ
	*
	* @param nMethod : ƥ�䷽ʽ����Ӧ��ֵ
	* @return 
	*/
	void SetHRuleMethod(HRule_Method nMethod);

	/**
	* ��������ʽ�������ĳ������ʱ���õ�������ֵ
	*
	* @param nIndex : ����ʽ�������ĳ������ʱ���õ�������ֵ
	* @return 
	*/
	void SetHRuleIndex(int nIndex);

	/**
	* ��������ʽ�����Ȩ��
	*
	* @param nWeight : ����ʽ�����Ȩ��
	* @return 
	*/
	void SetHRuleWeight(int nWeight);

	/**
	* ��������ʽ������Ҫƥ���ֵ
	*
	* @param szValue : ����ʽ������Ҫƥ���ֵ
	* @return 
	*/
	void SetHRuleValue(char* szValue);

	/**
	* ��������ʽ���������
	*
	* @param szName : ����ʽ���������
	* @return 
	*/
	void SetHRuleName(char* szName);

	void SetHRuleNameNum(HRule_NAME nHRName);

	/**
	* ��������ʽ�������ȡ�����ص�����
	*
	* @param pExtSignFunc : ����ʽ�������ȡ�����ص�����
	* @return 
	*/
	void SetExtSignFunc(PExtSignCbFunc pExtSignFunc);

	/**
	* ��������ʽ�����ƥ�䷽ʽ�Ļص�����
	*
	* @param pMatchFunc : ����ʽ�����ƥ�䷽ʽ�Ļص�����
	* @param CustFuncNum : ����ƥ�䷽ʽ�Ļص�����������ֵ
	* @return 
	*/
	void SetMethodFunc(PMethodCbFunc pMatchFunc);

	BOOL SetHSignParser(CHeurSignParser* pHSignParser);//todo HRuler�Ƿ���Ҫ��������������
	
	/**
	* ��ȡ����ʽ����Ķ�Ӧ�ı��
	*
	* @return ����ʽ����Ķ�Ӧ�ı��
	*/
	int GetHRuleNameNum();

	/**
	* ��ȡ����ʽ�����Ӧ��Ȩ��
	*
	* @return ����ʽ�����Ӧ��Ȩ��
	*/
	int GetHRuleWeight();

	/**
	* ��ȡ����ʽ�����Ӧ������
	*
	* @param szHRName : ����ʽ�����Ӧ������
	* @return 
	*/
	char* GetHRuleName();

	/**
	* ��ȡ����ʽ�����ƥ�䷽���ı��
	*
	* @return ����ʽ�����ƥ�䷽���ı��
	*/
	int GetHRuleMethodNum();

	/**
	* ƥ��
	*
	* @param bMatch : �Ƿ�ƥ�䵽��������
	*
	* @return 
	*/
	BOOL Match(BOOL *bMatch);//�������������ص������ж�

private:
	//BOOL QueryHSign(int nIndex, char* szSign);

private:
	//int m_nSecNum;//���HRule��Ӧ�ı��
	HRule_Method m_nHRMethod;
	HRule_NAME	m_nHRName;
	int m_nWeight;
	int m_nIndex;
	PExtSignCbFunc m_pExtSignFunc;
	PMethodCbFunc m_pMatchCbFunc;
	//PCustFunc m_pCustFunc;
	char m_szValue[HR_VALUE_SIZE];
	char m_szHRName[HR_NAME_SIZE];
	char m_szSign[HR_SIGN_SIZE];//���������в鵽��ֵ
	CHeurSignParser* m_pHSignParser; //todo HRuler�Ƿ���Ҫ��������������
	CMemoryManager* m_pMemoryManager;
};
