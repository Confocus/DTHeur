#include "EntropyCalculator.h"
#include "HeurRuleMacro.h"

#define		ENTROPY_BLOCK_SIZE		4096
#define		ENTROPY_VALUE_SIZE		0x10

CEntropyCalculator::CEntropyCalculator(void)
{
	Clean();
	GetEntropyConfigPath();
	ParseEntropyFile();
}

void CEntropyCalculator::Clean()
{
	//预读的值不要清零
	/*m_dHighBlockEntropy = 0.0;
	m_dHighSectionEntropy = 0.0;*/

	m_dHighEntropyBlockRatio = 0.0;
	m_dHighEntropySectionRatio = 0.0;
	m_nHighEntropyBlockCnt = 0;
}

CEntropyCalculator::~CEntropyCalculator(void)
{
}

BOOL CEntropyCalculator::Check(PUCHAR image_base, BOOL bmem, LONGLONG llFileSize)
{
	int nHighSecCnt = 0;
	PIMAGE_DOS_HEADER		pDosHeader = (PIMAGE_DOS_HEADER)image_base;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	PIMAGE_NT_HEADERS		pNtHeaders = (PIMAGE_NT_HEADERS)(image_base + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}
	PIMAGE_SECTION_HEADER	pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);
	const double pack_value = 6.75;
	const double normal_value = 6.5;
	for (int i = 0ul; i < pNtHeaders->FileHeader.NumberOfSections; i++)
	{
		int code_size = (pSectionHeader[i].SizeOfRawData < pSectionHeader[i].Misc.VirtualSize) ? pSectionHeader[i].SizeOfRawData : pSectionHeader[i].Misc.VirtualSize;
		if(code_size < 0)
		{
			continue;
		}

		PBYTE code_buffer = (PBYTE)(image_base + pSectionHeader[i].PointerToRawData);
		if (bmem)
		{
			code_buffer = (PBYTE)(image_base + pSectionHeader[i].VirtualAddress);
		}

		if(pSectionHeader[i].PointerToRawData >= llFileSize)
		{
			continue;
		}
		
		if(pSectionHeader[i].PointerToRawData + code_size > llFileSize)//加UPX壳会访问越界
		{
			code_size = llFileSize - pSectionHeader[i].PointerToRawData;
		}

		while (code_size && code_buffer[code_size - 1] == 0)
		{
			code_size--;
		}

		double fent = CalcEntropy((const char *)code_buffer, code_size);
		if(fent > m_dHighSectionEntropy)
		{
			nHighSecCnt++;
		}
		/*fprintf(stdout, "section = %s\r\n", pSectionHeader[i].Name);
		if (fent > pack_value)
		{
			printf("section packed or crypted\r\n");
		}
		if (fent < normal_value)
		{
			printf("section normal\r\n");
		}
		if (fent >= normal_value
			&&fent <= pack_value)
		{
			printf("section may packed or crypted\r\n");
		}*/
	}
	if(!pNtHeaders->FileHeader.NumberOfSections)
	{
		return FALSE;
	}
	m_dHighEntropySectionRatio = (double)nHighSecCnt/(double)pNtHeaders->FileHeader.NumberOfSections;

	return TRUE;
}

