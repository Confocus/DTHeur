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
	* 设置待扫描对象的各个属性
	*
	* @return 
	*/
	BOOL SetObj(char* szPath);

	/**
	* 释放扫描对象
	*
	* @return 
	*/
	BOOL ReleaseObj();

	BOOL IsFileSizePESizeAbn();

	/**
	* 拿到指向PE全部内容的指针
	*
	* @return 
	*/
	BOOL GetPEBuf(char** ppBuf, LONGLONG* pllSize);

	/**
	* rdata节是否具有可执行属性
	*
	* @return 
	*/
	BOOL IsRdataExe();

	/**
	* rdata节是否具有可写属性
	*
	* @return 
	*/
	BOOL IsRdataWritable();

	/**
	* 返回空节名的个数
	*
	* @return 
	*/
	int GetUnprintableSecNameNum();

	/**
	* 是否只有重名的节区
	*
	* @return 
	*/
	BOOL HasRepeatedSecName();

	/**
	* 是否只有Rsrc节的VirtualSize非0
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	BOOL IsOnlyRsrcVirtualSize();

	/**
	* 获取最后一个节区的名称
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	BOOL GetLastSecName(char*, int);

	/**
	* 入口点是否在重定位节
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	BOOL IsEntryReloc();

	/**
	* 入口点是否在资源节
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	BOOL IsEntryRsrc();

	/**
	* 拿到IAT的数量
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	int GetIATNum();

	/**
	* 拿到DOS头中的e_cp数据
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	int GetPageInFile();

	/**
	* 拿到DOS头中的e_cblp数据
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	int GetByteLastPage();

	/**
	* 获取DataDirectory地址异常的个数
	*
	* @return 
	*/
	int GetInvalidDataDirNum();

	/**
	* 获取DataDirectory中大小为0的个数
	*
	* @return 
	*/
	int GetZeroDataDirNum();

	/**
	* 是否所有的节区都具备不可执行属性
	*
	* @return 
	*/
	BOOL IsAllUnexeSec();

	/**
	* 获取时间戳
	*
	* @return 
	*/
	DWORD GetTimeStamp();

	/**
	* 获取具备可执行属性节区的比率
	*
	* @return 
	*/
	double GetExeSecRatio();

	/**
	* 获取节区的数量
	*
	* @return 
	*/
	int GetSecCnt();

	/**
	* 获取高熵值的块的比率
	*
	* @return 
	*/
	double GetHighEntropyBlockRatio();

	/**
	* 获取高熵值的节区的比率
	*
	* @return 
	*/
	double GetHighEntropySectionRatio();

	/**
	* RawSize异常的节区的个数
	*
	* @return 
	*/
	int GetRawSizeAbnNum();

	/**
	* 判断RawSize字段值是否异常
	*
	* @return 
	*/
	BOOL IsRawSizeAbn();

	/**
	* 获取资源节的熵值
	*
	* @return 
	*/
	double GetRsrcEntropy();

	/**
	* 获取整个文件的熵值
	*
	* @return 
	*/
	double GetFileEntropy();

	/**
	* 获取入口点所在节区的熵值
	*
	* @return 
	*/
	double GetEntryEntropy();

	/**
	* 判断是否加壳
	*
	* @return 
	*/
	BOOL IsPacked();

	/**
	* 代码段属性是否异常
	*
	* @return 
	*/
	BOOL IsCodeSecAttrAbn();

	/**
	* 导入表是否异常
	*
	* @return 
	*/
	BOOL IsImpCorrupt();

	/**
	* 拿到节名称串
	*
	* @return 
	*/
	BOOL GetSectionNames(char *szSecName, int nSize);//可疑的代码节名称

	/**
	* 入口点指向的地址不在任何一个节区
	*
	* @return 
	*/
	BOOL IsEntryPointOutOfSection();

	/**
	* 入口点是否在最后一个节中
	*
	* @return 
	*/
	BOOL IsEntryPointLastSection();//入口点指向最后一个节区

	/**
	* 获取入口点所在的节区名字
	*
	* @return 
	*/
	BOOL GetEntryPointSectionName(char* szSecName);//入口点指向的节区的名称不是“CODE”或".text"

	/**
	* 获取可选头的大小
	*
	* @return 
	*/
	int GetOptionalHeadSize();//过大的SizeOfOptionalHeader

	/**
	* 获取导入函数的个数
	*
	* @return 
	*/
	BOOL GetImpFuncNum();

	/**
	* 判断Tls表是否为空
	*
	* @return 
	*/
	BOOL IsTlsFuncNull(PBOOL pRes);//Tls回调函数列表是否为空

	/**
	* SizeOfCode的值是否正确
	*
	* @return 
	*/
	BOOL IsErrorSizeOfCode(PBOOL pRes);//可选头部的SizeOfCode与实际代码长度不符

	/**
	* 入口点指向的节区并没有“可执行”属性
	*
	* @param bEntryExe : 返回是否具有“可执行”属性
	* @return 
	*/
	BOOL IsEntrySecAttrAbn(BOOL* bEntryExe);

	/**
	* 是否有多个节具备可执行属性
	*
	* @return 返回节区数目
	*/
	int GetExeSecNum();

	/**
	* 判断校验值是否错误
	*
	* @return 
	*/
	BOOL IsCheckSumError();

	/**
	* 返回RawSize为0的节的个数
	*
	* @return 
	*/
	int GetZeroRawSizeSecNum();

	/**
	* 返回RawSize为0的节的个数
	*
	* @return 
	*/
	BOOL IsSumSecSizeError();
	
	/**
	* 入口点所在的节区的Raw Addr是否对齐
	*
	* @return 
	*/
	BOOL IsSecRawAddrAlignErr();

	/**
	* 判断资源节是否具有可执行属性
	*
	* @return 
	*/
	BOOL IsRsrcExe();

	/**
	* 获取PointerSymbolOfTable的值
	*
	* @return 
	*/
	int GetPointSymTab();

	/**
	* 获取NumberOfSymbols的值
	*
	* @return 
	*/
	int GetSymNum();

private:
	LONGLONG GetPESize(FILE*);//todo 其它地方需不需要改为longlong

private:
	//todo 后期改成构建CPeObj的列表吧，有个缓冲区，也可以便于多线程扫描
	CPeObj* m_pPEObj;
	CMemoryManager* m_pMemoryManager;
	int nImpDllNum;
	int nImpFuncNum;
	CSafeArray<char>* m_pPEFileBuf;
};
