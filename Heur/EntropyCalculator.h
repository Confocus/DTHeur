#pragma once
#include "stdafx.h"
#include <stdint.h>
#include <algorithm>
#include <cmath>

#define MAX_SIZE	0x500

class CEntropyCalculator
{
public:
	CEntropyCalculator(void);
	~CEntropyCalculator(void);

public:
	void Clean();
	BOOL Check(PUCHAR, BOOL, LONGLONG);
	void Check2(PUCHAR, BOOL, LONGLONG, double*);//计算熵值但是不包括头部

	BOOL CheckExtraEntropy(PUCHAR, BOOL, LONGLONG, LONGLONG, double*);

	/**
	* 设置这条特征的属性，是黑特征还是白特征
	*
	* @param image_base : 待计算熵的内存数据地址
	* @param bmem : 是内存数据还是文件映射，默认是FALSE（文件映射）
	* @param llSize : 传入的数据的大小
	* @param nSecIndex : 要计算熵的节区的索引值
	* @param pfent : 保存计算的熵值的变量的指针
	* @return 
	*/
	BOOL CheckSection(PUCHAR image_base, BOOL bmem, LONGLONG llSize, int nSecIndex, double* pfent);

	/**
	* 设置这条特征的属性，是黑特征还是白特征
	*
	* @param image_base : 待计算熵的内存数据地址
	* @param bmem : 是内存数据还是文件映射，默认是FALSE（文件映射）
	* @param llSize : 传入的数据的大小
	* @param pfent : 保存计算的熵值的变量的指针
	* @return 
	*/
	BOOL CheckFileEntropy(PUCHAR image_base, BOOL bmem, LONGLONG llSize, double* pfent);

	/**
	* 设置这条特征的属性，是黑特征还是白特征
	*
	* @param image_base : 待计算熵的内存数据地址
	* @param bmem : 是内存数据还是文件映射，默认是FALSE（文件映射）
	* @param llSize : 传入的数据的大小
	* @param pfent : 保存计算的熵值的变量的指针
	* @return 
	*/
	BOOL CheckRsrcEntropy(PUCHAR image_base, BOOL bmem, LONGLONG llSize, double* pfent);

	/**
	* 设置这条特征的属性，是黑特征还是白特征
	*
	* @param image_base : 待计算熵的内存数据地址
	* @param bmem : 是内存数据还是文件映射，默认是FALSE（文件映射）
	* @param llSize : 传入的数据的大小
	* @return 
	*/
	BOOL CheckBlockEntropy(PUCHAR image_base, BOOL bmem, LONGLONG llSize);

	BOOL Is_Packed(PUCHAR);

	double GetHighEntropySectionRatio();

	double GetHighEntropyBlockRatio();

private:
	double CalcEntropy(const uint32_t byte_count[256], size_t total_length);
	double CalcEntropy(const char* data, size_t length);
	BOOL GetEntropyConfigPath();
	BOOL ParseEntropyFile();
	BOOL GetEntropyIniValue(char* szItemName, double* pEntropy);

private:
	int m_nHighEntropyBlockCnt;
	double m_dHighBlockEntropy;
	double m_dHighEntropyBlockRatio;
	double m_dHighSectionEntropy;
	double m_dHighEntropySectionRatio;
	char m_szEntropyFilePath[MAX_SIZE];
};