void CEntropyCalculator::Check2(PUCHAR image_base, BOOL bmem, LONGLONG llFileSize, double *pfent)
{
	RETURN_IF_FAILED(pfent);
	RETURN_IF_FAILED(image_base);

	PIMAGE_DOS_HEADER		pDosHeader = (PIMAGE_DOS_HEADER)image_base;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return;
	}
	PIMAGE_NT_HEADERS		pNtHeaders = (PIMAGE_NT_HEADERS)(image_base + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return;
	}
	PIMAGE_SECTION_HEADER	pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);
	uint32_t byte_count[256] = { 0 };
	size_t total_size = 0;
	for (int i = 0ul; i < pNtHeaders->FileHeader.NumberOfSections; i++)
	{
		int code_size = (pSectionHeader[i].SizeOfRawData < pSectionHeader[i].Misc.VirtualSize) ? pSectionHeader[i].SizeOfRawData : pSectionHeader[i].Misc.VirtualSize;
		if(code_size < 0)
		{
			continue;
		}

		if(pSectionHeader[i].PointerToRawData > llFileSize)
		{
			continue;
		}

		PBYTE code_buffer = (PBYTE)(image_base + pSectionHeader[i].PointerToRawData);

		if (bmem)
		{
			code_buffer = (PBYTE)(image_base + pSectionHeader[i].VirtualAddress);
		}
		
		if(pSectionHeader[i].PointerToRawData >= llFileSize)
		{
			continue;
		}

		if(pSectionHeader[i].PointerToRawData + code_size > llFileSize)//加UPX壳会访问越界
		{
			code_size = llFileSize - pSectionHeader[i].PointerToRawData;
		}
		while (code_size && code_buffer[code_size - 1] == 0)
		{
			code_size--;
		}

		for (size_t j = 0; j != code_size; ++j)
		{
			++byte_count[static_cast<unsigned char>(code_buffer[j])];
		}
		total_size += code_size;
	}

	*pfent = CalcEntropy(byte_count, total_size);
}

BOOL CEntropyCalculator::CheckExtraEntropy(PUCHAR image_base, BOOL bmem, LONGLONG llFileSize, LONGLONG llPESizes, double *pfent)
{
	RETURN_FALSE_IF_FAILED(pfent);
	RETURN_FALSE_IF_FAILED(image_base);

	PIMAGE_DOS_HEADER		pDosHeader = (PIMAGE_DOS_HEADER)image_base;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	PIMAGE_NT_HEADERS		pNtHeaders = (PIMAGE_NT_HEADERS)(image_base + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}
	uint32_t byte_count[256] = { 0 };
	size_t code_size = llFileSize - llPESizes;
	PBYTE code_buffer = (PBYTE)(image_base + llPESizes);

	while (code_size && code_buffer[code_size - 1] == 0)
	{
		code_size--;
	}

	for (size_t j = 0; j != code_size; ++j)
	{
		++byte_count[static_cast<unsigned char>(code_buffer[j])];
	}

	*pfent = CalcEntropy(byte_count, code_size);
	return TRUE;
}

BOOL CEntropyCalculator::Is_Packed(PUCHAR image_base)
{
	PIMAGE_DOS_HEADER		pDosHeader = (PIMAGE_DOS_HEADER)image_base;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return false;
	}
	PIMAGE_NT_HEADERS		pNtHeaders = (PIMAGE_NT_HEADERS)(image_base + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return false;
	}
	long EpAddress = pNtHeaders->OptionalHeader.AddressOfEntryPoint;
	PIMAGE_SECTION_HEADER	pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);
	for (int i = 0ul; i < pNtHeaders->FileHeader.NumberOfSections; i++)
	{
		if (EpAddress >= pSectionHeader[i].VirtualAddress
			&& EpAddress <= pSectionHeader[i].VirtualAddress + pSectionHeader[i].Misc.VirtualSize)
		{
			//Ep命中
			IMAGE_SECTION_HEADER pSection = pSectionHeader[i];
			//fprintf(stdout, "section oep at %s\r\n", pSection.Name);
			if (pSection.Characteristics & IMAGE_SCN_MEM_WRITE)//加壳特点是该节表可写
			{
				//节表名字不正常
				if (memcmp(pSection.Name, ".textbss", 8) != 0
					&& memcmp(pSection.Name, ".text\x0\x0\x0", 8) != 0
					&& memcmp(pSection.Name, "CODE\x0\x0\x0\x0", 8) != 0
					&& memcmp(pSection.Name, "INIT\0\0\0\0", 8) != 0)
				{
					return true;
				}
			}
			if (i>2)
			{
				//节表位于0 1 2三个序列后面,
				//驱动需要判断节表名字不是init
				if (pNtHeaders->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_NATIVE)
				{
					if (memcmp(pSection.Name, "INIT\0\0\0\0", 8) == 0)
						return false;
				}
				return true;
			}
			break;
		}
	}
	return false;
}

