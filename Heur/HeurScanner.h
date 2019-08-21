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
	* 初始化，包括加载启发式规则库，启发式特征库
	*
	* @return 
	*/
	BOOL Init();

	/**
	* 启动启发式扫描
	*
	* @param：szPath 扫描对象的路径
	* @return 
	*/
	BOOL Scan(char* szPath);

	/**
	* 对扫描的结果进行统计，后期用来评估权重
	*
	* @return 
	*/
	BOOL Estimate();

	/**
	* 反初始化，包括卸载启发式规则库，启发式特征库等清理工作
	*
	* @return 
	*/
	BOOL UnInit();
	
	/**
	* 设置结果处理回调函数
	*
	* @return 
	*/
	void SetResultCbFunc(PResultCbFunc pResultCbFunc = NULL);	

private:
	

	BOOL ReleaseObj();

	/**
	* 启动启发式扫描，扫描单个文件
	*
	* @return 
	*/
	BOOL ScanPE(char* szPath);

	/**
	* 启动启发式扫描，扫描单个文件
	*
	* @return 
	*/
	BOOL ScanPE();

	/**
	* 启动启发式扫描，扫描某个目录中的文件
	*
	* @szPath 设置待扫描的目录
	* @return 
	*/
	BOOL ScanDir(char* szPath);

	/**
	* 设置待扫描对象
	*
	* @return 
	*/
	BOOL SetObj(char* szPath);

	/**
	* 关联某个规则库和特征库
	*
	* @return 
	*/
	BOOL AssociateHRuleAndHSign(CHeurRuleParser* pHRuleParser, CHeurSignParser* pHSignParser);

	/**
	* 计算满足多个规则时是否产生放大效应
	*
	* @return 
	*/
	BOOL CalcLollapalooza(int* szHitRuleFlag, int* pRiskValue);

	/**
	* 设置命中的规则
	*
	* @return 
	*/
	BOOL SetHitRuleFlag(int* szHitRuleFlag, int nRuleIndex);

private:
	//todo HRule和HSign解析者可能有多个，考虑这里改成数组吧
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
	int m_nScanCount;//统计扫描样本的个数
	char m_szScannedObj[MAX_SIZE];
};
