#include "PeObj.h"
#include "stdafx.h"
#include "HeurRuleMacro.h"
#include <ImageHlp.h>
#pragma comment(lib, "imagehlp")

#define ALIGN_VALUE(val, algin) ( ~(algin-1) & (algin -1 + val ))

extern double dTotalEntryEntropyValue;
extern int nEntropyItem;
extern int nBlockItem;
extern double g_dExtraEntropy;
extern int g_nExtraCnt;

CPeObj::CPeObj():
m_pEntropyCalculator(new CEntropyCalculator())//todo
{
	Clean();
}

void CPeObj::Clean()
{
	m_pSecHdr = NULL;
	m_pOrgSecHdr = NULL;
	m_pEntrySecHdr = NULL;
	m_dataEntryNum = 0;
	m_pDataDirectory = NULL;
	m_entry = 0;
	m_is64 = false;
	m_flag = 0;
	m_typeFlag = 0;
	m_execSecNum = 0;
	//m_realImageSize = { 0 };
	m_currentOff = 0;
	memset(&m_dosHdr, 0, sizeof(m_dosHdr) );
	m_pComInfo = NULL;
	memset(&m_comInfo, 0, sizeof(m_comInfo));
	//m_fileSize = { 0 };
	memset(m_crcSigList, 0, sizeof(m_crcSigList));
	//m_szBuf = NULL;
	//m_inputObj = NULL;
	m_nSizeOfOptionalHead = 0;
	m_nImpFuncNum = 0;
	m_nIATNum = 0;
	m_nSizeOfCode = 0;
	m_nCheckSum = 0;
	m_nCalcedCheckSum = 0;
	m_nZeroRawSizeSecNum = 0;
	m_dZeroRawSizeSecRatio = 0.0;
	m_llSumRawSize.QuadPart = 0;
	m_bImpCorrupt = TRUE;
	m_bCodeSecAttrAbn = FALSE;
	m_bIsRawAddrAlignErr = FALSE;
	m_bIsRsrcExe = FALSE;
	m_nPointToSymTab = 0;
	m_nNumOfSym = 0;
	m_bIsPacked = FALSE;
	m_dEntryEntropyValue = 0.0;
	m_dFileEntropy = 0.0;
	m_dRsrcEntropy = 0.0;
	m_dRelocEntropy = 0.0;
	m_dHighEntropyBlockRatio = 0.0;
	m_dHighEntropySectionRatio = 0.0;
	m_dExeSecRatio = 0.0;
	m_dExtraEntropy = 0.0;
	m_nEntryIndex = -1;
	m_bIsRawSizeAbn = FALSE;
	m_bAllUnexe = FALSE;
	m_nRawSizeAbnNum = 0;
	m_nSecCnt = 0;
	m_dwTimeStamp = 0;
	m_nZeroSizeDataDirNum = 0;
	m_nInvalidDataDirNum = 0;
	m_nByteLastPage = 0;
	m_nPageInFile = 0;
	m_bIsEntryReloc = FALSE;
	m_bIsEntryRsrc = FALSE;
	m_bIsOnlyRsrcVirtualSize = FALSE;
	m_bHasRepeatedSecName = FALSE;
	m_nUnprintSecNameNum = 0;
	m_bRdataWritable = FALSE;
	m_bRdataExe = FALSE;
	m_pPEBuf = NULL;
	m_bIsFileSizePESizesAbn = FALSE;
}

CPeObj::~CPeObj()
{
	if (m_pSecHdr)
	{
		free(m_pSecHdr);
		m_pSecHdr = NULL;
	}
	if (m_pOrgSecHdr)
	{
		free(m_pOrgSecHdr);
		m_pOrgSecHdr = NULL;
	}

	if(m_pDataDirectory)
	{
		free(m_pDataDirectory);
	}
	
	SAFE_DELETE_POINTER(m_pEntropyCalculator);
}

BOOL CPeObj::IsPe(char* szBuf, LONGLONG llFileSize)
{
	char* pPos = NULL;
	BOOL isPe = FALSE;
	IMAGE_DOS_HEADER dosHdr = {0};
	pPos = szBuf;
	//input->Reset();
	memcpy_s(&dosHdr, sizeof(dosHdr), pPos, sizeof(dosHdr));
	int readSize = sizeof(dosHdr);
	//int readSize = input->Read(&dosHdr, sizeof(dosHdr));//这里已经改为传入buf了，就不用判断流操作是否读取成功了
	if (readSize ==  sizeof(IMAGE_DOS_HEADER) )
	{
		if ( (dosHdr.e_magic == 0x5a4d))
		{
			IMAGE_NT_HEADERS ntHdr;
			//todo 判断dosHdr.e_lfanew是否合法
			if(dosHdr.e_lfanew >= llFileSize)
			{
				return FALSE;
			}
			pPos += dosHdr.e_lfanew;
			//if (input->Seek(dosHdr.e_lfanew) >= 0)
			{
				memcpy_s(&ntHdr, sizeof(ntHdr), pPos, sizeof(ntHdr));
				readSize = sizeof(ntHdr);
				//readSize = input->Read(&ntHdr, sizeof(ntHdr));
				if (readSize == sizeof(ntHdr))
				{
					if (ntHdr.Signature == 0x4550)
					{
						isPe = TRUE;
					}
				}
			}

		}
	}
	return isPe;
}

int CPeObj::Init(char* szBuf, LONGLONG llSize)
{
	Clean();
	if (CPeObj::IsPe(szBuf, llSize))
	{
		m_fileSize.QuadPart = llSize;
		IMAGE_DOS_HEADER* dosHdr = (IMAGE_DOS_HEADER*)szBuf;
		RETURN_FALSE_IF_FAILED(dosHdr);
		
		IMAGE_NT_HEADERS* ntHdr = (IMAGE_NT_HEADERS*)(szBuf + dosHdr->e_lfanew);
		RETURN_FALSE_IF_FAILED(ntHdr);
		SetPEBuf(szBuf);
		SetByteLastPage(dosHdr->e_cblp);
		SetPageInFile(dosHdr->e_cp);
		SetDosHdr(dosHdr);//m_dosHdr
		SetSecHdrInfo(ntHdr);//m_pSecHdr
		SetNtHdrInfo(ntHdr);//m_NtHdr
		SetDataDirInfo(ntHdr);//m_pDataDirectory

		if(!CalcImpFuncNum(szBuf))//新增计算导入函数个数
		{
			m_bImpCorrupt = TRUE;//解析导入表时发现导入表异常
		}
		
		CalcCheckSum(szBuf, llSize);//计算校验和
		SetEntryPointInfo(ntHdr);
		SetSizeOfCode(ntHdr);
		SetCheckSum(ntHdr);
		
		SetTimeStamp(ntHdr->FileHeader.TimeDateStamp);
		SetPointerToSymbolTable();
		SetNumberOfSymbols();
		SetSectionInfo();//依赖

		SetEntryOfSectionIndex();//获取入口点所在节区的索引值
		CheckEntryRsrc();
		CheckEntryReloc();

		GetEntropyInfo((PUCHAR)szBuf, llSize);//计算熵相关的信息
		SetSectionAttr(m_ntHdr.FileHeader.NumberOfSections);
		ReadDotNetSymbol();
		
		return 1;
	}
	DEBUG_PRINT("Invalid PE..\n");
	return 0;
}

