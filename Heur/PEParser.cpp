#include "PEParser.h"
#include "HeurRuleMacro.h"
#include "SafeArray.h"

CPEParser::CPEParser(void):
m_pPEObj(new CPeObj()),//todo smartptr
m_pMemoryManager(new CMemoryManager()),//todo smartptr
nImpDllNum(0),
nImpFuncNum(0),
m_pPEFileBuf(NULL)
{
}

CPEParser::~CPEParser(void)
{
	SAFE_DELETE_POINTER(m_pMemoryManager);
	SAFE_DELETE_POINTER(m_pPEObj);
}

int CPEParser::GetExeSecNum()
{
	return m_pPEObj->GetExeSecNum();
}

BOOL CPEParser::IsEntrySecAttrAbn(BOOL* pRes)
{
	IMAGE_SECTION_HEADER* szSecHead = NULL;
	RETURN_FALSE_IF_FAILED((szSecHead = m_pPEObj->GetSecHdr()));

	int nIndex = m_pPEObj->GetEntryOfSectionIndex();
	if(INVALID_RETURN_VALUE == nIndex)
	{
		return FALSE;
	}
	
	if((szSecHead[nIndex]).Characteristics == 0x60000020)
	{
		*pRes = FALSE;
	}
	else
	{
		*pRes = TRUE;
	}

	return TRUE;
}

BOOL CPEParser::IsErrorSizeOfCode(PBOOL pRes)
{
	printf("IsErrorSizeOfCode\n");
	IMAGE_SECTION_HEADER* szSecHead = NULL;
	if(!(szSecHead = m_pPEObj->GetSecHdr()))
	{
		return FALSE;
	}

	if(-1 == m_pPEObj->GetEntryOfSectionIndex())
	{
		return FALSE;
	}
	//入口点所在的节的大小与SizeOfCode进行比较
	*pRes = (m_pPEObj->GetSizeOfCode() != (szSecHead[m_pPEObj->GetEntryOfSectionIndex()]).SizeOfRawData);

	return TRUE;
}

BOOL CPEParser::IsTlsFuncNull(PBOOL pRes)
{
	//通过TLS目录大小是否为0判断
	IMAGE_DATA_DIRECTORY* pDataDirectory = NULL;
	pDataDirectory = m_pPEObj->GetDataDirectory();
	RETURN_FALSE_IF_FAILED(pDataDirectory);

	if((m_pPEObj->GetDataDirNum() <= IMAGE_DIRECTORY_ENTRY_TLS) || (pDataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size == 0))
	{
		*pRes = TRUE;
	}
	else
	{
		*pRes = FALSE;
	}

	return TRUE;
}

int CPEParser::GetImpFuncNum()
{
	return m_pPEObj->GetImpFuncNum();
}

int CPEParser::GetOptionalHeadSize()
{
	return m_pPEObj->GetSizeOfOptionalHead();
}

BOOL CPEParser::GetEntryPointSectionName(char* szSecName)
{
	IMAGE_SECTION_HEADER* szSecHead = NULL;
	if(!(szSecHead = m_pPEObj->GetSecHdr()))
	{
		return FALSE;
	}
	int nIndex = m_pPEObj->GetEntryOfSectionIndex();
	if(INVALID_RETURN_VALUE == nIndex)
	{
		return FALSE;
	}
	m_pMemoryManager->Copy(szSecName, SECTION_NAME_SIZE, (szSecHead[nIndex]).Name, sizeof((szSecHead[nIndex]).Name));
	strlwr(szSecName);

	return TRUE;
}

BOOL CPEParser::IsEntryPointLastSection()
{
	return m_pPEObj->GetEntryOfSectionIndex() == (m_pPEObj->GetSectionNum() - 1);
}

BOOL CPEParser::IsEntryPointOutOfSection()
{
	return (m_pPEObj->GetEntryOfSectionIndex() < 0 || m_pPEObj->GetEntryOfSectionIndex() > (m_pPEObj->GetSectionNum() - 1)); 
}

BOOL CPEParser::GetSectionNames(char* szSecName, int nSize)
{
	char* pDst = szSecName;
	int nSecNameSize = 0;
	int nSecCnt = m_pPEObj->GetSectionNum();
	IMAGE_SECTION_HEADER* szSecHead = m_pPEObj->GetSecHdr();
	for(int i = 0; i < nSecCnt; i++)
	{
		if((nSecNameSize += strlen((char*)szSecHead[i].Name) + 1) > nSize)
		{
			//超出范围直接break吧
			break;
		}

		memcpy_s(pDst, strlen((char*)szSecHead[i].Name), szSecHead[i].Name, strlen((char*)szSecHead[i].Name));
		pDst += strlen((char*)szSecHead[i].Name);
		*pDst++ = ';';
	}
	*(--pDst) = 0;//抹掉末尾的分号

	return TRUE;
}

BOOL CPEParser::SetObj(char* szPath)//后期这里改成只配置列表
{
	//todo:szPath保存到数组里
	FILE *fp = NULL;
	LONGLONG llSize = 0;
	fopen_s(&fp, szPath, "rb");
	RETURN_FALSE_IF_FAILED(fp);
	RETURN_FALSE_IF_FAILED((llSize = GetPESize(fp)));
	//CSafeArray<char> PEFileBuf(llSize);

	m_pPEFileBuf = new CSafeArray<char>(llSize);
	RETURN_FALSE_IF_FAILED(m_pPEFileBuf);

	fread_s(m_pPEFileBuf->GetBuff(), llSize, 1, llSize, fp);
	fclose(fp);
	
	RETURN_FALSE_IF_FAILED(m_pPEObj->Init(m_pPEFileBuf->GetBuff(), llSize));//PE文件解析过程

	return TRUE;
}