double CEntropyCalculator::CalcEntropy(const uint32_t byte_count[256], size_t total_length)
{
	const double xlog2 = 1.4426950408889634073599246810023;
	double entropy = 0.;
	for (uint32_t i = 0; i < 256; ++i)
	{
		double temp = static_cast<double>(byte_count[i]) / total_length;
		if (temp > 0.)
			entropy += std::abs(temp * (std::log(temp) * xlog2));
	}
	return entropy;
}

double CEntropyCalculator::CalcEntropy(const char* data, size_t length)
{
	uint32_t byte_count[256] = { 0 }; //Byte count for each of 255 bytes

	if (!length) //Don't calculate entropy for empty buffers
		return -1;
	//Count bytes
	for (size_t i = 0; i != length; ++i)//遇到UPX壳会访问越界崩溃
	{
		//int ntmp = static_cast<unsigned char>(data[i]);
		++byte_count[static_cast<unsigned char>(data[i])];
	}

	return CalcEntropy(byte_count, length);
}

BOOL CEntropyCalculator::CheckSection(PUCHAR image_base, BOOL bmem, LONGLONG llSize, int nSecIndex, double* pfent)
{
	RETURN_FALSE_IF_FAILED(pfent);
	RETURN_FALSE_IF_FAILED(image_base);

	PIMAGE_DOS_HEADER		pDosHeader = (PIMAGE_DOS_HEADER)image_base;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	PIMAGE_NT_HEADERS		pNtHeaders = (PIMAGE_NT_HEADERS)(image_base + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

	if(nSecIndex < 0 || nSecIndex >= pNtHeaders->FileHeader.NumberOfSections)
	{
		return FALSE;
	}

	PIMAGE_SECTION_HEADER	pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);
	const double pack_value = 6.75;//todo 这里是否改成可配置的
	const double normal_value = 6.5;
	int code_size = (pSectionHeader[nSecIndex].SizeOfRawData < pSectionHeader[nSecIndex].Misc.VirtualSize) ? pSectionHeader[nSecIndex].SizeOfRawData : pSectionHeader[nSecIndex].Misc.VirtualSize;
	if(code_size < 0)
	{
		return FALSE;
	}
	if(pSectionHeader[nSecIndex].PointerToRawData > llSize)//70_08d2d99e551ac1123a7009a23ec2e0e9的PointerToRawData大于文件大小，入口点异常，无法算熵
	{
		return FALSE;
	}
	PBYTE code_buffer = (PBYTE)(image_base + pSectionHeader[nSecIndex].PointerToRawData);
	if (bmem)
	{
		code_buffer = (PBYTE)(image_base + pSectionHeader[nSecIndex].VirtualAddress);
	}
	
	if(pSectionHeader[nSecIndex].PointerToRawData + code_size > llSize)//UPX壳的描述的大小会超过文件本身的大小，会访问越界，所以这里修正下
	{
		code_size = llSize - pSectionHeader[nSecIndex].PointerToRawData;
	}
	while (code_size && code_buffer[code_size - 1] == 0)
	{
		code_size--;
	}

	*pfent = CalcEntropy((const char *)code_buffer, code_size);
	
	return TRUE;
}

BOOL CEntropyCalculator::CheckFileEntropy(PUCHAR image_base, BOOL bmem, LONGLONG llSize, double* pfent)
{
	PIMAGE_DOS_HEADER		pDosHeader = (PIMAGE_DOS_HEADER)image_base;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	PIMAGE_NT_HEADERS		pNtHeaders = (PIMAGE_NT_HEADERS)(image_base + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}
	PIMAGE_SECTION_HEADER	pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);
	while (image_base[llSize - 1] == 0)
	{
		llSize--;
	}

	*pfent = CalcEntropy((const char *)image_base, llSize);
	
	return TRUE;
}