BOOL CPeObj::GetEntropyInfo(PUCHAR szBuf, LONGLONG llFileSize)
{
	m_pEntropyCalculator->Clean();
	m_pEntropyCalculator->Check((PUCHAR)szBuf, FALSE, llFileSize);
	m_pEntropyCalculator->Check2((PUCHAR)szBuf, FALSE, llFileSize, &m_dFileEntropy);
	/*if(m_bIsFileSizePESizesAbn)
	{
		m_pEntropyCalculator->CheckExtraEntropy((PUCHAR)szBuf, FALSE, m_fileSize.QuadPart, m_realImageSize.QuadPart, &m_dExtraEntropy);
		g_dExtraEntropy += m_dExtraEntropy;
		g_nExtraCnt++;
	}*/
	//检查入口点所在的节的熵值是否正常，如果入口点异常，那么就不会继续去算熵
	m_pEntropyCalculator->CheckSection((PUCHAR)szBuf, FALSE, llFileSize, GetEntryOfSectionIndex(), &m_dEntryEntropyValue);
	m_pEntropyCalculator->CheckSection((PUCHAR)szBuf, FALSE, llFileSize, GetDataDirSecIndex(IMAGE_DIRECTORY_ENTRY_RESOURCE), &m_dRsrcEntropy);
	m_pEntropyCalculator->CheckSection((PUCHAR)szBuf, FALSE, llFileSize, GetDataDirSecIndex(IMAGE_DIRECTORY_ENTRY_BASERELOC), &m_dRelocEntropy);
	
	//m_pEntropyCalculator->CheckRsrcEntropy(szBuf, FALSE, llFileSize, &m_dRsrcEntropy);
	//m_pEntropyCalculator->CheckRelocEntropy(szBuf, FALSE, llFileSize, &m_dRelocEntropy);
	m_pEntropyCalculator->CheckBlockEntropy(szBuf, FALSE, llFileSize);
	
	return TRUE;
}

//BOOL CPeObj::IsPe(IXInputStream* input)
//{
//	BOOL isPe = FALSE;
//	IMAGE_DOS_HEADER dosHdr = {0};
//	input->Reset();
//	int readSize = input->Read(&dosHdr, sizeof(dosHdr));
//	if (readSize ==  sizeof(IMAGE_DOS_HEADER) )
//	{
//		if ( (dosHdr.e_magic == 0x5a4d))
//		{
//			IMAGE_NT_HEADERS ntHdr;
//			if (input->Seek(dosHdr.e_lfanew) >= 0)
//			{
//				readSize = input->Read(&ntHdr, sizeof(ntHdr));
//				if (readSize == sizeof(ntHdr))
//				{
//					if (ntHdr.Signature == 0x4550)
//					{
//						isPe = TRUE;
//					}
//				}
//			}
//					
//		}
//	}
//	return isPe;
//}
//
//
//int CPeObj::Init(IXInputStream* input)
//{
//	
//	if (CPeObj::IsPe(input) )
//	{
//		m_inputObj = input;
//		BYTE buf[1024] = { 0 };
//		m_fileSize.QuadPart = input->GetSize();
//		input->Reset();
//		int readSize = input->Read(buf, sizeof(buf));
//		IMAGE_DOS_HEADER* dosHdr = (IMAGE_DOS_HEADER*)buf;
//		IMAGE_NT_HEADERS* ntHdr = (IMAGE_NT_HEADERS*)(buf + dosHdr->e_lfanew);
//		if (readSize < dosHdr->e_lfanew + sizeof(IMAGE_NT_HEADERS) +
//			ntHdr->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER) )
//		{
//			return -1;
//		}
//		m_dosHdr = *dosHdr;
//		IMAGE_SECTION_HEADER* secHdr = IMAGE_FIRST_SECTION(ntHdr);
//		size_t orgSecSize = ntHdr->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
//		size_t secSize = orgSecSize + sizeof(IMAGE_SECTION_HEADER);
//		m_pSecHdr = (IMAGE_SECTION_HEADER*)malloc(secSize);
//		m_pOrgSecHdr = (IMAGE_SECTION_HEADER*)malloc(orgSecSize);
//		memset(m_pSecHdr, 0, secSize);
//		memcpy(m_pSecHdr, secHdr, orgSecSize);
//		memcpy(m_pOrgSecHdr, m_pSecHdr, orgSecSize);
//
//		if (ntHdr->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
//		{
//			IMAGE_NT_HEADERS64* ntHdr64 = (IMAGE_NT_HEADERS64*)ntHdr;
//			m_pDataDirectory = ntHdr64->OptionalHeader.DataDirectory;
//			memcpy(&m_ntHdr64, ntHdr64, sizeof(m_ntHdr64));
//			if (m_ntHdr64.OptionalHeader.NumberOfRvaAndSizes > 16)
//				m_ntHdr64.OptionalHeader.NumberOfRvaAndSizes = 16;
//			m_dataEntryNum = m_ntHdr64.OptionalHeader.NumberOfRvaAndSizes;
//		}
//		else
//		{
//			m_pDataDirectory = ntHdr->OptionalHeader.DataDirectory;
//			memcpy(&m_ntHdr, ntHdr, sizeof(m_ntHdr));
//			if (m_ntHdr.OptionalHeader.NumberOfRvaAndSizes > 16)
//				m_ntHdr.OptionalHeader.NumberOfRvaAndSizes = 16;
//			m_dataEntryNum = m_ntHdr.OptionalHeader.NumberOfRvaAndSizes;
//		}
//
//		m_entry = ntHdr->OptionalHeader.AddressOfEntryPoint;
//		if (m_ntHdr.OptionalHeader.FileAlignment > 0x200)
//		{
//			m_ntHdr.OptionalHeader.FileAlignment = 0x200;
//		}
//
//		IMAGE_SECTION_HEADER* pCur = m_pSecHdr;
//		LARGE_INTEGER imageEnd = {0};
//		DWORD pSecEnd;
//		DWORD maxOff = 0;
//		for (WORD i = 0; i < m_ntHdr.FileHeader.NumberOfSections; i++)
//		{
//			if (m_flag & 1)
//			{
//				pCur->SizeOfRawData = pCur->Misc.VirtualSize;
//			}
//			if (pCur->SizeOfRawData)
//			{
//				pSecEnd = pCur->PointerToRawData + pCur->SizeOfRawData;
//				if (pSecEnd > imageEnd.LowPart)
//				{
//					imageEnd.QuadPart = ALIGN_VALUE(pSecEnd, m_ntHdr.OptionalHeader.FileAlignment);
//				}
//				if (pSecEnd > maxOff)
//				{
//					maxOff = pCur->PointerToRawData + pCur->SizeOfRawData;
//				}
//			}
//
//			if (pCur->Characteristics & IMAGE_SCN_MEM_EXECUTE)
//			{
//				m_execSecNum++;
//			}
//			pCur++;
//		}
//		if (imageEnd.QuadPart)
//		{
//			m_realImageSize = imageEnd;
//		}
//		else
//			m_realImageSize = m_fileSize;
//
//		SetSectionAttr(m_ntHdr.FileHeader.NumberOfSections);
//		ReadDotNetSymbol();
//		return 1;
//	}
//	return 0;
//}

