#include "stdafx.h"
#include "PEParser.h"
#include "ExtFuncManager.h"

ExtFuncManager::ExtFuncManager()
{
}

ExtFuncManager::~ExtFuncManager()
{
}

BOOL ExtFuncManager::IsErrorSizeOfCode(EXT_FUNC_PARAM* pParam)
{
	//printf("IsCorrectSizeOfCode..\n");
	BOOL bRes = FALSE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->IsErrorSizeOfCode(&bRes));
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = bRes;

	return TRUE;
}

BOOL ExtFuncManager::IsTlsFuncNonNull(EXT_FUNC_PARAM* pParam)
{
	//printf("IsTlsFuncNonNull..\n");
	BOOL bRes = FALSE;
	if(!m_pPEParser->IsTlsFuncNull(&bRes))
	{
		return FALSE;
	}
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = !bRes;//返回是否为空，我们检查的是它不是空

	return TRUE;
}

BOOL ExtFuncManager::GetSectionNames(EXT_FUNC_PARAM* pParam)
{
	//printf("GetSectionNames..\n");
	//出现过某些样本由于节区数过多，导致数组越界，覆盖了其它数据的情况
	pParam->OutParam.nType = TYPE_ARRAY_VALUE;
	if(!m_pPEParser->GetSectionNames(pParam->OutParam.szResult, HR_SIGN_SIZE))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL ExtFuncManager::IsEntryPointOutOfSection(EXT_FUNC_PARAM* pParam)
{
	//printf("IsEntryPointOutOfSection..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsEntryPointOutOfSection();
	return TRUE;
}

BOOL ExtFuncManager::IsEntryPointLastSection(EXT_FUNC_PARAM* pParam)
{
	//printf("IsEntryPointLastSection..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsEntryPointLastSection();
	return TRUE;
}

BOOL ExtFuncManager::GetEntryPointSectionName(EXT_FUNC_PARAM* pParam)
{
	//printf("GetEntryPointSectionName..\n");
	pParam->OutParam.nType = TYPE_ARRAY_VALUE;
	if(!m_pPEParser->GetEntryPointSectionName(pParam->OutParam.szResult))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL ExtFuncManager::GetOptionalHeadSize(EXT_FUNC_PARAM* pParam)
{
	//printf("GetOptionalHeadSize..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetOptionalHeadSize();
	return TRUE;
}

BOOL ExtFuncManager::GetImpFuncNum(EXT_FUNC_PARAM* pParam)
{
	//printf("GetImpFuncNum..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetImpFuncNum();
	return TRUE;
}

BOOL ExtFuncManager::IsEntrySecAttrAbn(EXT_FUNC_PARAM* pParam)
{
	//printf("IsEntrySecAttrAbn..\n");
	BOOL bRes = FALSE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->IsEntrySecAttrAbn(&bRes));
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = bRes;
	return TRUE;
}

BOOL ExtFuncManager::IsMultiExeSec(EXT_FUNC_PARAM* pParam)
{
	//printf("GetExeSecNum..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetExeSecNum();
	return TRUE;
}

BOOL ExtFuncManager::SetObj(char* szPath)
{
	RETURN_FALSE_IF_FAILED(m_pPEParser->SetObj(szPath));

	return TRUE;
}

BOOL ExtFuncManager::IsCheckSumError(EXT_FUNC_PARAM* pParam)
{
	//printf("IsCheckSumError..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsCheckSumError();
	return TRUE;
}

BOOL ExtFuncManager::GetZeroRawSizeSecNum(EXT_FUNC_PARAM* pParam)
{
	//printf("GetZeroRawSizeSecNum..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetZeroRawSizeSecNum();
	return TRUE;
}

BOOL ExtFuncManager::IsSumSecSizeError(EXT_FUNC_PARAM* pParam)
{
	//printf("IsSumSecSizeError..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsSumSecSizeError();
	return TRUE;
}

BOOL ExtFuncManager::Release()
{
	//m_pPEParser->ReleaseObj();
	SAFE_DELETE_POINTER(m_pPEParser);

	return TRUE;
}

BOOL ExtFuncManager::IsImpCorrupt(EXT_FUNC_PARAM* pParam)
{
	//printf("IsImpCorrupt..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsImpCorrupt();
	return TRUE;
}

BOOL ExtFuncManager::IsCodeSecAttrAbn(EXT_FUNC_PARAM* pParam)
{
	//printf("IsCodeSecAttrAbn..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsCodeSecAttrAbn();
	return TRUE;
}

BOOL ExtFuncManager::IsSecRawAddrAlignErr(EXT_FUNC_PARAM* pParam)
{
	//printf("IsSecRawAddrAlignErr..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsSecRawAddrAlignErr();
	return TRUE;
}

BOOL ExtFuncManager::IsRsrcExe(EXT_FUNC_PARAM* pParam)
{
	//printf("IsRsrcExe..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsRsrcExe();
	return TRUE;
}

BOOL ExtFuncManager::GetPointSymTab(EXT_FUNC_PARAM* pParam)
{
	//printf("GetPointSymTab..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetPointSymTab();
	return TRUE;
}

BOOL ExtFuncManager::GetSymNum(EXT_FUNC_PARAM* pParam)
{
	//printf("GetSymNum..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetSymNum();
	return TRUE;
}

BOOL ExtFuncManager::IsPacked(EXT_FUNC_PARAM* pParam)
{
	//printf("IsPacked..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsPacked();
	return TRUE;
}

BOOL ExtFuncManager::GetEntryEntropy(EXT_FUNC_PARAM* pParam)
{
	//printf("GetEntryEntropy..\n");
	pParam->OutParam.nType = TYPE_DOUBLE_VALUE;
	pParam->OutParam.dResult = m_pPEParser->GetEntryEntropy();//这里改成dResult赋值
	if(0.0 == pParam->OutParam.dResult)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL ExtFuncManager::GetFileEntropy(EXT_FUNC_PARAM* pParam)
{
	//printf("GetFileEntropy..\n");
	pParam->OutParam.nType = TYPE_DOUBLE_VALUE;
	pParam->OutParam.dResult = m_pPEParser->GetFileEntropy();//这里改成dResult赋值

	if(fabs(pParam->OutParam.dResult) < DOUBLE_DIFF_VALUE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL ExtFuncManager::GetRsrcEntropy(EXT_FUNC_PARAM* pParam)
{
	//DEBUG_PRINT("GetRsrcEntropy..\n");
	pParam->OutParam.nType = TYPE_DOUBLE_VALUE;
	pParam->OutParam.dResult = m_pPEParser->GetRsrcEntropy();//这里改成dResult赋值
	if(0.0 == pParam->OutParam.dResult)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL ExtFuncManager::IsRawSizeAbn(EXT_FUNC_PARAM* pParam)
{
	//printf("IsRawSizeAbn..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsRawSizeAbn();
	pParam->OutParam.nNum = m_pPEParser->GetRawSizeAbnNum();
	return TRUE;
}

BOOL ExtFuncManager::GetHighEntropyBlockRatio(EXT_FUNC_PARAM* pParam)
{
	//printf("GetHighEntropyBlockRatio..\n");
	pParam->OutParam.nType = TYPE_DOUBLE_VALUE;
	pParam->OutParam.dResult = m_pPEParser->GetHighEntropyBlockRatio();
	return TRUE;
}

BOOL ExtFuncManager::GetHighEntropySectionRatio(EXT_FUNC_PARAM* pParam)
{
	//printf("GetHighEntropySectionRatio..\n");
	pParam->OutParam.nType = TYPE_DOUBLE_VALUE;
	pParam->OutParam.dResult = m_pPEParser->GetHighEntropySectionRatio();
	return TRUE;
}

BOOL ExtFuncManager::GetSecCnt(EXT_FUNC_PARAM* pParam)
{
	//printf("GetSecCnt..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetSecCnt();
	return TRUE;
}

BOOL ExtFuncManager::GetExeSecRatio(EXT_FUNC_PARAM* pParam)
{
	//printf("GetExeSecRatio..\n");
	pParam->OutParam.nType = TYPE_DOUBLE_VALUE;
	pParam->OutParam.dResult = m_pPEParser->GetExeSecRatio();
	return TRUE;
}

BOOL ExtFuncManager::GetTimeStamp(EXT_FUNC_PARAM* pParam)
{
	//printf("GetTimeStamp..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetTimeStamp();
	return TRUE;
}

BOOL ExtFuncManager::IsAllUnexeSec(EXT_FUNC_PARAM* pParam)
{
	//printf("IsAllUnexeSec..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsAllUnexeSec();
	return TRUE;
}

BOOL ExtFuncManager::GetZeroDataDirNum(EXT_FUNC_PARAM* pParam)
{
	//printf("GetZeroDataDirNum..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetZeroDataDirNum();
	return TRUE;
}

BOOL ExtFuncManager::GetInvalidDataDirNum(EXT_FUNC_PARAM* pParam)
{
	//printf("GetInvalidDataDirNum..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetInvalidDataDirNum();
	return TRUE;
}

BOOL ExtFuncManager::GetByteLastPage(EXT_FUNC_PARAM* pParam)
{
	//printf("GetByteLastPage..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetByteLastPage();
	return TRUE;
}

BOOL ExtFuncManager::GetPageInFile(EXT_FUNC_PARAM* pParam)
{
	//printf("GetPageInFile..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetPageInFile();
	return TRUE;
}

BOOL ExtFuncManager::GetIATNum(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetIATNum();
	return TRUE;
}

BOOL ExtFuncManager::IsEntryReloc(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsEntryReloc();
	return TRUE;
}

BOOL ExtFuncManager::IsEntryRsrc(EXT_FUNC_PARAM* pParam)
{
	//printf("IsEntryRsrc..\n");
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsEntryRsrc();
	return TRUE;
}

BOOL ExtFuncManager::GetLastSecName(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_ARRAY_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetLastSecName(pParam->OutParam.szResult, MAX_SEC_NAME_SIZE));

	return TRUE;
}

BOOL ExtFuncManager::IsOnlyRsrcVirtualSize(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsOnlyRsrcVirtualSize();
	return TRUE;
}

BOOL ExtFuncManager::HasRepeatedSecName(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->HasRepeatedSecName();
	return TRUE;
}

BOOL ExtFuncManager::GetUnprintableSecNameNum(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->GetUnprintableSecNameNum();
	return TRUE;
}

BOOL ExtFuncManager::IsRdataWritable(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsRdataWritable();
	return TRUE;
}

BOOL ExtFuncManager::IsRdataExe(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsRdataExe();
	return TRUE;
}

BOOL ExtFuncManager::IsAntiAV(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}

BOOL ExtFuncManager::IsClipboardAct(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}


BOOL ExtFuncManager::IsServiceAct(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}


BOOL ExtFuncManager::IsHookAct(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}

BOOL ExtFuncManager::IsReverseVolumeAct(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}

BOOL ExtFuncManager::IsAntiDebug(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}

BOOL ExtFuncManager::IsVirtualMemOp(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}

BOOL ExtFuncManager::IsPriviledgeOp(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}

BOOL ExtFuncManager::IsInternetOp(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}


BOOL ExtFuncManager::IsNetEnum(EXT_FUNC_PARAM* pParam)
{
	return GetPEBuff(pParam);
}

BOOL ExtFuncManager::ScanBuff(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}

BOOL ExtFuncManager::ReleaseObj()
{
	nToLowerFlag = FALSE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->ReleaseObj());

	return TRUE;
}

void ExtFuncManager::BuffToLower(char* ptr, LONGLONG llSize)
{
	if(nToLowerFlag)//不必每次都对内存进行ToLower
	{
		return;
	}

	char* p = ptr;
	for(int i = 0; i < llSize; i++)
	{
		*p = tolower(*p);
		p++;
	}

	nToLowerFlag = TRUE;
}

BOOL ExtFuncManager::GetPEBuff(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_POINTER_VALUE;
	RETURN_FALSE_IF_FAILED(m_pPEParser->GetPEBuf(&pParam->OutParam.BufResult.ptr, &pParam->OutParam.BufResult.len));
	RETURN_FALSE_IF_FAILED(pParam->OutParam.BufResult.len);
	BuffToLower(pParam->OutParam.BufResult.ptr, pParam->OutParam.BufResult.len);

	return TRUE;
}

BOOL ExtFuncManager::IsFileSizePESizeAbn(EXT_FUNC_PARAM* pParam)
{
	pParam->OutParam.nType = TYPE_INT_VALUE;
	pParam->OutParam.nResult = m_pPEParser->IsFileSizePESizeAbn();
	return TRUE;
}

CPEParser* ExtFuncManager::m_pPEParser = new CPEParser();
int ExtFuncManager::nToLowerFlag = FALSE;