BOOL CEntropyCalculator::CheckBlockEntropy(PUCHAR image_base, BOOL bmem, LONGLONG llSize)
{
	double dEntropy = 0.0;
	int nBlockCnt = 0;
	double fent = 0.0;
	m_nHighEntropyBlockCnt = 0;
	PIMAGE_DOS_HEADER		pDosHeader = (PIMAGE_DOS_HEADER)image_base;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	PUCHAR	pPos = (PUCHAR)(image_base + pDosHeader->e_lfanew);
	while(pPos - image_base + ENTROPY_BLOCK_SIZE < llSize)
	{
		nBlockCnt++;
		fent = CalcEntropy((const char *)pPos, ENTROPY_BLOCK_SIZE);
		if(fent > 0)
		{
			dEntropy += fent;
		}
		if(fent > m_dHighBlockEntropy)
		{
			m_nHighEntropyBlockCnt++;
		}
		pPos += ENTROPY_BLOCK_SIZE;
	}

	if(pPos - image_base < llSize)
	{
		nBlockCnt++;
		fent = CalcEntropy((const char *)pPos, llSize - (pPos - image_base));
		if(fent > 0)
		{
			dEntropy += fent;
		}
		if(fent > m_dHighBlockEntropy)
		{
			m_nHighEntropyBlockCnt++;
		}
	}
	//DEBUG_PRINT("blocks average entropy is %lf\n", dEntropy/(double)nBlockCnt);
	m_dHighEntropyBlockRatio = (double)m_nHighEntropyBlockCnt/(double)nBlockCnt;

	return TRUE;
}

BOOL CEntropyCalculator::CheckRsrcEntropy(PUCHAR image_base, BOOL bmem, LONGLONG llFileSize, double* pfent)
{
	RETURN_FALSE_IF_FAILED(pfent);
	RETURN_FALSE_IF_FAILED(image_base);
	*pfent = 0;
	double highfent = 0.0;

	PIMAGE_DOS_HEADER		pDosHeader = (PIMAGE_DOS_HEADER)image_base;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	PIMAGE_NT_HEADERS		pNtHeaders = (PIMAGE_NT_HEADERS)(image_base + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}
	PIMAGE_SECTION_HEADER	pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);
	const double pack_value = 6.75;
	const double normal_value = 6.5;
	for (int i = 0ul; i < pNtHeaders->FileHeader.NumberOfSections; i++)
	{
		if(strcmp(strlwr((char*)pSectionHeader[i].Name), ".rsrc") != 0 &&
			strcmp(strlwr((char*)pSectionHeader[i].Name), "rsrc") != 0)
		{
			continue;
		}
		int32_t code_size = (pSectionHeader[i].SizeOfRawData < pSectionHeader[i].Misc.VirtualSize) ? pSectionHeader[i].SizeOfRawData : pSectionHeader[i].Misc.VirtualSize;
		if(code_size < 0)
		{
			continue;
		}

		if(pSectionHeader[i].PointerToRawData > llFileSize)
		{
			continue;
		}

		PBYTE code_buffer = (PBYTE)(image_base + pSectionHeader[i].PointerToRawData);
		if (bmem)
		{
			code_buffer = (PBYTE)(image_base + pSectionHeader[i].VirtualAddress);
		}

		if(pSectionHeader[i].PointerToRawData + code_size > llFileSize)//资源节也会有RawSize越界的情况
		{
			code_size = llFileSize - pSectionHeader[i].PointerToRawData;
		}
		while (code_size && code_buffer[code_size - 1] == 0)
		{
			code_size--;
		}
		highfent = CalcEntropy((const char *)code_buffer, code_size);
		if(highfent > *pfent)
		{
			*pfent = highfent;
		}
	}

	return TRUE;
}