IMAGE_SECTION_HEADER* CPeObj::GetRvaOfSection(DWORD rva)
{
	if (m_ntHdr.FileHeader.NumberOfSections == -1)
	{
		return NULL;
	}
	for (WORD i = 0; i < m_ntHdr.FileHeader.NumberOfSections; i++)
	{
		DWORD vSize = m_pSecHdr[i].Misc.VirtualSize;
		if (vSize == 0)
			vSize = m_pSecHdr[i].SizeOfRawData;

		if ( (rva >= m_pSecHdr[i].VirtualAddress)
			&& (rva < (m_pSecHdr[i].VirtualAddress + vSize) ) )
		{
			return &m_pSecHdr[i];
		}
	}
	return NULL;
}

IMAGE_SECTION_HEADER* CPeObj::GetOffOfSection(DWORD off)
{
	if (m_ntHdr.FileHeader.NumberOfSections == -1)
	{
		return NULL;
	}
	for (WORD i = 0; i < m_ntHdr.FileHeader.NumberOfSections; i++)
	{		
		if ( (off >= m_pSecHdr[i].PointerToRawData)
			&& (off < (m_pSecHdr[i].PointerToRawData + m_pSecHdr[i].SizeOfRawData) ) )
		{
			return &m_pSecHdr[i];
		}
	}
	return NULL;
}

DWORD CPeObj::RvaToOff(DWORD rva)
{
	IMAGE_SECTION_HEADER* pSec = GetRvaOfSection(rva);
	if (pSec)
	{
		return rva + pSec->PointerToRawData - pSec->VirtualAddress;
	}
	else
	{
		return (DWORD)-1;
	}
}

void CPeObj::SetEntryOfSectionIndex()
{
	if (m_pEntrySecHdr == NULL)
	{
		m_pEntrySecHdr = GetRvaOfSection(m_entry);
	}
	if (m_pEntrySecHdr >= m_pSecHdr)
	{
		m_nEntryIndex = ((int)m_pEntrySecHdr - (int)m_pSecHdr) / sizeof(IMAGE_SECTION_HEADER);//如果不强转为int，相减会得1
	}
	if(m_pSecHdr[m_nEntryIndex].PointerToRawData > m_fileSize.QuadPart)
	{
		m_nEntryIndex = -1;
	}
}

void CPeObj::SetSectionAttr(WORD secNum)
{
	IMAGE_SECTION_HEADER* pLastSec = &m_pSecHdr[secNum];
	*(DWORD*)pLastSec->Name = 'rdH';
	DWORD alignHeaderSize;
	if (secNum)
	{
		alignHeaderSize = ALIGN_VALUE(m_ntHdr.OptionalHeader.SizeOfHeaders, m_ntHdr.OptionalHeader.FileAlignment);
	}
	else
		alignHeaderSize = m_fileSize.LowPart;

	DWORD imageSize = 0;
	pLastSec->SizeOfRawData = alignHeaderSize;
	pLastSec->VirtualAddress = 0;
	pLastSec->Misc.VirtualSize = ALIGN_VALUE(alignHeaderSize, m_ntHdr.OptionalHeader.SectionAlignment);
	for (WORD i = 0; i < secNum+1; i++)
	{
		if (m_pSecHdr[i].Misc.VirtualSize == 0)
		{
			m_pSecHdr[i].Misc.VirtualSize = m_pSecHdr[i].SizeOfRawData;
		}
		if (i != secNum)
		{
			m_pSecHdr[i].VirtualAddress = ALIGN_VALUE(m_pSecHdr[i].VirtualAddress, m_ntHdr.OptionalHeader.SectionAlignment);
			m_pSecHdr[i].Misc.VirtualSize = ALIGN_VALUE(m_pSecHdr[i].Misc.VirtualSize, m_ntHdr.OptionalHeader.SectionAlignment);
			m_pSecHdr[i].SizeOfRawData = ALIGN_VALUE(m_pSecHdr[i].SizeOfRawData, m_ntHdr.OptionalHeader.FileAlignment);
		}
		if (m_pSecHdr[i].PointerToRawData <= m_fileSize.LowPart)
		{
			if (m_pSecHdr[i].PointerToRawData + m_pSecHdr[i].SizeOfRawData > m_fileSize.LowPart)
			{
				m_pSecHdr[i].SizeOfRawData = m_fileSize.LowPart - m_pSecHdr[i].PointerToRawData;
			}
		}
		else
		{
			m_pSecHdr[i].SizeOfRawData = 0;
			m_pSecHdr[i].PointerToRawData = 0;
		}
		if (i > 0)
		{
			DWORD *pFirstVAddr = &m_pSecHdr[0].VirtualAddress;
			DWORD *pFirstVSize = &m_pSecHdr[0].Misc.VirtualSize;
			for (WORD j = 0; j < i; j++)
			{
				if ( (m_pSecHdr[i].VirtualAddress < *pFirstVAddr)
					&& (m_pSecHdr[i].VirtualAddress + m_pSecHdr[i].Misc.VirtualSize > *pFirstVAddr) )
				{
					m_pSecHdr[i].Misc.VirtualSize = *pFirstVAddr - m_pSecHdr[i].VirtualAddress;
				}
				if (m_pSecHdr[i].VirtualAddress > *pFirstVAddr
					&& (m_pSecHdr[i].VirtualAddress < *pFirstVAddr + *pFirstVSize) )
				{
					m_pSecHdr[j].Misc.VirtualSize = m_pSecHdr[i].VirtualAddress - m_pSecHdr[j].VirtualAddress;
					break;
				}
			}
			if (m_pSecHdr[i-1].SizeOfRawData 
				&& m_pSecHdr[i].PointerToRawData > m_pSecHdr[i-1].PointerToRawData)
			{
				DWORD subSize = m_pSecHdr[i].PointerToRawData - m_pSecHdr[i - 1].PointerToRawData;
				if (subSize > 0)
				{
					m_pSecHdr[i - 1].SizeOfRawData = subSize;
				}								
			}
		}

		if (m_pSecHdr[i].SizeOfRawData > m_pSecHdr[i].Misc.VirtualSize)
		{
			m_pSecHdr[i].SizeOfRawData = m_pSecHdr[i].Misc.VirtualSize;
		}
		if (i < secNum)
		{
			if (m_pSecHdr[i].VirtualAddress + m_pSecHdr[i].Misc.VirtualSize < m_pSecHdr[i+1].VirtualAddress)
			{
				m_pSecHdr[i].Misc.VirtualSize = m_pSecHdr[i + 1].VirtualAddress - m_pSecHdr[i].VirtualAddress;
			}
		}
		if (m_pSecHdr[i].VirtualAddress + m_pSecHdr[i].Misc.VirtualSize > imageSize)
		{
			imageSize = m_pSecHdr[i].VirtualAddress + m_pSecHdr[i].Misc.VirtualSize;
		}
	}

	DWORD dataEntryNum = m_dataEntryNum;
	IMAGE_DATA_DIRECTORY* pEntry = m_pDataDirectory;

	// 检查哪些节包含了IMAGE_DATA_ENTRY里的数据
	for (WORD i = secNum; i > 0; i--)
	{
		if (m_pSecHdr[i].Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA)
		{
			for (DWORD j = 0; j < dataEntryNum; j++)
			{
				if (pEntry[j].VirtualAddress && pEntry[j].Size)
				{
					if (pEntry[j].VirtualAddress >= m_pSecHdr[i].VirtualAddress
						&& (pEntry[j].VirtualAddress < m_pSecHdr[i].VirtualAddress + m_pSecHdr[i].Misc.VirtualSize))
					{
						//todo 这里修改了每个Section Header的属性，是否有影响
						m_pSecHdr[i].Characteristics &= 0xFFFFFF7F;
					}
				}
			}
		}
	}

	if (m_ntHdr.OptionalHeader.SizeOfImage != imageSize)
		m_ntHdr.OptionalHeader.SizeOfImage = imageSize;
	if (m_ntHdr.OptionalHeader.SectionAlignment == m_ntHdr.OptionalHeader.FileAlignment
		&& m_ntHdr.OptionalHeader.FileAlignment < 0x200 && (secNum + 1) )
	{
		for (WORD i = 0; i < secNum+1; i++)
		{
			m_pSecHdr[i].Characteristics |= 0x80000000;
		}
	}
}