BOOL CPEParser::IsCheckSumError()
{
	return m_pPEObj->IsCheckSumError();
}

int CPEParser::GetZeroRawSizeSecNum()
{
	return m_pPEObj->GetZeroRawSizeSecNum();
}

BOOL CPEParser::IsSumSecSizeError()
{
	DEBUG_PRINT("RawSizeSume:%ld\n", m_pPEObj->GetRawSizeSum().QuadPart);
	DEBUG_PRINT("Size:%ld\n", m_pPEObj->GetSize().QuadPart);
	return m_pPEObj->GetRawSizeSum().QuadPart > m_pPEObj->GetSize().QuadPart;
}

LONGLONG CPEParser::GetPESize(FILE* fp)
{
	if(!fp)
	{
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	LONGLONG llSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	return llSize;
}

BOOL CPEParser::IsImpCorrupt()
{
	return m_pPEObj->IsImpCorrupt();
}

BOOL CPEParser::IsCodeSecAttrAbn()
{
	return m_pPEObj->IsCodeSecAttrAbn();
}

BOOL CPEParser::IsSecRawAddrAlignErr()
{
	return m_pPEObj->IsRawAddrAlignErr();
}

BOOL CPEParser::IsRsrcExe()
{
	return m_pPEObj->IsRsrcExe();
}

int CPEParser::GetPointSymTab()
{
	return m_pPEObj->GetPointerToSymbolTable();
}

int CPEParser::GetSymNum()
{
	return m_pPEObj->GetNumberOfSymbols();
}

BOOL CPEParser::IsPacked()
{
	return m_pPEObj->IsPacked();
}

double CPEParser::GetEntryEntropy()
{
	return m_pPEObj->GetEntryEntropy();
}

double CPEParser::GetFileEntropy()
{
	return m_pPEObj->GetFileEntropy();
}

double CPEParser::GetRsrcEntropy()
{
	return m_pPEObj->GetRsrcEntropy();
}

BOOL CPEParser::IsRawSizeAbn()
{
	return m_pPEObj->IsRawSizeAbn();
}

int CPEParser::GetRawSizeAbnNum()
{
	return m_pPEObj->GetRawSizeAbnNum();
}

double CPEParser::GetHighEntropyBlockRatio()
{
	return m_pPEObj->GetHighEntropyBlockRatio();
}

double CPEParser::GetHighEntropySectionRatio()
{
	return m_pPEObj->GetHighEntropySectionRatio();
}

int CPEParser::GetSecCnt()
{
	return m_pPEObj->GetSecCnt();
}

double CPEParser::GetExeSecRatio()
{
	return m_pPEObj->GetExeSecRatio();
}

DWORD CPEParser::GetTimeStamp()
{
	return m_pPEObj->GetTimeStamp();
}

BOOL CPEParser::IsAllUnexeSec()
{
	return m_pPEObj->IsAllUnexeSec();
}

int CPEParser::GetZeroDataDirNum()
{
	return m_pPEObj->GetZeroDataDirNum();
}

int CPEParser::GetInvalidDataDirNum()
{
	return m_pPEObj->GetInvalidDataDirNum();
}

int CPEParser::GetByteLastPage()
{
	return m_pPEObj->GetByteLastPage();
}

int CPEParser::GetPageInFile()
{
	return m_pPEObj->GetPageInFile();
}

int CPEParser::GetIATNum()
{
	return m_pPEObj->GetIATNum();
}

BOOL CPEParser::IsEntryRsrc()
{
	return m_pPEObj->IsEntryRsrc();
}

BOOL CPEParser::IsEntryReloc()
{
	return m_pPEObj->IsEntryReloc();
}

BOOL CPEParser::GetLastSecName(char* szLastSecName, int nLen)
{
	
	int nSecCnt = GetSecCnt();
	if(nSecCnt <= 0)
	{
		return FALSE;
	}

	IMAGE_SECTION_HEADER* szSecHead = NULL;
	if(!(szSecHead = m_pPEObj->GetSecHdr()))
	{
		return FALSE;
	}
	int nIndex = m_pPEObj->GetEntryOfSectionIndex();
	if(INVALID_RETURN_VALUE == nIndex)
	{
		return FALSE;
	}
	m_pMemoryManager->Copy(szLastSecName, sizeof((szSecHead[nSecCnt - 1]).Name), (szSecHead[nSecCnt - 1]).Name, sizeof((szSecHead[nSecCnt - 1]).Name));
	strlwr(szLastSecName);

	return TRUE;
}

BOOL CPEParser::IsOnlyRsrcVirtualSize()
{
	return m_pPEObj->IsOnlyRsrcVirtualSize();
}

BOOL CPEParser::HasRepeatedSecName()
{
	return m_pPEObj->HasRepeatedSecName();
}

int CPEParser::GetUnprintableSecNameNum()
{
	return m_pPEObj->GetUnprintableSecNameNum();
}

BOOL CPEParser::IsRdataWritable()
{
	return m_pPEObj->IsRdataWritable();
}

BOOL CPEParser::IsRdataExe()
{
	return m_pPEObj->IsRdataExe();
}

BOOL CPEParser::GetPEBuf(char** ppBuf, LONGLONG* pllSize)
{
	return m_pPEObj->GetPEBuf(ppBuf, pllSize);
}

BOOL CPEParser::ReleaseObj()
{
	SAFE_DELETE_POINTER(m_pPEFileBuf);
	return TRUE;
}

BOOL CPEParser::IsFileSizePESizeAbn()
{
	return m_pPEObj->IsFileSizePESizeAbn();
}

//BOOL CPEParser::GetObj()//后期这里改成只配置列表
//{
//todo 从数组中依次返回PEObj
//返回时再通过szPath具体生成对象
//	return 
//}