BOOL CEntropyCalculator::GetEntropyConfigPath()
{
	char *pPos = NULL;
	char szPathBuf[MAX_SIZE] = {0};
	memset(m_szEntropyFilePath, 0, MAX_SIZE);
	GetModuleFileName(NULL, szPathBuf, MAX_SIZE);
	pPos = strrchr(szPathBuf, '\\');
	*pPos = 0;
	sprintf_s(m_szEntropyFilePath, MAX_SIZE, "%s%s", szPathBuf, "\\entropy.ini");

	return TRUE;
}

BOOL CEntropyCalculator::ParseEntropyFile()
{
	GetEntropyIniValue("block", &m_dHighBlockEntropy);
	GetEntropyIniValue("section", &m_dHighSectionEntropy);//m_dHighSectionEntropy

	return TRUE;
}

BOOL CEntropyCalculator::GetEntropyIniValue(char* szItemName, double* pEntropy)
{
	char szEntropy[ENTROPY_VALUE_SIZE] = {0};
	GetPrivateProfileString("entropy", szItemName, NULL, szEntropy, sizeof(szEntropy), m_szEntropyFilePath);
	RETURN_FALSE_IF_FAILED(szEntropy[0]);
	strlwr(szEntropy);
	*pEntropy = atof(szEntropy);

	return TRUE;
}

double CEntropyCalculator::GetHighEntropySectionRatio()
{
	return m_dHighEntropySectionRatio;
}

double CEntropyCalculator::GetHighEntropyBlockRatio()
{
	return m_dHighEntropyBlockRatio;
}

//void CEntropyCalculator::CheckSection2(PUCHAR image_base, BOOL bmem = FALSE, int nSecIndex)
//{
//	if(nSecIndex < 0 || nSecIndex > pNtHeaders->FileHeader.NumberOfSections)
//	{
//		return;
//	}
//
//	PIMAGE_DOS_HEADER		pDosHeader = (PIMAGE_DOS_HEADER)image_base;
//	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
//	{
//		return;
//	}
//	PIMAGE_NT_HEADERS		pNtHeaders = (PIMAGE_NT_HEADERS)(image_base + pDosHeader->e_lfanew);
//	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
//	{
//		return;
//	}
//	PIMAGE_SECTION_HEADER	pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);
//	const double pack_value = 6.75;
//	const double normal_value = 6.5;
//	uint32_t byte_count[256] = { 0 };
//	size_t total_size = 0;
//	for (int i = 0ul; i < pNtHeaders->FileHeader.NumberOfSections; i++)
//	{
//		int code_size = (pSectionHeader[nSecIndex].SizeOfRawData < pSectionHeader[nSecIndex].Misc.VirtualSize) ? pSectionHeader[nSecIndex].SizeOfRawData : pSectionHeader[nSecIndex].Misc.VirtualSize;
//		PBYTE code_buffer = (PBYTE)(image_base + pSectionHeader[nSecIndex].PointerToRawData);
//
//		if (bmem)
//		{
//			code_buffer = (PBYTE)(image_base + pSectionHeader[nSecIndex].VirtualAddress);
//		}
//
//		while (code_size && code_buffer[code_size - 1] == 0)
//		{
//			code_size--;
//		}
//
//		for (size_t j = 0; j != code_size; ++j)
//			++byte_count[static_cast<unsigned char>(code_buffer[j])];
//		total_size += code_size;
//	}
//
//	double fent = CalcEntropy(byte_count, total_size);
//
//	if (fent > pack_value)
//	{
//		//必然存在被加壳！
//		printf("pe packed or crypted\r\n");
//	}
//	if (fent < normal_value)
//	{
//		printf("pe normal\r\n");
//	}
//	if (fent >= normal_value
//		&&fent <= pack_value)
//	{
//		printf("pe may packed or crypted\r\n");
//	}
//}
