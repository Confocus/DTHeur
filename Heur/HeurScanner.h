#pragma once
#include "stdafx.h"
#include "HeurRuleParser.h"
#include "HeurSignParser.h"
#include "HScannerMarco.h"
#include "IHeurScanner.h"
#include "ScanResultManager.h"

class CHeurScanner : public IHeurScanner
{
public:
	CHeurScanner(void);
	virtual ~CHeurScanner(void);

	/**
	* ��ʼ����������������ʽ����⣬����ʽ������
	*
	* @return 
	*/
	BOOL Init();

	/**
	* ��������ʽɨ��
	*
	* @param��szPath ɨ������·��
	* @return 
	*/
	BOOL Scan(char* szPath);

	/**
	* ��ɨ��Ľ������ͳ�ƣ�������������Ȩ��
	*
	* @return 
	*/
	BOOL Estimate();

	/**
	* ����ʼ��������ж������ʽ����⣬����ʽ�������������
	*
	* @return 
	*/
	BOOL UnInit();
	
	/**
	* ���ý������ص�����
	*
	* @return 
	*/
	void SetResultCbFunc(PResultCbFunc pResultCbFunc = NULL);	

private:
	

	BOOL ReleaseObj();

	/**
	* ��������ʽɨ�裬ɨ�赥���ļ�
	*
	* @return 
	*/
	BOOL ScanPE(char* szPath);

	/**
	* ��������ʽɨ�裬ɨ�赥���ļ�
	*
	* @return 
	*/
	BOOL ScanPE();

	/**
	* ��������ʽɨ�裬ɨ��ĳ��Ŀ¼�е��ļ�
	*
	* @szPath ���ô�ɨ���Ŀ¼
	* @return 
	*/
	BOOL ScanDir(char* szPath);

	/**
	* ���ô�ɨ�����
	*
	* @return 
	*/
	BOOL SetObj(char* szPath);

	/**
	* ����ĳ��������������
	*
	* @return 
	*/
	BOOL AssociateHRuleAndHSign(CHeurRuleParser* pHRuleParser, CHeurSignParser* pHSignParser);

	/**
	* ��������������ʱ�Ƿ�����Ŵ�ЧӦ
	*
	* @return 
	*/
	BOOL CalcLollapalooza(int* szHitRuleFlag, int* pRiskValue);

	/**
	* �������еĹ���
	*
	* @return 
	*/
	BOOL SetHitRuleFlag(int* szHitRuleFlag, int nRuleIndex);

private:
	//todo HRule��HSign�����߿����ж������������ĳ������
	CHeurRuleParser* m_pHRuleParser;
	CHeurSignParser* m_pHSignParser;
	CMemoryManager* m_pMemoryManager;
	CScanResultManager* m_pScanResultManager;
	CSafeArray<CHRuler*> *m_pScanHRRulers;
	CSafeArray<CCombRule*> *m_pCombRulers;
	PResultCbFunc m_pResultCbFunc;
	//CHRuler** m_szHRuler;
	int m_nRiskValue;
	int m_nHRuleCount;
	int m_nCombRuleCnt;
	int m_nScanCount;//ͳ��ɨ�������ĸ���
	char m_szScannedObj[MAX_SIZE];
};
