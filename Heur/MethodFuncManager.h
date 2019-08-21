#pragma once
#include "stdafx.h"
#include "HeurRuleMacro.h"
#include "ACMatcher.h"
#include <set>

class MethodFuncManager
{
public:
	MethodFuncManager(void);
	~MethodFuncManager(void);

public:
	/**
	* 方法函数1：精确匹配配置文件中的value对应的值
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodAccurateMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* 方法函数2：判断某个特征值是否满足某个范围
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodWithinRange(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* 方法函数3：
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodSignDbStringMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);


	/**
	* 方法函数4：判断特征是否在特征库对应的特征集中出现过
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodSignDbAnyofMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* 方法函数5：判断特征在特征库对应的特征集中出现的比率
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodSignDbRatioMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* 方法函数6：判断特征在特征库对应的特征集中出现的个数
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	//BOOL MethodSignDbCountMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch)
	//{
	//	return TRUE;
	//}

	/**
	* 方法函数6：用字符匹配算法去匹配某个Buf中是否出现某个特征集中的一个
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodACSearchMatch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);
	
	/**
	* 方法函数7：用字符匹配算法去匹配某个Buf中是否出现某个特征以及该特征的比率
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodACSearchMatchRatio(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* 方法函数8：用字符匹配算法去匹配某个Buf中是否出现某个特征集中的特征的条数
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodACSearchMatchNum(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* 方法函数9：用字符匹配算法去匹配某个Buf中是否出现某个特征以及该特征的次数
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodACSearchMatchCount(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* 方法函数10：用来搜索某个指令是否存在，支持模糊匹配
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodInstSearch(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	/**
	* 方法函数11：用来搜索某个指令出现的次数，支持模糊匹配
	*
	* @param PMETHOD_FUNC_IN_PARAM : 从样本中拿到的特征
	* @param PEXT_SIGN 从特征库或者hr.ini中拿到的特征值
	* @param PBOOL比较结果
	*
	* @return 
	*/
	static BOOL MethodInstSearchRatio(PMETHOD_PARAM pParam, PEXT_SIGN pExtSign, PBOOL pMatch);

	static BOOL Init(CHeurSignParser*);
	static BOOL Release();
private:
	
	/**
	* 用形如[,]和(,)的形式表示范围,[,](,)(,][,) 是否改用正则匹配
	*
	* @param result : 传入的结果值
	* @param szValue：义字符串形式表示的匹配范围
	* @param PBOOL：比较结果
	*
	* @return 
	*/
	template<typename T>
	static BOOL MethodParseWithinRangeExpression(T result, char* szValue, PBOOL pMatch);

	static BOOL CreateUnicodeBuff(PMETHOD_PARAM ,MEMREF*);

	static BOOL FreeUnicodeBuff(MEMREF*);
	
	static int MethodGetSignNum(char* szSigns);

	static void InitSignPath();

public:
	static std::set<int> m_Set;

private:
	static char m_szHSignPath[MAX_SIZE];
	static CACMatcher* m_pACMatcher;
	static ACISM_ACTION* m_pResultCb;
	//static int m_nCount;
};
