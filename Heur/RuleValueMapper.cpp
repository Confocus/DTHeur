#include "RuleValueMapper.h"

CRuleValueMapper::CRuleValueMapper(void)
{
	
}

CRuleValueMapper::~CRuleValueMapper(void)
{
}

HRule_NAME CRuleValueMapper::find(char* szHRName)
{
	std::map<std::string, HRule_NAME>::iterator it =  m_mapHRuleName.find(szHRName);
	if(it != m_mapHRuleName.end())
	{	
		return it->second;
	}

	return HR_NONE;
}

BOOL CRuleValueMapper::Init()
{
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("codesecattr", HR_CODESECATTR)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("entryattr", HR_ENTRYATTR)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("entrylastsec", HR_ENTRYLASTSEC)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("entryout", HR_ENTRYOUT)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("entryname", HR_ENTRYNAME)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("badsecname", HR_BADSECNAME)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("importfuncnum", HR_IMPORTFUNCNUM)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("overoptionalheadersize", HR_IMPORTFUNCNUM)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("tlssec", HR_TLSSEC)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("sizeofcodeerror", HR_SIZEOFCODEERR)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("multiexesec", HR_MULEXESEC)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("checksumerr", HR_CHECKSUM_ERROR)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("zerorawsizesec", HR_ZERO_RAWSIZE_SEC_NUM)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("sumsecsize", HR_SUM_SEC_SIZE)); 
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("impcorrupt", HR_IMP_CORRUPT));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("secrawaddralignerr", HR_SECRAWADDR_ALIGN_ERR));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("isrsrcexe", HR_RSRC_EXE));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("pointsymtab", HR_POINT_SYM_TAB_ERR));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("symnum", HR_SYM_NUM_ERR));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("ispacked", HR_IS_PACKED));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("entryentropy", HR_ENTTRY_ENTROPY));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("fileentropy", HR_FILE_ENTROPY));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("rsrcentropy", HR_HIGHEST_RSRC_ENTROPY));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("israwsizeabn", HR_RAW_SIZE_ABN));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("blockentropyratio", HR_RATIO_HIGH_ENTROPY_BLOCK));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("sectionentropyratio", HR_MULITI_HIGH_ENTROPY_SEC));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("lownumsec", HR_SEC_CNT));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("highnumsec", HR_SEC_CNT));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("exesecratio", HR_EXE_SEC_RATIO));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("timestamperr", HR_TIME_STAMP_ERR));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("isallunexesec", HR_ALL_UNEXE_SEC));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("zerodatadirnum", HR_ZERO_DATADIR_NUM));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("invaliddatadirnum", HR_INVALID_DATADIR_NUM));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("badsecnameratio", HR_BAD_SEC_RATIO));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("byteslastpage", HR_BYTE_LAST_PAGE));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("pageinfile", HR_PAGE_IN_FILE));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("iatnum", HR_IAT_NUM));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("isentryreloc", HR_ENTRY_RELOC));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("isentryrsrc", HR_ENTRY_RSRC));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("lastsecnamebad", HR_LAST_SEC_NAME));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("isonlyrsrcvirtualsize", HR_ONLY_RSRC_VS));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("repeatedsecname", HR_REPEATED_SEC_NAME));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("unprintsecnamenum", HR_UNPRINT_SEC_NAME));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("rdatawritable", HR_WRITABLE_RDATA));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("rdataexe", HR_EXE_RDATA));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("antiav", HR_ANTI_AV));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("clipboardact", HR_CLIPBOARD_ACT));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("serviceact", HR_SERVICE_ACT));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("hookact", HR_HOOK_ACT));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("reversevolume", HR_REVERSE_VOLUME_ACT));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("antidebug", HR_ANTIDEBUG));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("virtualmem", HR_VIRTUAL_MEM_OP));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("priviledgeop", HR_PRIVILEDGE_OP));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("internetop", HR_INTERNET_OP));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("scanbuff", HR_SCAN_BUFF));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("netenum", HR_NET_ENUM));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("qqspy", HR_QQ_SPY));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("midiop", HR_MIDI_OP));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("waveop", HR_WAVE_OP));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("exeinexe", HR_EXE_IN_EXE));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("totalcspsvc", HR_TOTAL_SCAN));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("getremoteaccount", HR_REMOTE_ACCOUNT));
	m_mapHRuleName.insert(std::pair<char*, HRule_NAME>("filesizepesizeabn", HR_FILESIZE_PESIZE_ABN));


	return TRUE;
}

BOOL CRuleValueMapper::Uninit()
{
	return TRUE;
}
