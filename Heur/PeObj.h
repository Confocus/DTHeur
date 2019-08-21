#pragma once
#include <windows.h>
//#include "BaseTypeObj.h"
//#include "XStream.h"
#include "EntropyCalculator.h"

#pragma pack(push, 1)

struct CrcSigInfo 
{
	DWORD crc;
	DWORD dataSize;
};

typedef struct STORAGE_SIGNATURE
{
	DWORD    lSignature;
	WORD     iMajorVersion;
	WORD     iMinorVersion;
	DWORD    iExtraData;
	DWORD    iLength;
	//字符串编译环境版本号的长度由iLength决定，包含尾部0，且按4字节对齐。
	//BYTE     iVersionString[];
}STORAGE_SIGNATURE;

typedef struct STORAGE_HEADER
{
	BYTE      fFlags;
	BYTE      reserved;
	WORD      NumberOfStreams;
}STORAGE_HEADER;

typedef struct STREAM_HEADER
{
	DWORD    iOffset;  //相对于MetaData_raw的偏移	
	DWORD    iSize;
	BYTE     rcName[32];
	//流名称字符串得长度由iSize决定，包含尾部0，且按4字节对齐。
}STREAM_HEADER;


struct PeComInfo 
{
	LARGE_INTEGER cor20HdrOff;
	LARGE_INTEGER metaDataOff;
	LARGE_INTEGER streamOff;
	LARGE_INTEGER comDataOff;
	IMAGE_COR20_HEADER corHdr;

	// off 0x68
	STORAGE_SIGNATURE metaDataHdr;
	STORAGE_HEADER	storageHdr;
	STREAM_HEADER streamHdr[6];
};

#pragma pack(pop)

class CPeObj /*: public CBaseTypeObj*/
{
public:
	CPeObj();
	~CPeObj();
	//static BOOL IsPe(IXInputStream* input);
	static BOOL IsPe(char* szBuf, LONGLONG llFileSize);

	//int Init(IXInputStream* input);
	int Init(char* szBuf, LONGLONG llSize);
	BOOL GetPEBuf(char** ppBuf, LONGLONG* pllSize);
	DWORD GetPeFlag();

	DWORD GetEntry() { return m_entry; }
	LARGE_INTEGER GetSize() { 
		return m_fileSize; 
	}
	IMAGE_DOS_HEADER* GetDosHdr() { return &m_dosHdr; }
	int GetSectionNum() { return m_ntHdr.FileHeader.NumberOfSections; }
	IMAGE_SECTION_HEADER* GetSecHdr() { return m_pSecHdr; }
	LARGE_INTEGER GetImageSize() { return m_realImageSize; }
	IMAGE_DATA_DIRECTORY* GetDataDirectory() { return m_pDataDirectory; }
	PeComInfo* GetComInfo() { return m_pComInfo; }
	int GetExeSecNum(){return m_execSecNum;}
	int GetSizeOfCode(){return m_nSizeOfCode;}
	int GetDataDirNum(){return m_dataEntryNum;}
	//char* GetBuf(){return m_szBuf;}
	int GetSizeOfOptionalHead(){return m_nSizeOfOptionalHead;}
	LARGE_INTEGER GetRawSizeSum(){return m_llSumRawSize;}
	BOOL IsImpCorrupt(){return m_bImpCorrupt;}
	int GetImpFuncNum(){return m_nImpFuncNum;}
	BOOL CalcImpFuncNum(char *szBuf);
	BOOL CalcCheckSum(char* szBuf, int nSize);
	DWORD RvaToOff(DWORD rva);
	BOOL IsCheckSumError();
	int GetZeroRawSizeSecNum();
	BOOL IsCodeSecAttrAbn(){return m_bCodeSecAttrAbn;}
	BOOL IsRawAddrAlignErr(){return m_bIsRawAddrAlignErr;}
	BOOL IsRsrcExe(){return m_bIsRsrcExe;}
	IMAGE_SECTION_HEADER* GetRvaOfSection(DWORD rva);

	IMAGE_SECTION_HEADER* GetOffOfSection(DWORD off);

	DWORD GetDataCrc32(DWORD index, DWORD& outDataSize);
	
