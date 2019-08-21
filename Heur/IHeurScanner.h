#pragma once
#include "stdafx.h"

class IHeurScanner
{
public:
	/**
	* ��ʼ����������������ʽ����⣬����ʽ������
	*
	* @return 
	*/
	virtual BOOL Init() = 0;

	/**
	* ��������ʽɨ��
	*
	* @return 
	*/
	virtual BOOL Scan(char*) = 0;

	/**
	* ����ʼ��������ж������ʽ����⣬����ʽ�������������
	*
	* @return 
	*/
	virtual BOOL UnInit() = 0;
	
	/**
	* ���ý������ص�����
	*
	* @return 
	*/
	virtual void SetResultCbFunc(PResultCbFunc pResultCbFunc = NULL) = 0;
};