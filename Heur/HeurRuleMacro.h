#pragma once
#include "stdafx.h"
#include "PEParser.h"

#define INVALID_RETURN_VALUE	-1
#define HR_VALUE_SIZE	0x100
#define HR_NAME_SIZE	0x100
#define HR_SIGN_SIZE	0x500
#define HR_EACH_SIGN_SIZE	0x50

#define MAX_HR_METHOD_COUNT	10
#define MAX_HR_CUST_FUNC_COUNT	10
#define MAX_SIZE	0x500
#define MAX_HEUR_RULE_COUNT	99
#define MAX_SIGN_SIZE 0x500
#define MAX_SIGN_CNT	0x100
#define MAX_INDEX_VALUE	0x100
#define MAX_SEC_NAME_SIZE	0x100
#define MAX_COMB_RULE_CNT	0x100
#define MAX_COMB_RULE_SIZE	0x100
#define MAX_AUTOMATA_NUM	MAX_SIGN_CNT

typedef enum
{
	METHOD_NONE = 0,
	METHOD_ACCURATE_MATCH = 1,	//匹配某个值是否相等
	METHOD_WITHIN_RANGE = 2,	//匹配某个值是否在某个范围当中
	METHOD_STRSEQ_MATCH = 3,		//匹配这个字符串或API序列的匹配程度
	METHOD_ANYOF = 4,	//匹配某个值是否在这些集合之中
	METHOD_SIGNDB_RATIO = 5,//匹配某个特征在特征库中对应的特征集中出现的比率
	METHOD_AC_SEARCH = 6,
	METHOD_AC_SEARCH_RATIO = 7,
	METHOD_AC_SEARCH_NUM = 8,
	METHOD_AC_SEARCH_CNT = 9,
	METHOD_CUST = 10,	//定制化的匹配方法
	METHOD_END = 99
}HRule_Method;

typedef enum
{
	HR_NONE = 0,
	HR_CODESECATTR,//done
	HR_ENTRYATTR,//done
	HR_ENTRYLASTSEC,//done
	HR_ENTRYOUT,//done
	HR_ENTRYNAME,//done
	HR_BADSECNAME,//done
	HR_IMPORTFUNCNUM,//7
	HR_TLSSEC,//done
	HR_OVEROPHEADSIZE,//done
	HR_SIZEOFCODEERR,//done
	HR_MULEXESEC,//done:多个具备可执行属性的节区
	HR_MULITI_HIGH_ENTROPY_SEC,//done
	HR_ENTTRY_ENTROPY,//done13
	HR_RATIO_HIGH_ENTROPY_BLOCK,//done
	HR_CHECKSUM_ERROR,//done15
	HR_ZERO_RAWSIZE_SEC_NUM,//done
	HR_SUM_SEC_SIZE,//done
	HR_IMP_CORRUPT,//done
	HR_SECRAWADDR_ALIGN_ERR,//done
	HR_RSRC_EXE,//done
	HR_POINT_SYM_TAB_ERR,//done 判断PointerOfSymbolTable的值是否正确
	HR_SYM_NUM_ERR,//done 判断NumberOfSymbols的值是否正确
	HR_IS_PACKED,//done
	HR_FILE_ENTROPY,//done
	HR_HIGHEST_RSRC_ENTROPY,//done
	HR_RAW_SIZE_ABN,//done
	HR_SEC_CNT,//done
	HR_EXE_SEC_RATIO,
	HR_TIME_STAMP_ERR,
	HR_ALL_UNEXE_SEC,
	HR_ZERO_DATADIR_NUM,
	HR_INVALID_DATADIR_NUM,
	HR_BAD_SEC_RATIO,
	HR_BYTE_LAST_PAGE,
	HR_PAGE_IN_FILE,
	HR_IAT_NUM,
	HR_ENTRY_RSRC,
	HR_ENTRY_RELOC,
	HR_LAST_SEC_NAME,
	HR_ONLY_RSRC_VS,
	HR_REPEATED_SEC_NAME,
	HR_UNPRINT_SEC_NAME,
	HR_WRITABLE_RDATA,
	HR_EXE_RDATA,
	HR_ANTI_AV,
	HR_CLIPBOARD_ACT,
	HR_SERVICE_ACT,
	HR_HOOK_ACT,
	HR_REVERSE_VOLUME_ACT,
	HR_ANTIDEBUG,
	HR_VIRTUAL_MEM_OP,
	HR_PRIVILEDGE_OP,
	HR_INTERNET_OP,
	HR_SCAN_BUFF,
	HR_NET_ENUM,
	HR_QQ_SPY,
	HR_MIDI_OP,
	HR_WAVE_OP,
	HR_EXE_IN_EXE,
	HR_TOTAL_SCAN,
	HR_REMOTE_ACCOUNT,
	HR_FILESIZE_PESIZE_ABN,
	HR_END = 99
}HRule_NAME;