int GetComStreamTypeByName(const char* name)
{
	if (strcmp(name, "#Strings") == 0)
		return 0;
	else if (strcmp(name, "#Blob") == 0)
		return 1;
	else if (strcmp(name, "#GUID") == 0)
		return 2;
	else if (strcmp(name, "#US") == 0)
		return 3;
	else if (strcmp(name, "#~") == 0)
		return 4;
	else if (strcmp(name, "#-") == 0)
		return 5;

	return -1;
}

void CPeObj::ReadDotNetSymbol()
{
	//char* pPos = NULL;
	//int readSize = 0;
	//IMAGE_DATA_DIRECTORY* pDotNetDir = &m_pDataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR];
	//if (pDotNetDir->VirtualAddress == NULL || (pDotNetDir->Size < sizeof(IMAGE_COR20_HEADER)))
	//	return;

	//m_comInfo.cor20HdrOff.QuadPart = RvaToOff(pDotNetDir->VirtualAddress);
	//if (m_comInfo.cor20HdrOff.LowPart == (DWORD)-1)
	//	return;

	//if (m_inputObj->Seek(m_comInfo.cor20HdrOff.QuadPart) < 0)
	//	return;

	//m_inputObj->Read(&m_comInfo.corHdr, sizeof(IMAGE_COR20_HEADER));
	//if (m_comInfo.corHdr.cb != sizeof(IMAGE_COR20_HEADER) )
	//	return;
	//if (m_comInfo.corHdr.MetaData.VirtualAddress == 0)
	//	return;
	//if (m_comInfo.corHdr.MetaData.Size == 0)
	//	return;

	//m_comInfo.metaDataOff.QuadPart = RvaToOff(m_comInfo.corHdr.MetaData.VirtualAddress);
	//if (m_comInfo.metaDataOff.LowPart == (DWORD)-1)
	//	return;

	//if (m_inputObj->Seek(m_comInfo.metaDataOff.QuadPart) < 0)
	//	return;

	//m_inputObj->Read(&m_comInfo.metaDataHdr, sizeof(STORAGE_SIGNATURE));
	//if (m_comInfo.metaDataHdr.lSignature != 'BJSB')
	//	return;
	//
	//m_comInfo.streamOff.QuadPart = m_comInfo.metaDataOff.LowPart 
	//	+ m_comInfo.metaDataHdr.iLength + sizeof(STORAGE_SIGNATURE);

	//if (m_inputObj->Seek(m_comInfo.streamOff.QuadPart))
	//	return;

	//readSize = m_inputObj->Read(&m_comInfo.storageHdr, sizeof(STORAGE_HEADER));
	//if (readSize < sizeof(STORAGE_HEADER))
	//{
	//	return;
	//}
	//m_comInfo.comDataOff.QuadPart = m_comInfo.streamOff.LowPart + sizeof(STORAGE_HEADER);
	//if (m_inputObj->Seek(m_comInfo.comDataOff.QuadPart) < 0)
	//	return;
	//
	//STREAM_HEADER hdr;
	//for (DWORD i = 0; i < m_comInfo.storageHdr.NumberOfStreams; i++)
	//{
	//	readSize = m_inputObj->Read(&hdr, sizeof(hdr));
	//	if (readSize < sizeof(hdr) )
	//	{
	//		break;
	//	}
	//	const char* name = (char*)hdr.rcName;
	//	int typeCode = GetComStreamTypeByName(name);
	//	if (typeCode >= 0 && typeCode < 6)
	//	{
	//		memcpy(&m_comInfo.streamHdr[typeCode], &hdr, sizeof(hdr));
	//	}
	//	// stream hdr size : 8 + align(nameLen + 1, 4)
	//	m_comInfo.comDataOff.LowPart += ALIGN_VALUE(strlen(name) + 1, 4) + 8;
	//}
	//m_pComInfo = &m_comInfo;
}

DWORD CPeObj::GetDataCrc(DWORD seed, LARGE_INTEGER off, DWORD size)
{
	DWORD crc = 0;
	BYTE* buf = (BYTE*)malloc(size);
	if (buf)
	{
		/*if (m_inputObj->Seek(off.QuadPart) >= 0)
		{
			int readSize = m_inputObj->Read(buf, size);
			if (readSize == (int)size)
			{
				crc = MyGetCrc32(seed, buf, readSize);
			}
		}*/
		
		free(buf);
	}
	return crc;
}

