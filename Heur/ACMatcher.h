/*
 * DTHEUR - "Heur Engine".
 */

#pragma once
#include "stdafx.h"
#include "HeurRuleMacro.h"
#include "acism.h"
#include "msutil.h"
#include "HeurSignParser.h"

//WINAPI
//typedef acism_bool (*PACISM_LOAD_SIGN)(const char *filename, MEMREF** ppRef, int* pSigNum, int nIndex);
typedef acism_bool (*PACISM_DUMP)(ACISM const* psp, PS_DUMP_TYPE pdt, FILE* out, MEMREF const* pattv);
typedef ACISM* (*PACISM_CREATE)(MEMREF const *strv, int nstrs);
typedef int (*PACISM_MORE)(ACISM const*, MEMREF const text, ACISM_ACTION *fn, void *fndata, int *state, int* pMatchCnt);
typedef void (*PACISM_DESTROY)(ACISM *psp);

typedef struct AC_AUTOMATA_INFO
{
	int nSigNum;
	ACISM* pAutoMata;
	MEMREF *szSig;
}AC_AUTOMATA_INFO, *PAC_AUTOMATA_INFO;

class CACMatcher
{
public:
	CACMatcher(void);
	~CACMatcher(void);

public:
	BOOL Register(ACISM *psp, MEMREF *szSig, int nSigNum, int nIndex);
	PAC_AUTOMATA_INFO GetAutomata(int nindex);
	BOOL ACLoadSign(const char *filename, MEMREF** ppRef, int* pSigNum, int nIndex);
	BOOL ACDump(ACISM const* psp, PS_DUMP_TYPE pdt, FILE* out, MEMREF const* pattv);
	ACISM* ACCreate(MEMREF const *strv, int nstrs);
	int ACMore(ACISM const*, MEMREF const text, ACISM_ACTION *fn, void *fndata, int *state, int* pMatchCnt);
	void ACDestroyAll();
	void SetHeurSignParser(CHeurSignParser*);

private:
	void ACDestroy(ACISM *psp);
	MEMREF* SigSplit(char *pText, char sep, int *pCount);

private:
	HMODULE m_hMod;
	CHeurSignParser* m_pHeurSignParser;
	//PACISM_LOAD_SIGN m_pACLoadSign;
	PACISM_DUMP m_pACDump;
	PACISM_CREATE m_pACCreate;
	PACISM_MORE m_pACMore;
	PACISM_DESTROY m_pACDestroy;
	PAC_AUTOMATA_INFO m_szAutoMata[MAX_AUTOMATA_NUM];
};
