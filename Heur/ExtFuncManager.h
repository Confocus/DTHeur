#pragma once
#include "stdafx.h"
#include "HeurRuleMacro.h"
#include "PEParser.h"

class ExtFuncManager
{
public:
	ExtFuncManager();
	~ExtFuncManager();

public:

	/**
	* 设置待扫描文件
	*
	* @param szPath : 待扫描文件路径
	* @return 
	*/
	static BOOL SetObj(char* szPath);
	
	static BOOL ReleaseObj();

	/**
	* 是否只有Rsrc节的VirtualSize非0
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsOnlyRsrcVirtualSize(EXT_FUNC_PARAM* pParam);
	
	/**
	* 是否只有重名的节区
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL HasRepeatedSecName(EXT_FUNC_PARAM* pParam);

	/**
	* 返回空节名的个数
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetUnprintableSecNameNum(EXT_FUNC_PARAM* pParam);

	/**
	* rdata节是否具有可写属性
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsRdataWritable(EXT_FUNC_PARAM* pParam);

	/**
	* rdata节是否具有可执行属性
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsRdataExe(EXT_FUNC_PARAM* pParam);

	/**
	* 是否有反查杀特征
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsAntiAV(EXT_FUNC_PARAM* pParam);

	/**
	* 是否有操作clipboard的嫌疑
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsClipboardAct(EXT_FUNC_PARAM* pParam);
	
	/**
	* 是否有操作服务的嫌疑
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsServiceAct(EXT_FUNC_PARAM* pParam);

	/**
	* 是否有Hook操作的嫌疑
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsHookAct(EXT_FUNC_PARAM* pParam);

	/**
	* 是否有遍历磁盘操作的嫌疑
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsReverseVolumeAct(EXT_FUNC_PARAM* pParam);

	/**
	* 是否有反调试操作的嫌疑
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsAntiDebug(EXT_FUNC_PARAM* pParam);
	
	/**
	* 是否有内存属性操作的嫌疑
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsVirtualMemOp(EXT_FUNC_PARAM* pParam);

	/**
	* 是否有提权操作的嫌疑
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsPriviledgeOp(EXT_FUNC_PARAM* pParam);
	
	/**
	* 是否有网络操作的嫌疑
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsInternetOp(EXT_FUNC_PARAM* pParam);
	
	/**
	* 需要扫描整个文件内容
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL ScanBuff(EXT_FUNC_PARAM* pParam);

	static BOOL IsFileSizePESizeAbn(EXT_FUNC_PARAM* pParam);

	/**
	* 是否有疑似内网扫描操作
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsNetEnum(EXT_FUNC_PARAM* pParam);

	/**
	* 判断SizeOfCode的大小是否异常
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsErrorSizeOfCode(EXT_FUNC_PARAM* pParam);

	/**
	* 判断Tls列表是否为空
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsTlsFuncNonNull(EXT_FUNC_PARAM* pParam);

	/**
	* 获取节区名
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetSectionNames(EXT_FUNC_PARAM* pParam);

	/**
	* 判断入口点是否越界，不在任何一个节区内
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsEntryPointOutOfSection(EXT_FUNC_PARAM* pParam);

	/**
	* 判断Tls列表是否为空
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsEntryPointLastSection(EXT_FUNC_PARAM* pParam);

	/**
	* 获取入口点节区名
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetEntryPointSectionName(EXT_FUNC_PARAM* pParam);

	/**
	* 获取OptionalHead的大小
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetOptionalHeadSize(EXT_FUNC_PARAM* pParam);

	/**
	* 获取导入函数的个数
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetImpFuncNum(EXT_FUNC_PARAM* pParam);

	/**
	* 判断入口点所在的节区是否具有可执行属性
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsEntrySecAttrAbn(EXT_FUNC_PARAM* pParam);

	/**
	* 判断是否有多个节存在exe属性
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsMultiExeSec(EXT_FUNC_PARAM* pParam);

	/**
	* 判断校验和是否有错误
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsCheckSumError(EXT_FUNC_PARAM* pParam);

	/**
	* 获取RawSize大小为0的节区数目
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetZeroRawSizeSecNum(EXT_FUNC_PARAM* pParam);

	/**
	* 节区大小总和是否错误
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsSumSecSizeError(EXT_FUNC_PARAM* pParam);

	/**
	* 导入表是否被破坏
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsImpCorrupt(EXT_FUNC_PARAM* pParam);

	/**
	* 代码段属性是否异常
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsCodeSecAttrAbn(EXT_FUNC_PARAM* pParam);

	/**
	* 入口点所在的节区的Raw Addr是否对齐
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsSecRawAddrAlignErr(EXT_FUNC_PARAM* pParam);

	/**
	* 判断资源节是否具有可执行属性
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsRsrcExe(EXT_FUNC_PARAM* pParam);

	/**
	* 获取PointerSymbolToTable的值
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetPointSymTab(EXT_FUNC_PARAM* pParam);

	/**
	* 获取NumberOfSymbols的值
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetSymNum(EXT_FUNC_PARAM* pParam);

	/**
	* 判断是否加壳
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsPacked(EXT_FUNC_PARAM* pParam);

	/**
	* 获取入口点所在节区的熵值
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetEntryEntropy(EXT_FUNC_PARAM* pParam);

	/**
	* 计算整个文件的熵
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetFileEntropy(EXT_FUNC_PARAM* pParam);

	/**
	* 计算资源节的熵，如果有多个资源节，取最大的那个熵值
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetRsrcEntropy(EXT_FUNC_PARAM* pParam);

	/**
	* 判断RawSize值的大小是否有异常
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsRawSizeAbn(EXT_FUNC_PARAM* pParam);

	/**
	* 获取高熵值的块的比率
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetHighEntropyBlockRatio(EXT_FUNC_PARAM* pParam);

	/**
	* 获取高熵值的节区的比率
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetHighEntropySectionRatio(EXT_FUNC_PARAM* pParam);
	
	/**
	* 获取节区的数量
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetSecCnt(EXT_FUNC_PARAM* pParam);

	/**
	* 获取具备可执行属性的节区的比率
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetExeSecRatio(EXT_FUNC_PARAM* pParam);

	/**
	* 获取时间戳
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetTimeStamp(EXT_FUNC_PARAM* pParam);
	
	/**
	* 获取DataDirectory大小为0的个数
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetZeroDataDirNum(EXT_FUNC_PARAM* pParam);

	/**
	* 获取DataDirectory地址异常的个数
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetInvalidDataDirNum(EXT_FUNC_PARAM* pParam);

	/**
	* 是否所有的节区都具备不可执行属性
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsAllUnexeSec(EXT_FUNC_PARAM* pParam);

	/**
	* 拿到DOS头中的e_cblp数据
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetByteLastPage(EXT_FUNC_PARAM* pParam);

	/**
	* 拿到DOS头中的e_cp数据
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetPageInFile(EXT_FUNC_PARAM* pParam);

	/**
	* 拿到IAT的数量
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetIATNum(EXT_FUNC_PARAM* pParam); 

	/**
	* 入口点是否在重定位节
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsEntryReloc(EXT_FUNC_PARAM* pParam);

	/**
	* 入口点是否在资源节
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL IsEntryRsrc(EXT_FUNC_PARAM* pParam);
	
	/**
	* 获取最后一个节区的名称
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL GetLastSecName(EXT_FUNC_PARAM* pParam);

	/**
	* 提供一些释放操作
	*
	* @param pParam : 返回拿到的结果
	* @return 
	*/
	static BOOL Release();

private:
	static BOOL GetPEBuff(EXT_FUNC_PARAM* pParam);
	static void BuffToLower(char*, LONGLONG);

private:
	static CPEParser* m_pPEParser;//todo 这里是否要改为链表，作为一个缓冲区。如果有个缓冲区后期还可以改为多线程扫描。
	static int nToLowerFlag;
};