DWORD CPeObj::GetEntryCrc32(DWORD& outDataSize)
{
	if (m_ntHdr.FileHeader.NumberOfSections == -1)
	{
		return 0;
	}
	if (m_pEntrySecHdr == NULL)
	{
		m_pEntrySecHdr = GetRvaOfSection(m_entry);
	}
	if (m_pEntrySecHdr == NULL || m_pEntrySecHdr->SizeOfRawData < 200)
	{
		return 0;
	}
	LARGE_INTEGER dataBegOff;
	int entryOff = (int)m_entry - (int)m_pEntrySecHdr->VirtualAddress;
	DWORD dataBeg = entryOff >= 100 ?  (DWORD)m_entry - 100 : m_pEntrySecHdr->VirtualAddress;
	DWORD dataEnd = m_pEntrySecHdr->VirtualAddress + m_pEntrySecHdr->SizeOfRawData;
	if (dataBeg + 200 < dataEnd)
		dataEnd = dataBeg + 200;

	dataBegOff.QuadPart = RvaToOff(dataBeg);
	if (dataBegOff.QuadPart == (DWORD)-1)
		return 0;

	DWORD dataSize = dataEnd - dataBeg;
	outDataSize = CheckDataSize(dataBegOff.LowPart, dataSize);
	DWORD crc = GetDataCrc(0, dataBegOff, outDataSize);

	return crc;
}

DWORD CPeObj::GetEntryOfSectionCrc32(DWORD& outDataSize)
{
	if (m_ntHdr.FileHeader.NumberOfSections == -1)
	{
		return 0;
	}
	if (m_pEntrySecHdr == NULL)
	{
		m_pEntrySecHdr = GetRvaOfSection(m_entry);
	}
	if (m_pEntrySecHdr == NULL || m_pEntrySecHdr->SizeOfRawData < 200)
	{
		return 0;
	}

	DWORD crc = 0;
	int blockSize = (int)m_pEntrySecHdr->SizeOfRawData;
	if (blockSize > 200)
		blockSize = 200;
	DWORD pSecEnd = m_pEntrySecHdr->VirtualAddress + m_pEntrySecHdr->SizeOfRawData;
	for (DWORD rva = m_pEntrySecHdr->VirtualAddress; rva <pSecEnd; rva += blockSize)
	{
		LARGE_INTEGER off;
		
		off.QuadPart = RvaToOff(rva);
		if (off.LowPart == (DWORD)-1)
			return 0;
		if (off.QuadPart > m_fileSize.QuadPart)
			return 0;
		blockSize = CheckDataSize(off.LowPart, blockSize);
		crc = GetDataCrc(0, off, blockSize);
	}
	outDataSize = m_pEntrySecHdr->SizeOfRawData;
	return crc;
}


DWORD CPeObj::GetEntryOfSectionFirst200Crc32(DWORD& outDataSize)
{
	if (m_ntHdr.FileHeader.NumberOfSections == -1)
	{
		return 0;
	}
	if (m_pEntrySecHdr == NULL)
	{
		m_pEntrySecHdr = GetRvaOfSection(m_entry);
	}
	if (m_pEntrySecHdr == NULL || m_pEntrySecHdr->SizeOfRawData < 200)
	{
		return 0;
	}

	DWORD crc = 0;
	LARGE_INTEGER secBegOff;
	secBegOff.QuadPart = RvaToOff(m_pEntrySecHdr->VirtualAddress);
	if (secBegOff.LowPart == (DWORD)-1)
	{
		return 0;
	}
	outDataSize = CheckDataSize(secBegOff.LowPart, 200);
	crc = GetDataCrc(0, secBegOff, outDataSize);

	return crc;	
}


DWORD CPeObj::GetEntryBefore200Crc32(DWORD& outDataSize)
{
	if (m_ntHdr.FileHeader.NumberOfSections == -1)
	{
		return 0;
	}
	if (m_pEntrySecHdr == NULL)
	{
		m_pEntrySecHdr = GetRvaOfSection(m_entry);
	}
	if (m_pEntrySecHdr == NULL 
		|| m_pEntrySecHdr->SizeOfRawData < 200
		|| m_entry - m_pEntrySecHdr->VirtualAddress < 200)
	{
		return 0;
	}
	LARGE_INTEGER off;
	off.QuadPart = RvaToOff(m_entry - m_pEntrySecHdr->VirtualAddress);
	if (off.LowPart == (DWORD)-1)
		return 0;

	outDataSize = CheckDataSize(off.LowPart, 200);
	DWORD crc = GetDataCrc(0, off, outDataSize);
	return crc;
}


DWORD CPeObj::GetCurrentOffDataCrc32(DWORD& outDataSize)
{
	if (m_fileSize.LowPart < 200 || m_currentOff + 100 >  m_fileSize.LowPart)
		return 0;

	LARGE_INTEGER off;
	off.LowPart = m_currentOff;
	DWORD crc = GetDataCrc(0, off, 100);
	
	off.QuadPart = m_fileSize.QuadPart - 100;
	crc = GetDataCrc(crc, off, 100);
	outDataSize = 200;

	return crc;
}

DWORD CPeObj::GetDataCrc32(DWORD index, DWORD& outDataSize)
{
	DWORD crc;
	DWORD dataSize;
	if (index >= 6)
	{
		return 0;
	}
	if (m_crcSigList[index].crc && m_crcSigList[index].dataSize)
	{
		outDataSize = m_crcSigList[index].dataSize;
		return m_crcSigList[index].crc;
	}

	switch (index)
	{
	case 0:
		dataSize = crc = (DWORD)-1;
		break;
	case 1:
		crc = GetEntryCrc32(dataSize);
		break;
	case 2:
		crc = GetEntryOfSectionCrc32(dataSize);
		break;
	case 3:
		crc = GetEntryOfSectionFirst200Crc32(dataSize);
		break;
	case 4:
		crc = GetEntryBefore200Crc32(dataSize);
		break;
	case 5:
		crc = GetCurrentOffDataCrc32(dataSize);
		break;
	}

	if (crc == 0)
	{
		dataSize = crc = (DWORD)-1;
	}
	
	m_crcSigList[index].crc = crc;
	m_crcSigList[index].dataSize = dataSize;
	outDataSize = dataSize;
	return crc;
}

DWORD CPeObj::GetPeFlag()
{

	// IMAGE_SUBSYSTEM_NATIVE 如驱动
	// 
	DWORD shift;

	if (m_ntHdr.FileHeader.Characteristics & 0x2000) // IS DLL
		shift = m_ntHdr.OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_NATIVE + 2; // DLL IMAGE_SUBSYSTEM_WINDOWS_CUI
	else
		shift = m_ntHdr.OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_NATIVE;
	m_typeFlag |= (1 << shift);
	if (!(m_flag & 0x20))
	{
		m_flag = m_flag | 0x20;
	}
	return m_typeFlag;
}