	int GetPointerToSymbolTable(){return m_nPointToSymTab;}
	int GetNumberOfSymbols(){return m_nNumOfSym;}
	BOOL IsPacked(){return m_bIsPacked;}
	double GetEntryEntropy(){return m_dEntryEntropyValue;}
	double GetFileEntropy(){return m_dFileEntropy;}
	double GetRsrcEntropy(){return m_dRsrcEntropy;}
	int GetEntryOfSectionIndex(){return m_nEntryIndex;}
	BOOL IsRawSizeAbn(){return m_bIsRawSizeAbn;}
	BOOL GetRawSizeAbnNum(){return m_nRawSizeAbnNum;}
	double GetHighEntropyBlockRatio(){return m_pEntropyCalculator->GetHighEntropyBlockRatio();}
	double GetHighEntropySectionRatio(){return m_pEntropyCalculator->GetHighEntropySectionRatio();}
	int GetSecCnt(){return m_nSecCnt;}
	double GetExeSecRatio(){return m_dExeSecRatio;}
	DWORD GetTimeStamp(){return m_dwTimeStamp;}
	BOOL IsAllUnexeSec(){return m_bAllUnexe;}
	int GetZeroDataDirNum(){return m_nZeroSizeDataDirNum;}
	int GetInvalidDataDirNum(){return m_nInvalidDataDirNum;}
	int GetByteLastPage(){return m_nByteLastPage;}
	int GetPageInFile(){return m_nPageInFile;}
	int GetIATNum(){return m_nIATNum;}
	BOOL IsEntryReloc(){return m_bIsEntryReloc;}
	BOOL IsEntryRsrc(){return m_bIsEntryRsrc;}
	BOOL IsOnlyRsrcVirtualSize(){return m_bIsOnlyRsrcVirtualSize;}
	BOOL HasRepeatedSecName(){return m_bHasRepeatedSecName;}
	int GetUnprintableSecNameNum(){return m_nUnprintSecNameNum;}
	BOOL IsRdataWritable(){return m_bRdataWritable;}
	BOOL IsRdataExe(){return m_bRdataExe;}
	BOOL IsFileSizePESizeAbn(){return m_bIsFileSizePESizesAbn;};

private:
	void Clean();
	void CheckIsOnlyRsrcVirtualSize(IMAGE_SECTION_HEADER*, int);
	void CalcSectionInfo();
	void CheckEntryRsrc();
	void CheckEntryReloc();
	int GetDataDirSecIndex(int);
	void SetPEBuf(char*);
	void SetTimeStamp(DWORD);
	void SetByteLastPage(int);
	void SetDosHdr(IMAGE_DOS_HEADER*);
	void SetSecHdrInfo(IMAGE_NT_HEADERS* ntHdr);
	void SetSectionInfo();
	void SetDataDirInfo(IMAGE_NT_HEADERS*);
	void CalcInvalidDataDir();
	void SetNtHdrInfo(IMAGE_NT_HEADERS*);
	void SetEntryPointInfo(IMAGE_NT_HEADERS*);
	void SetSizeOfCode(IMAGE_NT_HEADERS*);
	void SetCheckSum(IMAGE_NT_HEADERS*);
	void SetPageInFile(int);
	//void SetDataDirectoryInfo();
	void SetIsAllUnexeSec(BOOL);
	void SetExeSecRatio(double);
	void SetSecCnt();
	void SetEntryOfSectionIndex();
	ULONG RvaToOffset(IMAGE_SECTION_HEADER*, ULONG ulImpTableVA);
	void SetSectionAttr(WORD secNum);
	void ReadDotNetSymbol();
	DWORD GetEntryCrc32(DWORD& outDataSize);
	DWORD GetEntryOfSectionCrc32(DWORD& outDataSize);
	DWORD GetEntryOfSectionFirst200Crc32(DWORD& outDataSize);
	DWORD GetEntryBefore200Crc32(DWORD& outDataSize);
	DWORD GetCurrentOffDataCrc32(DWORD& outDataSize);
	void SetPointerToSymbolTable();
	void SetNumberOfSymbols();
	DWORD GetDataCrc(DWORD seed, LARGE_INTEGER off, DWORD size);
	DWORD CheckDataSize(DWORD off, DWORD size)
	{
		return  ( (off + size) < m_fileSize.LowPart ? size : (m_fileSize.LowPart - off) );
	}
	BOOL GetEntropyInfo(PUCHAR szBuf, LONGLONG llFileSizes);

private:
	BOOL m_bIsPacked;
	IMAGE_DOS_HEADER m_dosHdr;
	union 
	{
		IMAGE_NT_HEADERS m_ntHdr;
		IMAGE_NT_HEADERS64 m_ntHdr64;
	};

	//IXInputStream* m_inputObj;
	
	IMAGE_SECTION_HEADER *m_pSecHdr;
	IMAGE_SECTION_HEADER *m_pOrgSecHdr;
	IMAGE_SECTION_HEADER *m_pEntrySecHdr;	// 入口点所在节表
	DWORD m_dataEntryNum;
	IMAGE_DATA_DIRECTORY* m_pDataDirectory;
	DWORD m_entry;
	bool m_is64;
	DWORD m_flag;
	DWORD m_typeFlag;
	DWORD m_execSecNum;
	LARGE_INTEGER m_realImageSize;

	PeComInfo* m_pComInfo;
	PeComInfo m_comInfo;
	char* m_pPEBuf;
	DWORD m_currentOff;

	LARGE_INTEGER m_fileSize;
	CrcSigInfo m_crcSigList[6];

	//char* m_szBuf;
	int m_nSizeOfCode;
	int m_nSizeOfOptionalHead;
	int m_nImpFuncNum;
	int m_nIATNum;
	int m_nCheckSum;
	int m_nCalcedCheckSum;
	int m_nSecCnt;
	int m_nZeroRawSizeSecNum;
	double m_dZeroRawSizeSecRatio;

	LARGE_INTEGER m_llSumRawSize;
	BOOL m_bAllUnexe;
	BOOL m_bImpCorrupt;
	BOOL m_bCodeSecAttrAbn;
	BOOL m_bIsRawAddrAlignErr;
	BOOL m_bIsRsrcExe;
	int m_nPointToSymTab;
	int m_nNumOfSym;
	double m_dEntryEntropyValue;
	double m_dFileEntropy;
	double m_dRsrcEntropy;
	double m_dExtraEntropy;
	double m_dRelocEntropy;
	double m_dHighEntropyBlockRatio;
	double m_dHighEntropySectionRatio;
	double m_dExeSecRatio;
	int m_nEntryIndex;
	BOOL m_bIsRawSizeAbn;
	int m_nRawSizeAbnNum;
	DWORD m_dwTimeStamp;
	int m_nZeroSizeDataDirNum;
	int m_nInvalidDataDirNum;
	int m_nByteLastPage;
	int m_nPageInFile;
	BOOL m_bIsEntryRsrc;
	BOOL m_bIsEntryReloc;
	BOOL m_bIsOnlyRsrcVirtualSize;
	BOOL m_bHasRepeatedSecName;
	BOOL m_bRdataWritable;
	BOOL m_bRdataExe;
	BOOL m_bIsFileSizePESizesAbn;
	int m_nUnprintSecNameNum;
	CEntropyCalculator *m_pEntropyCalculator;
};