typedef struct BUF_REF
{
	char* ptr;
	LONGLONG len;
}BUF_REF, *PBUF_REF;

typedef struct MethodParam
{
	MethodParam()
	{
		nType = 0;
		nIndex = 0;
		nResult = 0;
		dResult = 0.0;
		pHSignParser = NULL;
		BufResult.ptr = NULL;
		BufResult.len = 0;
		memset(szResult, 0, HR_SIGN_SIZE);
		memset(szValue, 0, HR_VALUE_SIZE);
	}

	int nType;
	int nIndex;
	int nResult;
	BUF_REF BufResult; 
	void* pHSignParser;
	double dResult;
	char szValue[HR_VALUE_SIZE];
	char szResult[HR_SIGN_SIZE];
}METHOD_PARAM, *PMETHOD_PARAM;

//不要改为union，后边你也不知道用哪个，还是得都赋值
typedef struct ExtSign
{
	ExtSign()
	{
		memset(szValue, 0, HR_VALUE_SIZE);
		memset(szSign, 0, HR_SIGN_SIZE);
	}
	char szValue[HR_VALUE_SIZE];
	char szSign[HR_SIGN_SIZE];
}EXT_SIGN, *PEXT_SIGN;

typedef struct EXT_FUNC_IN_PARAM
{
	int nIndex;
	int nWeight;
	HRule_Method nMethod;
	char* szValue[HR_VALUE_SIZE];
}EXT_FUNC_IN_PARAM, *PEXT_FUNC_IN_PARAM;

typedef enum
{
	INVALID_TYPE = 0,
	TYPE_INT_VALUE,
	TYPE_DOUBLE_VALUE,
	TYPE_ARRAY_VALUE,
	TYPE_POINTER_VALUE
}RESULT_TYPE;


typedef struct EXT_FUNC_OUT_PARAM
{
	EXT_FUNC_OUT_PARAM()
	{
		nType = INVALID_TYPE;
		nResult = 0;
		nNum = 0;
		dResult = 0.0;
		dRatio = 0.0;
		BufResult.ptr = NULL;
		BufResult.len = 0;
		memset(szResult, 0, HR_SIGN_SIZE);
	}
	RESULT_TYPE nType;//结果Result类型
	int nResult;
	int nNum;//命中某个规则的个数
	double dResult;
	double dRatio;//命中某个规则的比率
	BUF_REF BufResult;
	char szResult[HR_SIGN_SIZE];//HR_SIGN_SIZE用来存储某条特征，用SIGN_SIZE
}EXT_FUNC_OUT_PARAM, *PEXT_FUNC_OUT_PARAM;

#define METHOD_FUNC_IN_PARAM EXT_FUNC_OUT_PARAM
#define PMETHOD_FUNC_IN_PARAM PEXT_FUNC_OUT_PARAM

typedef struct EXT_FUNC_PARAM
{
	EXT_FUNC_IN_PARAM	InParam;
	EXT_FUNC_OUT_PARAM	OutParam;
}EXT_FUNC_PARAM, *PEXT_FUNC_PARAM;

typedef BOOL (*PExtSignCbFunc)(PEXT_FUNC_PARAM); 
typedef BOOL (*PMethodCbFunc)(PMETHOD_PARAM, PEXT_SIGN, PBOOL); //EXT_FUNC_OUT_PARAM

#define SAFE_DELETE_POINTER(p)	if(p){delete p; p = NULL;}//todo 检查这里的p是否应该是*p？
#define RETURN_FALSE_IF_FAILED(p)		if(!p){\
	printf("False return——File:%s, Line:%d, Function:%s, ErrorCode:%d\n", __FILE__, __LINE__, __FUNCTION__, GetLastError()); \
	return FALSE;\
	}
#define RETURN_IF_FAILED(p)		if(!p){return;}

#define DEBUG_PRINT	printf

#define HIT_RULE_FLAG_SIZE	HR_END/(sizeof(int) * 8) + 1