BOOL CPeObj::CalcImpFuncNum(char* szBuf)
{
	//	IMAGE_DATA_DIRECTORY* pDataDirectory = m_pPEObj->GetDataDirectory();
		ULONG ulImpTableVA = m_pDataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	//	IMAGE_SECTION_HEADER *pSecHead = m_pPEObj->GetSecHdr();
		for(int i = 0; i < m_ntHdr.FileHeader.NumberOfSections; i++)
		{
			if(m_pSecHdr[i].VirtualAddress <= ulImpTableVA && ulImpTableVA <= m_pSecHdr[i].VirtualAddress + m_pSecHdr[i].Misc.VirtualSize)//如果是在一个合法的节区里，就没问题
			{
				m_bImpCorrupt = FALSE;
			}
		}

		if(m_bImpCorrupt)//如果导入表的位置不对，不进行下边的解析，否则会崩溃
		{
			m_nImpFuncNum = 0;
			return FALSE;
		}

		ULONG ulOffset = RvaToOffset(m_pSecHdr, ulImpTableVA);
		if(!ulOffset || ulOffset > m_fileSize.QuadPart)//偏移位置超过了文件的大小
		{
			return FALSE;
		}

		IMAGE_IMPORT_DESCRIPTOR *pImportTable = (IMAGE_IMPORT_DESCRIPTOR *)(szBuf + ulOffset);
		IMAGE_IMPORT_DESCRIPTOR ImpDesc;
		memset(&ImpDesc, 0, sizeof(IMAGE_IMPORT_DESCRIPTOR));
	
		//pImportTable表的最后一个是全0，或者Name是0
		for(int i=0; memcmp(pImportTable + i, &ImpDesc, sizeof(ImpDesc))!=0; i++)
		{

			ULONG ulNameOffset = RvaToOffset(m_pSecHdr, pImportTable[i].Name);
			if(0 == ulNameOffset || ulNameOffset > m_fileSize.QuadPart)//如果拿到dll名字失败
			{
				m_bImpCorrupt = TRUE;
				DEBUG_PRINT("Import table corrupt\n");
				return FALSE;
			}
			m_nIATNum++;

			//拿到了DLL的名字
			char *dllName= (char*)(szBuf + RvaToOffset(m_pSecHdr, pImportTable[i].Name));
	
			ULONG ulFirstThunkOffset = RvaToOffset(m_pSecHdr, pImportTable[i].FirstThunk);
			if((0 == ulFirstThunkOffset) || ulFirstThunkOffset >  m_fileSize.QuadPart)
			{
				m_bImpCorrupt = TRUE;
				DEBUG_PRINT("Import table corrupt\n");
				return FALSE;
			}
			PIMAGE_THUNK_DATA32 pThunk=(PIMAGE_THUNK_DATA32)(szBuf + ulFirstThunkOffset);
			
			while(pThunk->u1.Ordinal!=NULL)
			{
				PIMAGE_IMPORT_BY_NAME pname=(PIMAGE_IMPORT_BY_NAME)(szBuf + RvaToOffset(m_pSecHdr, pThunk->u1.AddressOfData));
				//printf("函数编号: %d 名称: %s\n",pname->Hint,pname->Name);
				m_nImpFuncNum++;
				pThunk++;
			}
		}

		return TRUE;
}

ULONG CPeObj::RvaToOffset(IMAGE_SECTION_HEADER* pSecHead, ULONG ulImpTableVA)
{
	int nSecNum = GetSectionNum();
	for(int i = 0; i < nSecNum; i++)
	{
		if(pSecHead->VirtualAddress <= ulImpTableVA && ulImpTableVA < (pSecHead->VirtualAddress + pSecHead->Misc.VirtualSize))//
		{
			return ulImpTableVA - pSecHead->VirtualAddress + pSecHead->PointerToRawData;
		}
		pSecHead++;
	}

	return 0;
}

