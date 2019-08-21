#pragma once
#include "stdafx.h"

class IHeurScanner
{
public:
	/**
	* 初始化，包括加载启发式规则库，启发式特征库
	*
	* @return 
	*/
	virtual BOOL Init() = 0;

	/**
	* 启动启发式扫描
	*
	* @return 
	*/
	virtual BOOL Scan(char*) = 0;

	/**
	* 反初始化，包括卸载启发式规则库，启发式特征库等清理工作
	*
	* @return 
	*/
	virtual BOOL UnInit() = 0;
	
	/**
	* 设置结果处理回调函数
	*
	* @return 
	*/
	virtual void SetResultCbFunc(PResultCbFunc pResultCbFunc = NULL) = 0;
};