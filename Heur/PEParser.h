#pragma once
#include "stdafx.h"
#include "PeObj.h"
#include "PEMarco.h"
#include "MemoryManager.h"
#include "SafeArray.h"

class CPEParser
{
public:
	CPEParser(void);
	~CPEParser(void);

public:
	/**
	* ���ô�ɨ�����ĸ�������
	*
	* @return 
	*/
	BOOL SetObj(char* szPath);

	/**
	* �ͷ�ɨ�����
	*
	* @return 
	*/
	BOOL ReleaseObj();

	BOOL IsFileSizePESizeAbn();

	/**
	* �õ�ָ��PEȫ�����ݵ�ָ��
	*
	* @return 
	*/
	BOOL GetPEBuf(char** ppBuf, LONGLONG* pllSize);

	/**
	* rdata���Ƿ���п�ִ������
	*
	* @return 
	*/
	BOOL IsRdataExe();

	/**
	* rdata���Ƿ���п�д����
	*
	* @return 
	*/
	BOOL IsRdataWritable();

	/**
	* ���ؿս����ĸ���
	*
	* @return 
	*/
	int GetUnprintableSecNameNum();

	/**
	* �Ƿ�ֻ�������Ľ���
	*
	* @return 
	*/
	BOOL HasRepeatedSecName();

	/**
	* �Ƿ�ֻ��Rsrc�ڵ�VirtualSize��0
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	BOOL IsOnlyRsrcVirtualSize();

	/**
	* ��ȡ���һ������������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	BOOL GetLastSecName(char*, int);

	/**
	* ��ڵ��Ƿ����ض�λ��
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	BOOL IsEntryReloc();

	/**
	* ��ڵ��Ƿ�����Դ��
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	BOOL IsEntryRsrc();

	/**
	* �õ�IAT������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	int GetIATNum();

	/**
	* �õ�DOSͷ�е�e_cp����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	int GetPageInFile();

	/**
	* �õ�DOSͷ�е�e_cblp����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	int GetByteLastPage();

	/**
	* ��ȡDataDirectory��ַ�쳣�ĸ���
	*
	* @return 
	*/
	int GetInvalidDataDirNum();

	/**
	* ��ȡDataDirectory�д�СΪ0�ĸ���
	*
	* @return 
	*/
	int GetZeroDataDirNum();

	/**
	* �Ƿ����еĽ������߱�����ִ������
	*
	* @return 
	*/
	BOOL IsAllUnexeSec();

	/**
	* ��ȡʱ���
	*
	* @return 
	*/
	DWORD GetTimeStamp();

	/**
	* ��ȡ�߱���ִ�����Խ����ı���
	*
	* @return 
	*/
	double GetExeSecRatio();

	/**
	* ��ȡ����������
	*
	* @return 
	*/
	int GetSecCnt();

	/**
	* ��ȡ����ֵ�Ŀ�ı���
	*
	* @return 
	*/
	double GetHighEntropyBlockRatio();

	/**
	* ��ȡ����ֵ�Ľ����ı���
	*
	* @return 
	*/
	double GetHighEntropySectionRatio();

	/**
	* RawSize�쳣�Ľ����ĸ���
	*
	* @return 
	*/
	int GetRawSizeAbnNum();

	/**
	* �ж�RawSize�ֶ�ֵ�Ƿ��쳣
	*
	* @return 
	*/
	BOOL IsRawSizeAbn();

	/**
	* ��ȡ��Դ�ڵ���ֵ
	*
	* @return 
	*/
	double GetRsrcEntropy();

	/**
	* ��ȡ�����ļ�����ֵ
	*
	* @return 
	*/
	double GetFileEntropy();

	/**
	* ��ȡ��ڵ����ڽ�������ֵ
	*
	* @return 
	*/
	double GetEntryEntropy();

	/**
	* �ж��Ƿ�ӿ�
	*
	* @return 
	*/
	BOOL IsPacked();

	/**
	* ����������Ƿ��쳣
	*
	* @return 
	*/
	BOOL IsCodeSecAttrAbn();

	/**
	* ������Ƿ��쳣
	*
	* @return 
	*/
	BOOL IsImpCorrupt();

	/**
	* �õ������ƴ�
	*
	* @return 
	*/
	BOOL GetSectionNames(char *szSecName, int nSize);//���ɵĴ��������

	/**
	* ��ڵ�ָ��ĵ�ַ�����κ�һ������
	*
	* @return 
	*/
	BOOL IsEntryPointOutOfSection();

	/**
	* ��ڵ��Ƿ������һ������
	*
	* @return 
	*/
	BOOL IsEntryPointLastSection();//��ڵ�ָ�����һ������

	/**
	* ��ȡ��ڵ����ڵĽ�������
	*
	* @return 
	*/
	BOOL GetEntryPointSectionName(char* szSecName);//��ڵ�ָ��Ľ��������Ʋ��ǡ�CODE����".text"

	/**
	* ��ȡ��ѡͷ�Ĵ�С
	*
	* @return 
	*/
	int GetOptionalHeadSize();//�����SizeOfOptionalHeader

	/**
	* ��ȡ���뺯���ĸ���
	*
	* @return 
	*/
	BOOL GetImpFuncNum();

	/**
	* �ж�Tls���Ƿ�Ϊ��
	*
	* @return 
	*/
	BOOL IsTlsFuncNull(PBOOL pRes);//Tls�ص������б��Ƿ�Ϊ��

	/**
	* SizeOfCode��ֵ�Ƿ���ȷ
	*
	* @return 
	*/
	BOOL IsErrorSizeOfCode(PBOOL pRes);//��ѡͷ����SizeOfCode��ʵ�ʴ��볤�Ȳ���

	/**
	* ��ڵ�ָ��Ľ�����û�С���ִ�С�����
	*
	* @param bEntryExe : �����Ƿ���С���ִ�С�����
	* @return 
	*/
	BOOL IsEntrySecAttrAbn(BOOL* bEntryExe);

	/**
	* �Ƿ��ж���ھ߱���ִ������
	*
	* @return ���ؽ�����Ŀ
	*/
	int GetExeSecNum();

	/**
	* �ж�У��ֵ�Ƿ����
	*
	* @return 
	*/
	BOOL IsCheckSumError();

	/**
	* ����RawSizeΪ0�Ľڵĸ���
	*
	* @return 
	*/
	int GetZeroRawSizeSecNum();

	/**
	* ����RawSizeΪ0�Ľڵĸ���
	*
	* @return 
	*/
	BOOL IsSumSecSizeError();
	
	/**
	* ��ڵ����ڵĽ�����Raw Addr�Ƿ����
	*
	* @return 
	*/
	BOOL IsSecRawAddrAlignErr();

	/**
	* �ж���Դ���Ƿ���п�ִ������
	*
	* @return 
	*/
	BOOL IsRsrcExe();

	/**
	* ��ȡPointerSymbolOfTable��ֵ
	*
	* @return 
	*/
	int GetPointSymTab();

	/**
	* ��ȡNumberOfSymbols��ֵ
	*
	* @return 
	*/
	int GetSymNum();

private:
	LONGLONG GetPESize(FILE*);//todo �����ط��費��Ҫ��Ϊlonglong

private:
	//todo ���ڸĳɹ���CPeObj���б�ɣ��и���������Ҳ���Ա��ڶ��߳�ɨ��
	CPeObj* m_pPEObj;
	CMemoryManager* m_pMemoryManager;
	int nImpDllNum;
	int nImpFuncNum;
	CSafeArray<char>* m_pPEFileBuf;
};