BOOL CPeObj::CalcCheckSum(char* szBuf, int nSize)
{
	DWORD dwHeaderSum = 0;
	if(!CheckSumMappedFile(szBuf, nSize, &dwHeaderSum, (PDWORD)&m_nCalcedCheckSum))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CPeObj::IsCheckSumError()
{
	return m_nCalcedCheckSum != m_nCheckSum;
}

int CPeObj::GetZeroRawSizeSecNum()
{
	return m_nZeroRawSizeSecNum;
}

void CPeObj::SetPointerToSymbolTable()
{
	m_nPointToSymTab = m_ntHdr.FileHeader.PointerToSymbolTable;
}

void CPeObj::SetNumberOfSymbols()
{
	m_nNumOfSym = m_ntHdr.FileHeader.NumberOfSymbols;
}

void CPeObj::SetSecCnt()
{
	m_nSecCnt = m_ntHdr.FileHeader.NumberOfSections;
}

void CPeObj::SetExeSecRatio(double dExeSecRatio)
{
	m_dExeSecRatio = dExeSecRatio;
}

void CPeObj::SetTimeStamp(DWORD dwTimeStamp)
{
	m_dwTimeStamp = dwTimeStamp;
}

void CPeObj::SetIsAllUnexeSec(BOOL bAllUnexe)
{
	m_bAllUnexe = bAllUnexe;
}

//void CPeObj::SetDataDirectoryInfo()
//{
//	for(int i = IMAGE_DIRECTORY_ENTRY_EXPORT; i <= IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR; i++)
//	{
//		if(0 == m_pDataDirectory[i].Size)
//		{
//			m_nZeroSizeDataDirNum++;
//		}
//		if((m_pDataDirectory[i].VirtualAddress < m_pSecHdr[0].VirtualAddress || 
//			m_pDataDirectory[i].VirtualAddress > m_pSecHdr[m_ntHdr.FileHeader.NumberOfSections - 1].VirtualAddress + m_pSecHdr[m_ntHdr.FileHeader.NumberOfSections - 1].Misc.VirtualSize) &&
//			m_pDataDirectory[i].VirtualAddress != 0)
//		{
//			m_nInvalidDataDirNum++;
//		}
//	}
//}

void CPeObj::SetByteLastPage(int nByteLastPage)
{
	m_nByteLastPage = nByteLastPage;
}

void CPeObj::SetPageInFile(int nPageInFile)
{
	m_nPageInFile = nPageInFile;
}

//void CPeObj::CheckEntryRsrcOrReloc()//todo 修改所有Rsrc定位的方式，不要以节名定位，要通过datadirectory定位
//{
//	if(-1 == m_nEntryIndex)
//	{
//		return;
//	}
//
//	int nRsrcIndex = -1;
//	int nRelocIndex = -1;
//	ULONG ulRsrcVA = m_pDataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
//	ULONG ulRelocVA = m_pDataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
//
//	for(int i = 0; i < m_ntHdr.FileHeader.NumberOfSections; i++)
//	{
//		if(m_pSecHdr[i].VirtualAddress <= ulRsrcVA && ulRsrcVA <= m_pSecHdr[i].VirtualAddress + m_pSecHdr[i].Misc.VirtualSize)//如果是在一个合法的节区里，就没问题
//		{
//			nRsrcIndex = i;
//		}
//
//		if(m_pSecHdr[i].VirtualAddress <= ulRelocVA && ulRelocVA <= m_pSecHdr[i].VirtualAddress + m_pSecHdr[i].Misc.VirtualSize)//如果是在一个合法的节区里，就没问题
//		{
//			nRelocIndex = i;
//		}
//	}
//	
//	if(nRsrcIndex == m_nEntryIndex)
//	{
//		m_bIsEntryRsrc = TRUE;
//	}
//
//	if(nRelocIndex == m_nEntryIndex)
//	{
//		m_bIsEntryReloc = TRUE;
//	}
//	/*if(-1 != m_nEntryIndex)
//	{
//		if(strcmp(strlwr((char*)m_pSecHdr[m_nEntryIndex].Name), ".reloc") == 0 ||
//			strcmp(strlwr((char*)m_pSecHdr[m_nEntryIndex].Name), "reloc") == 0)
//		{
//			m_bIsEntryReloc = TRUE;
//		}
//		if(strcmp(strlwr((char*)m_pSecHdr[m_nEntryIndex].Name), ".rsrc") == 0 ||
//			strcmp(strlwr((char*)m_pSecHdr[m_nEntryIndex].Name), "rsrc") == 0)
//		{
//			m_bIsEntryRsrc = TRUE;
//		}
//	}*/
//}

void CPeObj::CheckEntryRsrc()
{
	if(-1 == m_nEntryIndex)
	{
		return;
	}

	if(m_nEntryIndex == GetDataDirSecIndex(IMAGE_DIRECTORY_ENTRY_RESOURCE) ||
		strcmp(strlwr((char*)m_pSecHdr[m_nEntryIndex].Name), ".rsrc") == 0 ||
		strcmp(strlwr((char*)m_pSecHdr[m_nEntryIndex].Name), "rsrc") == 0)
	{
		m_bIsEntryRsrc = TRUE;
	}
}

void CPeObj::CheckEntryReloc()
{
	if(-1 == m_nEntryIndex)
	{
		return;
	}

	if(m_nEntryIndex == GetDataDirSecIndex(IMAGE_DIRECTORY_ENTRY_BASERELOC) || 
		strcmp(strlwr((char*)m_pSecHdr[m_nEntryIndex].Name), ".reloc") == 0 ||
		strcmp(strlwr((char*)m_pSecHdr[m_nEntryIndex].Name), "reloc") == 0)
	{
		m_bIsEntryReloc = TRUE;
	}
}

int CPeObj::GetDataDirSecIndex(int nDataDirIndex)
{
	for(int i = 0; i < m_ntHdr.FileHeader.NumberOfSections; i++)
	{
		if(m_pSecHdr[i].VirtualAddress <= m_pDataDirectory[nDataDirIndex].VirtualAddress &&
			m_pDataDirectory[nDataDirIndex].VirtualAddress <= m_pSecHdr[i].VirtualAddress + m_pSecHdr[i].Misc.VirtualSize)
		{
			return i;
		}
	}
	return -1;
}

void CPeObj::CheckIsOnlyRsrcVirtualSize(IMAGE_SECTION_HEADER* pSec, int nRsrcIndex)
{
	RETURN_IF_FAILED(pSec);
	m_bIsOnlyRsrcVirtualSize = TRUE;
	if(1 == m_ntHdr.FileHeader.NumberOfSections)
	{
		m_bIsOnlyRsrcVirtualSize = FALSE;
		return;
	}

	if(-1 == nRsrcIndex)
	{
		m_bIsOnlyRsrcVirtualSize = FALSE;
		return;
	}

	for (WORD i = 0; i < m_ntHdr.FileHeader.NumberOfSections; i++)
	{
		if(nRsrcIndex == i)
		{
			if(0 == pSec->Misc.VirtualSize)
			{
				m_bIsOnlyRsrcVirtualSize = FALSE;
				break;
			}
		}
		else
		{
			if(0 != pSec->Misc.VirtualSize)
			{
				m_bIsOnlyRsrcVirtualSize = FALSE;
				break;
			}
		}
		pSec++;
	}
}

void CPeObj::SetSecHdrInfo(IMAGE_NT_HEADERS* ntHdr)
{
	IMAGE_SECTION_HEADER* secHdr = IMAGE_FIRST_SECTION(ntHdr);
	m_nSizeOfOptionalHead = ntHdr->FileHeader.SizeOfOptionalHeader;
	size_t orgSecSize = ntHdr->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER);
	size_t secSize = orgSecSize + sizeof(IMAGE_SECTION_HEADER);
	m_pSecHdr = (IMAGE_SECTION_HEADER*)malloc(secSize);
	m_pOrgSecHdr = (IMAGE_SECTION_HEADER*)malloc(orgSecSize);
	memset(m_pSecHdr, 0, secSize);
	memcpy(m_pSecHdr, secHdr, orgSecSize);
	memcpy(m_pOrgSecHdr, m_pSecHdr, orgSecSize);
}

void CPeObj::SetDosHdr(IMAGE_DOS_HEADER* dosHdr)
{
	m_dosHdr = *dosHdr;
}

void CPeObj::SetNtHdrInfo(IMAGE_NT_HEADERS* ntHdr)
{
	if (ntHdr->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		IMAGE_NT_HEADERS64* ntHdr64 = (IMAGE_NT_HEADERS64*)ntHdr;
		memcpy(&m_ntHdr64, ntHdr64, sizeof(m_ntHdr64));
		if (m_ntHdr64.OptionalHeader.NumberOfRvaAndSizes > 16)
			m_ntHdr64.OptionalHeader.NumberOfRvaAndSizes = 16;
		m_dataEntryNum = m_ntHdr64.OptionalHeader.NumberOfRvaAndSizes;
	}
	else
	{
		memcpy(&m_ntHdr, ntHdr, sizeof(m_ntHdr));
		if (m_ntHdr.OptionalHeader.NumberOfRvaAndSizes > 16)
			m_ntHdr.OptionalHeader.NumberOfRvaAndSizes = 16;
		m_dataEntryNum = m_ntHdr.OptionalHeader.NumberOfRvaAndSizes;
	}
}

