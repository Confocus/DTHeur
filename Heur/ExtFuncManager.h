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
	* ���ô�ɨ���ļ�
	*
	* @param szPath : ��ɨ���ļ�·��
	* @return 
	*/
	static BOOL SetObj(char* szPath);
	
	static BOOL ReleaseObj();

	/**
	* �Ƿ�ֻ��Rsrc�ڵ�VirtualSize��0
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsOnlyRsrcVirtualSize(EXT_FUNC_PARAM* pParam);
	
	/**
	* �Ƿ�ֻ�������Ľ���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL HasRepeatedSecName(EXT_FUNC_PARAM* pParam);

	/**
	* ���ؿս����ĸ���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetUnprintableSecNameNum(EXT_FUNC_PARAM* pParam);

	/**
	* rdata���Ƿ���п�д����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsRdataWritable(EXT_FUNC_PARAM* pParam);

	/**
	* rdata���Ƿ���п�ִ������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsRdataExe(EXT_FUNC_PARAM* pParam);

	/**
	* �Ƿ��з���ɱ����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsAntiAV(EXT_FUNC_PARAM* pParam);

	/**
	* �Ƿ��в���clipboard������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsClipboardAct(EXT_FUNC_PARAM* pParam);
	
	/**
	* �Ƿ��в������������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsServiceAct(EXT_FUNC_PARAM* pParam);

	/**
	* �Ƿ���Hook����������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsHookAct(EXT_FUNC_PARAM* pParam);

	/**
	* �Ƿ��б������̲���������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsReverseVolumeAct(EXT_FUNC_PARAM* pParam);

	/**
	* �Ƿ��з����Բ���������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsAntiDebug(EXT_FUNC_PARAM* pParam);
	
	/**
	* �Ƿ����ڴ����Բ���������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsVirtualMemOp(EXT_FUNC_PARAM* pParam);

	/**
	* �Ƿ�����Ȩ����������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsPriviledgeOp(EXT_FUNC_PARAM* pParam);
	
	/**
	* �Ƿ����������������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsInternetOp(EXT_FUNC_PARAM* pParam);
	
	/**
	* ��Ҫɨ�������ļ�����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL ScanBuff(EXT_FUNC_PARAM* pParam);

	static BOOL IsFileSizePESizeAbn(EXT_FUNC_PARAM* pParam);

	/**
	* �Ƿ�����������ɨ�����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsNetEnum(EXT_FUNC_PARAM* pParam);

	/**
	* �ж�SizeOfCode�Ĵ�С�Ƿ��쳣
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsErrorSizeOfCode(EXT_FUNC_PARAM* pParam);

	/**
	* �ж�Tls�б��Ƿ�Ϊ��
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsTlsFuncNonNull(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡ������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetSectionNames(EXT_FUNC_PARAM* pParam);

	/**
	* �ж���ڵ��Ƿ�Խ�磬�����κ�һ��������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsEntryPointOutOfSection(EXT_FUNC_PARAM* pParam);

	/**
	* �ж�Tls�б��Ƿ�Ϊ��
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsEntryPointLastSection(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡ��ڵ������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetEntryPointSectionName(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡOptionalHead�Ĵ�С
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetOptionalHeadSize(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡ���뺯���ĸ���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetImpFuncNum(EXT_FUNC_PARAM* pParam);

	/**
	* �ж���ڵ����ڵĽ����Ƿ���п�ִ������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsEntrySecAttrAbn(EXT_FUNC_PARAM* pParam);

	/**
	* �ж��Ƿ��ж���ڴ���exe����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsMultiExeSec(EXT_FUNC_PARAM* pParam);

	/**
	* �ж�У����Ƿ��д���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsCheckSumError(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡRawSize��СΪ0�Ľ�����Ŀ
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetZeroRawSizeSecNum(EXT_FUNC_PARAM* pParam);

	/**
	* ������С�ܺ��Ƿ����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsSumSecSizeError(EXT_FUNC_PARAM* pParam);

	/**
	* ������Ƿ��ƻ�
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsImpCorrupt(EXT_FUNC_PARAM* pParam);

	/**
	* ����������Ƿ��쳣
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsCodeSecAttrAbn(EXT_FUNC_PARAM* pParam);

	/**
	* ��ڵ����ڵĽ�����Raw Addr�Ƿ����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsSecRawAddrAlignErr(EXT_FUNC_PARAM* pParam);

	/**
	* �ж���Դ���Ƿ���п�ִ������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsRsrcExe(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡPointerSymbolToTable��ֵ
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetPointSymTab(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡNumberOfSymbols��ֵ
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetSymNum(EXT_FUNC_PARAM* pParam);

	/**
	* �ж��Ƿ�ӿ�
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsPacked(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡ��ڵ����ڽ�������ֵ
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetEntryEntropy(EXT_FUNC_PARAM* pParam);

	/**
	* ���������ļ�����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetFileEntropy(EXT_FUNC_PARAM* pParam);

	/**
	* ������Դ�ڵ��أ�����ж����Դ�ڣ�ȡ�����Ǹ���ֵ
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetRsrcEntropy(EXT_FUNC_PARAM* pParam);

	/**
	* �ж�RawSizeֵ�Ĵ�С�Ƿ����쳣
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsRawSizeAbn(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡ����ֵ�Ŀ�ı���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetHighEntropyBlockRatio(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡ����ֵ�Ľ����ı���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetHighEntropySectionRatio(EXT_FUNC_PARAM* pParam);
	
	/**
	* ��ȡ����������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetSecCnt(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡ�߱���ִ�����ԵĽ����ı���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetExeSecRatio(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡʱ���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetTimeStamp(EXT_FUNC_PARAM* pParam);
	
	/**
	* ��ȡDataDirectory��СΪ0�ĸ���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetZeroDataDirNum(EXT_FUNC_PARAM* pParam);

	/**
	* ��ȡDataDirectory��ַ�쳣�ĸ���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetInvalidDataDirNum(EXT_FUNC_PARAM* pParam);

	/**
	* �Ƿ����еĽ������߱�����ִ������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsAllUnexeSec(EXT_FUNC_PARAM* pParam);

	/**
	* �õ�DOSͷ�е�e_cblp����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetByteLastPage(EXT_FUNC_PARAM* pParam);

	/**
	* �õ�DOSͷ�е�e_cp����
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetPageInFile(EXT_FUNC_PARAM* pParam);

	/**
	* �õ�IAT������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetIATNum(EXT_FUNC_PARAM* pParam); 

	/**
	* ��ڵ��Ƿ����ض�λ��
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsEntryReloc(EXT_FUNC_PARAM* pParam);

	/**
	* ��ڵ��Ƿ�����Դ��
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL IsEntryRsrc(EXT_FUNC_PARAM* pParam);
	
	/**
	* ��ȡ���һ������������
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL GetLastSecName(EXT_FUNC_PARAM* pParam);

	/**
	* �ṩһЩ�ͷŲ���
	*
	* @param pParam : �����õ��Ľ��
	* @return 
	*/
	static BOOL Release();

private:
	static BOOL GetPEBuff(EXT_FUNC_PARAM* pParam);
	static void BuffToLower(char*, LONGLONG);

private:
	static CPEParser* m_pPEParser;//todo �����Ƿ�Ҫ��Ϊ������Ϊһ��������������и����������ڻ����Ը�Ϊ���߳�ɨ�衣
	static int nToLowerFlag;
};