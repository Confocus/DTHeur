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
	void Check2(PUCHAR, BOOL, LONGLONG, double*);//������ֵ���ǲ�����ͷ��

	BOOL CheckExtraEntropy(PUCHAR, BOOL, LONGLONG, LONGLONG, double*);

	/**
	* �����������������ԣ��Ǻ��������ǰ�����
	*
	* @param image_base : �������ص��ڴ����ݵ�ַ
	* @param bmem : ���ڴ����ݻ����ļ�ӳ�䣬Ĭ����FALSE���ļ�ӳ�䣩
	* @param llSize : ��������ݵĴ�С
	* @param nSecIndex : Ҫ�����صĽ���������ֵ
	* @param pfent : ����������ֵ�ı�����ָ��
	* @return 
	*/
	BOOL CheckSection(PUCHAR image_base, BOOL bmem, LONGLONG llSize, int nSecIndex, double* pfent);

	/**
	* �����������������ԣ��Ǻ��������ǰ�����
	*
	* @param image_base : �������ص��ڴ����ݵ�ַ
	* @param bmem : ���ڴ����ݻ����ļ�ӳ�䣬Ĭ����FALSE���ļ�ӳ�䣩
	* @param llSize : ��������ݵĴ�С
	* @param pfent : ����������ֵ�ı�����ָ��
	* @return 
	*/
	BOOL CheckFileEntropy(PUCHAR image_base, BOOL bmem, LONGLONG llSize, double* pfent);

	/**
	* �����������������ԣ��Ǻ��������ǰ�����
	*
	* @param image_base : �������ص��ڴ����ݵ�ַ
	* @param bmem : ���ڴ����ݻ����ļ�ӳ�䣬Ĭ����FALSE���ļ�ӳ�䣩
	* @param llSize : ��������ݵĴ�С
	* @param pfent : ����������ֵ�ı�����ָ��
	* @return 
	*/
	BOOL CheckRsrcEntropy(PUCHAR image_base, BOOL bmem, LONGLONG llSize, double* pfent);

	/**
	* �����������������ԣ��Ǻ��������ǰ�����
	*
	* @param image_base : �������ص��ڴ����ݵ�ַ
	* @param bmem : ���ڴ����ݻ����ļ�ӳ�䣬Ĭ����FALSE���ļ�ӳ�䣩
	* @param llSize : ��������ݵĴ�С
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