void CPeObj::SetDataDirInfo(IMAGE_NT_HEADERS* ntHdr)
{
	if (ntHdr->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
	{
		IMAGE_NT_HEADERS64* ntHdr64 = (IMAGE_NT_HEADERS64*)ntHdr;
		m_pDataDirectory = (IMAGE_DATA_DIRECTORY*)malloc(sizeof(ntHdr64->OptionalHeader.DataDirectory));
		memcpy(m_pDataDirectory, ntHdr64->OptionalHeader.DataDirectory, sizeof(ntHdr64->OptionalHeader.DataDirectory));
	}
	else
	{
		m_pDataDirectory = (IMAGE_DATA_DIRECTORY*)malloc(sizeof(ntHdr->OptionalHeader.DataDirectory));
		memcpy(m_pDataDirectory, ntHdr->OptionalHeader.DataDirectory, sizeof(ntHdr->OptionalHeader.DataDirectory));
	}

	CalcInvalidDataDir();
}

void CPeObj::SetEntryPointInfo(IMAGE_NT_HEADERS* ntHdr)
{
	RETURN_IF_FAILED(ntHdr);
	m_entry = ntHdr->OptionalHeader.AddressOfEntryPoint;
}

void CPeObj::SetSizeOfCode(IMAGE_NT_HEADERS* ntHdr)
{
	RETURN_IF_FAILED(ntHdr);
	m_nSizeOfCode = ntHdr->OptionalHeader.SizeOfCode;
}

void CPeObj::SetCheckSum(IMAGE_NT_HEADERS* ntHdr)
{
	RETURN_IF_FAILED(ntHdr);
	m_nCheckSum = ntHdr->OptionalHeader.CheckSum;
}

void CPeObj::SetSectionInfo()
{
	int nRsrcIndex = 0;

	IMAGE_SECTION_HEADER* pCur = m_pSecHdr;
	LARGE_INTEGER imageEnd = {0};
	DWORD pSecEnd;
	DWORD maxOff = 0;
	std::string sSecNames;
	nRsrcIndex = GetDataDirSecIndex(IMAGE_DIRECTORY_ENTRY_RESOURCE);
	CheckIsOnlyRsrcVirtualSize(m_pSecHdr, nRsrcIndex);

	int nRawFileAlign = m_ntHdr.OptionalHeader.FileAlignment;
	if (m_ntHdr.OptionalHeader.FileAlignment > 0x200)
	{
		m_ntHdr.OptionalHeader.FileAlignment = 0x200;
	}

	for (WORD i = 0; i < m_ntHdr.FileHeader.NumberOfSections; i++)
	{
		if (m_flag & 1)
		{
			pCur->SizeOfRawData = pCur->Misc.VirtualSize;
		}

		if((int32_t)pCur->SizeOfRawData <= 0 || (int32_t)pCur->SizeOfRawData >= m_fileSize.QuadPart)
		{
			m_bIsRawSizeAbn = TRUE;
			m_nRawSizeAbnNum++;
		}

		if (pCur->SizeOfRawData)
		{
			pSecEnd = pCur->PointerToRawData + pCur->SizeOfRawData;
			if (pSecEnd > imageEnd.LowPart)
			{
				imageEnd.QuadPart = ALIGN_VALUE(pSecEnd, m_ntHdr.OptionalHeader.FileAlignment);
			}
			if (pSecEnd > maxOff)
			{
				maxOff = pCur->PointerToRawData + pCur->SizeOfRawData;
			}
			if(nRawFileAlign)//有些样本的FileAlign会是0
			{
				if(!m_bIsRawAddrAlignErr && (pCur->SizeOfRawData % nRawFileAlign))
				{
					m_bIsRawAddrAlignErr = TRUE;
				}
			}
		}
		else
		{
			m_nZeroRawSizeSecNum++;
		}

		m_llSumRawSize.QuadPart += pCur->SizeOfRawData;

		if (pCur->Characteristics & IMAGE_SCN_MEM_EXECUTE)//计算是否有多个具备可执行属性的节区
		{
			m_execSecNum++;
		}

		//判断资源节是否具有可执行属性
		if(nRsrcIndex == i && strcmp(strlwr((char*)pCur->Name), ".text") != 0)
		{
			if(pCur->Characteristics & IMAGE_SCN_MEM_EXECUTE)
			{
				m_bIsRsrcExe = TRUE;
			}
		}

		if(strcmp(strlwr((char*)pCur->Name), ".text") == 0 ||
			strcmp(strlwr((char*)pCur->Name), "text") == 0 ||
			strcmp(strlwr((char*)pCur->Name), ".code") == 0 ||
			strcmp(strlwr((char*)pCur->Name), "code") == 0)
		{
			if(pCur->Characteristics & 0x9FFFFFDF)//代码段属性不是0x60000020的都视为异常；如果有多个代码段，那么其中一个不是0x60000020也视为异常。
			{
				m_bCodeSecAttrAbn = TRUE;
			}
		}

		if(strcmp(strlwr((char*)pCur->Name), ".rdata") == 0 ||
			strcmp(strlwr((char*)pCur->Name), "rdata") == 0)
		{
			if(pCur->Characteristics & IMAGE_SCN_MEM_WRITE)
			{
				m_bRdataWritable = TRUE;
			}
			if(pCur->Characteristics & IMAGE_SCN_MEM_EXECUTE ||
				pCur->Characteristics & IMAGE_SCN_CNT_CODE)
			{
				m_bRdataExe = TRUE;
			}
		}

		if(0 == strlen((char*)pCur->Name))
		{
			m_nUnprintSecNameNum++;
		}

		if(!m_bHasRepeatedSecName && 0 != strlen((char*)pCur->Name))
		{
			if(-1 != sSecNames.find((char*)pCur->Name))
			{
				m_bHasRepeatedSecName = TRUE;
			}
			else
			{
				sSecNames.append((char*)pCur->Name);
				sSecNames.append(";");
			}
		}

		pCur++;
	}

	if (imageEnd.QuadPart)
	{
		m_realImageSize = imageEnd;
	}
	else
	{
		m_realImageSize = m_fileSize;
	}

	m_bIsFileSizePESizesAbn = m_fileSize.QuadPart > m_realImageSize.QuadPart;
	SetSecCnt();
	SetExeSecRatio((double)m_execSecNum/(double)m_ntHdr.FileHeader.NumberOfSections);
	SetIsAllUnexeSec(m_execSecNum == 0);
}

void CPeObj::CalcInvalidDataDir()
{
	for(int i = IMAGE_DIRECTORY_ENTRY_EXPORT; i <= IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR; i++)
	{
		if(0 == m_pDataDirectory[i].Size)
		{
			m_nZeroSizeDataDirNum++;
		}
		if((m_pDataDirectory[i].VirtualAddress < m_pSecHdr[0].VirtualAddress || 
			m_pDataDirectory[i].VirtualAddress > m_pSecHdr[m_ntHdr.FileHeader.NumberOfSections - 1].VirtualAddress + m_pSecHdr[m_ntHdr.FileHeader.NumberOfSections - 1].Misc.VirtualSize) &&
			m_pDataDirectory[i].VirtualAddress != 0)
		{
			m_nInvalidDataDirNum++;
		}
	}
}

BOOL CPeObj::GetPEBuf(char** ppBuf, LONGLONG* pllSize)
{
	RETURN_FALSE_IF_FAILED(ppBuf);
	RETURN_FALSE_IF_FAILED(pllSize);

	*ppBuf = m_pPEBuf;
	*pllSize = m_fileSize.QuadPart;
}

void CPeObj::SetPEBuf(char* pPEBuf)
{
	RETURN_IF_FAILED(pPEBuf);
	m_pPEBuf = pPEBuf;
}