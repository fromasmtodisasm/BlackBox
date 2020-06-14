#pragma once

#ifndef _SCRIPTABLE_H_
#define _SCRIPTABLE_H_

#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/MathHelper.hpp>
#include <vector>
#include <string>
//#define _NO_HASHMAP
//#define NO_USERDATA_FOR_PROPERTIES
#ifndef _NO_HASHMAP
#include <map>
#else
#if defined(LINUX)
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#endif

#if !defined(_WIN32) || !defined(_WIN64)
#define OutputDebugString(str) GetISystem()->Log(str)
#endif

struct ScriptBase
{
  virtual ~ScriptBase() {}
};

//! Helper template class.
//! Handles dispatching of Lua to C script function callbacks to the specified
//! member functions of the CScriptableBase.
// See Also: CScriptableBase::RegisterTemplateFunction
struct ScriptTemplateCallHelper
{
	template<typename Callee, typename P0>
	static int Call(Callee* callee, P0 (Callee::* func)(), IFunctionHandler* pH)
	{
		return pH->EndFunction((callee->*func)());
	}
	template<typename Callee, typename P0, typename P1>
	static int Call(Callee* callee, P0 (Callee::* func)(P1), IFunctionHandler* pH)
	{
		P1 p1;
		if (!pH->GetParams(p1))
			return pH->EndFunction();
		return pH->EndFunction((callee->*func)( p1));
	}
	template<typename Callee, typename P0, typename P1, typename P2>
	static int Call(Callee* callee, P0 (Callee::* func)(P1, P2), IFunctionHandler* pH)
	{
		P1 p1;
		P2 p2;
		if (!pH->GetParams(p1, p2))
			return pH->EndFunction();
		return pH->EndFunction((callee->*func)( p1, p2));
	}
	template<typename Callee, typename P0, typename P1, typename P2, typename P3>
	static int Call(Callee* callee, P0 (Callee::* func)(P1, P2, P3), IFunctionHandler* pH)
	{
		P1 p1;
		P2 p2;
		P3 p3;
		if (!pH->GetParams(p1, p2, p3))
			return pH->EndFunction();
		return pH->EndFunction((callee->*func)( p1, p2, p3));
	}
	template<typename Callee, typename P0, typename P1, typename P2, typename P3, typename P4>
	static int Call(Callee* callee, P0 (Callee::* func)(P1, P2, P3, P4), IFunctionHandler* pH)
	{
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		if (!pH->GetParams(p1, p2, p3, p4))
			return pH->EndFunction();
		return pH->EndFunction((callee->*func)( p1, p2, p3, p4));
	}
	template<typename Callee, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
	static int Call(Callee* callee, P0 (Callee::* func)(P1, P2, P3, P4, P5), IFunctionHandler* pH)
	{
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
		if (!pH->GetParams(p1, p2, p3, p4, p5))
			return pH->EndFunction();
		return pH->EndFunction((callee->*func)( p1, p2, p3, p4, p5));
	}
	template<typename Callee, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static int Call(Callee* callee, P0 (Callee::* func)(P1, P2, P3, P4, P5, P6), IFunctionHandler* pH)
	{
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
		P6 p6;
		if (!pH->GetParams(p1, p2, p3, p4, p5, p6))
			return pH->EndFunction();
		return pH->EndFunction((callee->*func)( p1, p2, p3, p4, p5, p6));
	}
	template<typename Callee, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static int Call(Callee* callee, P0 (Callee::* func)(P1, P2, P3, P4, P5, P6, P7), IFunctionHandler* pH)
	{
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
		P6 p6;
		P7 p7;
		if (!pH->GetParams(p1, p2, p3, p4, p5, p6, p7))
			return pH->EndFunction();
		return pH->EndFunction((callee->*func)( p1, p2, p3, p4, p5, p6, p7));
	}
	template<typename Callee, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static int Call(Callee* callee, P0 (Callee::* func)(P1, P2, P3, P4, P5, P6, P7, P8), IFunctionHandler* pH)
	{
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
		P6 p6;
		P7 p7;
		P8 p8;
		if (!pH->GetParams(p1, p2, p3, p4, p5, p6, p7, p8))
			return pH->EndFunction();
		return pH->EndFunction((callee->*func)( p1, p2, p3, p4, p5, p6, p7, p8));
	}
	template<typename Callee, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	static int Call(Callee* callee, P0 (Callee::* func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), IFunctionHandler* pH)
	{
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
		P6 p6;
		P7 p7;
		P8 p8;
		P9 p9;
		if (!pH->GetParams(p1, p2, p3, p4, p5, p6, p7, p8, p9))
			return pH->EndFunction();
		return pH->EndFunction((callee->*func)( p1, p2, p3, p4, p5, p6, p7, p8, p9));
	}
	template<typename Callee, typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
	static int Call(Callee* callee, P0 (Callee::* func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), IFunctionHandler* pH)
	{
		P1 p1;
		P2 p2;
		P3 p3;
		P4 p4;
		P5 p5;
		P6 p6;
		P7 p7;
		P8 p8;
		P9 p9;
		P10 p10;
		if (!pH->GetParams(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10))
			return pH->EndFunction();
		return pH->EndFunction((callee->*func)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10));
	}

	template<typename Callee, typename Func>
	struct CallDispatcher
	{
		static int Dispatch(IFunctionHandler* pH)
		{
			Callee* pCallee = (Callee*)pH->GetThis();
			Func func;
			auto i = pH->GetFunctionID();
			memcpy(&func, &i, sizeof(func));
			gEnv->pLog->Log("i = %d; c = %c", 100, 'A');	
			return ScriptTemplateCallHelper::Call(pCallee, func, pH);
			return 0;
		}
	};
};


//////////////////////////////////////////////////////////////////////
template<class T>
class _ScriptableEx : public ScriptBase
{
public:
  _ScriptableEx()
  {
    m_pScriptThis = NULL;
    m_pScriptSystem = NULL;
    m_nBase = NULL;
  }

  virtual ~_ScriptableEx()
  {
    if (m_pScriptThis)
    {
      m_pScriptThis->SetNativeData(NULL);
      //m_pScriptThis->AddSetGetHandlers(NULL, NULL);
      m_pScriptThis->Release();
    }
  }

  typedef int(T::* MemberFunc)(IFunctionHandler* pH);

  typedef std::vector<MemberFunc> FunctionsVec;
  typedef typename FunctionsVec::iterator FunctionsVecItor;

  //////////////////////////////////////////////////////////////////////
  enum PropertyType
  {
    PROPERTY_TYPE_BOOL,
    PROPERTY_TYPE_INT,
    PROPERTY_TYPE_CHAR,
    PROPERTY_TYPE_FLOAT,
    PROPERTY_TYPE_STRING,
    PROPERTY_TYPE_OBJECT
  };

  //////////////////////////////////////////////////////////////////////
  struct Property
  {
    Property()
    {
      val = 0;
    }
    Property(const Property& p)
    {
      nType = p.nType;
      val = p.val;
    }
    PropertyType nType;
    unsigned int val;
  };

  UINT_PTR m_nBase;

  typedef std::vector<Property*> PropertiesVec;
  typedef typename PropertiesVec::iterator PropertiesVecItor;

  void Init(IScriptSystem* pScriptSystem, T* pParent)
  {
    m_pScriptSystem = pScriptSystem;
    _ScriptableEx<T>::m_pFunctionHandler = m_pScriptSystem->GetFunctionHandler();
    if (!_ScriptableEx<T>::m_pFunctionHandler)
      CryError("Scriptable EX:FUNCTION HANDLER NULL");
    m_pScriptThis = pScriptSystem->CreateObject();
    m_pScriptThis->SetNativeData(pParent);
    m_pScriptThis->Delegate(_ScriptableEx<T>::m_pTemplateTable);

    if (m_pScriptThis->GetNativeData() != pParent)
      CryError("Scriptable EX:Properties map");
  }

  void InitGlobal(IScriptSystem* pScriptSystem, const char* sName, T* pParent)
  {
    m_pScriptSystem = pScriptSystem;
    _ScriptableEx<T>::m_pFunctionHandler = m_pScriptSystem->GetFunctionHandler();
    if (!_ScriptableEx<T>::m_pFunctionHandler)
      CryError("Scriptable EX:FUNCTION HANDLER NULL");
    m_pScriptThis = pScriptSystem->CreateGlobalObject(sName);
    m_pScriptThis->SetNativeData(pParent);
    if (_ScriptableEx<T>::m_pTemplateTable)
      m_pScriptThis->Clone(_ScriptableEx<T>::m_pTemplateTable);

    if (m_pScriptThis->GetNativeData() != pParent)
      CryError("Scriptable EX:Init Global");
  }

  static void InitializeTemplate(IScriptSystem* pSS)
  {
    m_pSS = pSS;
  }

  static void ReleaseTemplate()
  {
    if (m_pTemplateTable != NULL)
    {
      m_pTemplateTable->Release();
      m_pTemplateTable = NULL;
    }
    if (m_pvPropertiesVector)
    {
      PropertiesVecItor itor;
      itor = m_pvPropertiesVector->begin();
      while (itor != m_pvPropertiesVector->end())
      {
        delete (*itor);
        itor++;
      }
      delete m_pvPropertiesVector;
    }
    m_pvPropertiesVector = NULL;
    if (m_pPropertiesTable)
      m_pPropertiesTable->Release();
    m_pPropertiesTable = NULL;
  }

  static void RegisterFunction(IScriptSystem* pSS, const char* sName, MemberFunc mfunc)
  {
    if (m_pTemplateTable == NULL)
    {
      m_pTemplateTable = pSS->CreateObject();
    }

    int nIdx = (int)m_vFuncs.size();
    m_vFuncs.push_back(mfunc);
    m_pTemplateTable->AddFunction(sName, _ScriptableEx<T>::FuncThunk, nIdx);
  }

	template<typename Callee, typename Func>
  void RegisterTemplateFunction(const char* sFuncName, const char* sFuncParams, Callee& callee, const Func& mfunc)
  {
    if (m_pTemplateTable == NULL)
    {
      m_pTemplateTable = m_pSS->CreateObject();
    }

    m_pTemplateTable->AddFunction(sFuncName, _ScriptableEx<T>::FuncThunk<Callee, Func>, *((INT_PTR*)(&mfunc)));
  }

  bool EnablePropertiesMapping(void* pBase)
  {
    assert(pBase);
    m_nBase = (UINT_PTR)pBase;
    return m_pScriptThis->AddSetGetHandlers(_ScriptableEx<T>::SetThunk, _ScriptableEx<T>::GetThunk);
  }

  static void AllowPropertiesMapping(IScriptSystem* pSS)
  {
    if (m_pPropertiesTable == NULL)
    {
      m_pPropertiesTable = pSS->CreateObject();
      m_pvPropertiesVector = new PropertiesVec;
    }
  }

  static void InsertProperty(const char* sName, Property& prop)
  {
#if 0
    USER_DATA ud;
    Property* p = new Property;
    m_pvPropertiesVector->push_back(p);
    *p = prop;
    ud = m_pSS->CreateUserData((INT_PTR)p, -1);
    m_pPropertiesTable->SetValue(sName, ud);
    //test
    int nTemp;
    ULONG_PTR nP;
    if (!m_pPropertiesTable->GetUDValue(sName, nP, nTemp))
      CryError("Scriptable EX:Insert Property (GetUDValue)");
    p = (Property*)nP;
    if (p->nType != prop.nType)
      CryError("Scriptable EX:Insert Property");
#endif // 0
  }

  static void RegisterProperty(const char* sName, PropertyType t, unsigned int offset)
  {
    Property prop;
    prop.nType = t;
    prop.val = offset;
    InsertProperty(sName, prop);
  }

  IScriptObject* GetScriptObject()
  {
    return m_pScriptThis;
  }

  IScriptObject* operator ->()
  {
    return m_pScriptThis;
  }

protected:

  static int FuncThunk(HSCRIPT h)
  {
    m_pFunctionHandler->__Attach(h);
    T* pThis = (T*)(m_pFunctionHandler->GetThis());
    if (!pThis)
    {
      m_pSS->RaiseError("Null Self");
      //::OutputDebugString("Null Self\n");
      return 0;
    }
    return (pThis->*(pThis->m_vFuncs[m_pFunctionHandler->GetFunctionID()]))(m_pFunctionHandler);
  }

	template<typename Callee, typename Func>
  static int FuncThunk(HSCRIPT h)
  {
    m_pFunctionHandler->__Attach(h);
    T* pThis = (T*)(m_pFunctionHandler->GetThis());
    if (!pThis)
    {
      m_pSS->RaiseError("Null Self");
      //::OutputDebugString("Null Self\n");
      return 0;
    }
		return ScriptTemplateCallHelper::CallDispatcher<Callee, Func>::Dispatch(m_pFunctionHandler);
  }

  static int SetThunk(HSCRIPT h)
  {
    m_pFunctionHandler->__Attach(h);
    T* pThis = (T*)m_pFunctionHandler->GetThis();
    const char* sIndex;
    if (pThis && m_pFunctionHandler->GetParam(1, sIndex))
    {
      int nCookie;
      USER_DATA nP;
      if (_ScriptableEx<T>::m_pPropertiesTable->GetUDValue(sIndex, nP, nCookie)/*pThis->m_pProperties->GetUDValue(sIndex,nP,nCookie)*/)
      {
        Property* prop = ((Property*)nP);
        if (nCookie != -1)
          CryError("Scriptable EX:Set Thunk");
        assert(pThis->m_nBase);
        void* val = (void*)(pThis->m_nBase + prop->val);
        switch (prop->nType)
        {
        case PROPERTY_TYPE_INT:
          m_pFunctionHandler->GetParam(2, (*((int*)val)));
          return 0;
          break;
        case PROPERTY_TYPE_FLOAT:
          m_pFunctionHandler->GetParam(2, (*((float*)val)));
          return 0;
          break;
        case PROPERTY_TYPE_CHAR:
        {
          const char* sTemp = NULL;
          m_pFunctionHandler->GetParam(2, sTemp);
          if (!sTemp)
            return -1;
          strcpy(((char*)val), sTemp);
        }
        return 0;
        break;
        case PROPERTY_TYPE_STRING:
        {
          const char* sTemp = NULL;
          m_pFunctionHandler->GetParam(2, sTemp);
          if (!sTemp)
            return -1;
#if 0
          (*((string*)val)) = sTemp;
#endif // 0
        }
        return 0;
        break;
        case PROPERTY_TYPE_BOOL:
          m_pFunctionHandler->GetParam(2, (*((bool*)val)));
          return 0;
          break;
        case PROPERTY_TYPE_OBJECT:
          m_pFunctionHandler->GetParam(2, ((IScriptObject*)val));
          return 0;
          break;
        default:
          return -1;
          break;
        }
      }
    }
    //returning -1 means that the script engine must manage this property himself
    return -1;
  }

  static int GetThunk(HSCRIPT h)
  {
    static char cTemp[200];
    m_pFunctionHandler->__Attach(h);
    T* pThis = (T*)m_pFunctionHandler->GetThis();
    const char* sIndex;
    if (pThis && m_pFunctionHandler->GetParam(1, sIndex))
    {
      int nCookie;
      USER_DATA nP;
      if (_ScriptableEx<T>::m_pPropertiesTable->GetUDValue(sIndex, nP, nCookie))
      {
        Property* prop = ((Property*)nP);
        if (nCookie != -1)
          CryError("Scriptable EX:GetParam:GetUdValue");
        assert(pThis->m_nBase);
        void* val = (void*)(pThis->m_nBase + prop->val);
        switch (prop->nType)
        {
        case PROPERTY_TYPE_INT:
          return m_pFunctionHandler->EndFunction((*((int*)val)));
          break;
        case PROPERTY_TYPE_FLOAT:
          return m_pFunctionHandler->EndFunction((*((float*)val)));
          break;
        case PROPERTY_TYPE_CHAR:
          return m_pFunctionHandler->EndFunction((const char*)val);
          break;
        case PROPERTY_TYPE_STRING: {
          return -1;//return m_pFunctionHandler->EndFunction((*((string*)val)).c_str());
        }
                                 break;
        case PROPERTY_TYPE_BOOL:
          return m_pFunctionHandler->EndFunction((*((bool*)val)));
          break;
        case PROPERTY_TYPE_OBJECT:
          return m_pFunctionHandler->EndFunction((((IScriptObject*)val)));
          break;
        default:
          return -1;
          break;
        }
      }
    }
    //returning -1 means that the script engine must manage this property himself
    return -1;
  }

  IScriptObject* m_pScriptThis;
  IScriptSystem* m_pScriptSystem;

  static FunctionsVec m_vFuncs;
  static IScriptObject* m_pTemplateTable;
  static IScriptObject* m_pPropertiesTable;
  static PropertiesVec* m_pvPropertiesVector;
  static IFunctionHandler* m_pFunctionHandler;
  static IScriptSystem* m_pSS;
};

/////////////////////////////////////////////////////////////////////////////
#if defined(LINUX) || defined(__MINGW32__)
#define _DECLARE_SCRIPTABLEEX(_class) template<> IFunctionHandler * _ScriptableEx<_class>::m_pFunctionHandler=NULL; \
		template<> _ScriptableEx<_class>::FunctionsVec _ScriptableEx<_class>::m_vFuncs = _ScriptableEx<_class>::FunctionsVec(); \
		template<> IScriptObject *_ScriptableEx<_class>::m_pTemplateTable=NULL; \
		template<> IScriptObject *_ScriptableEx<_class>::m_pPropertiesTable=NULL; \
		template<> IScriptSystem *_ScriptableEx<_class>::m_pSS=NULL; \
		template<> _ScriptableEx<_class>::PropertiesVec *_ScriptableEx<_class>::m_pvPropertiesVector=NULL;
#else
#define _DECLARE_SCRIPTABLEEX(_class) template<> IFunctionHandler * _ScriptableEx<_class>::m_pFunctionHandler=NULL; \
		_ScriptableEx<_class>::FunctionsVec _ScriptableEx<_class>::m_vFuncs; \
		IScriptObject *_ScriptableEx<_class>::m_pTemplateTable=NULL; \
		IScriptObject *_ScriptableEx<_class>::m_pPropertiesTable=NULL; \
		IScriptSystem *_ScriptableEx<_class>::m_pSS=NULL; \
		_ScriptableEx<_class>::PropertiesVec *_ScriptableEx<_class>::m_pvPropertiesVector=NULL;
#endif	//LINUX

#define CHECK_PARAMETERS_SS(_pSS,_n) \
	if (pH->GetParamCount() != _n) \
{  \
	_pSS->RaiseError("%s: %d arguments passed, " #_n " expected)", __FUNCTION__, pH->GetParamCount()); \
	return pH->EndFunctionNull(); \
}

// NOTE
// The following RegExp replace macro was used to change to the new check_parameters
// Find: CHECK_PARAMETERS(:Wh)*\([^,]+,(:Wh)*{[0-9]+}\)
// Repl: CHECK_PARAMETERS(\1)

#ifndef __INTEL_COMPILER
#define CHECK_PARAMETERS(_n) CHECK_PARAMETERS_SS(m_pScriptSystem,_n)
#else
// For Intel Compiler.
#define CHECK_PARAMETERS(_n) \
	if (pH->GetParamCount() != _n) \
	{  \
		m_pScriptSystem->RaiseError( ": %d arguments passed, " #_n " expected)", pH->GetParamCount()); \
		return pH->EndFunctionNull(); \
	}
#endif
#define SCRIPT_CHECK_PARAMETERS(_n) CHECK_PARAMETERS(_n)
#define SCRIPT_REG_CLASSNAME
#define SCRIPT_REG_FUNC(_func)  SCRIPT_REG_CLASSNAME::RegisterFunction(m_pSS,# _func, &SCRIPT_REG_CLASSNAME::_func);
#define REG_FUNC(_class,_func) _class::RegisterFunction(m_pSS,#_func,&_class::_func);
#define SCRIPT_REG_TEMPLFUNC(func, sFuncParams) RegisterTemplateFunction( # func, sFuncParams, *this, &SCRIPT_REG_CLASSNAME::func);
#define REG_DERIVED_FUNC(_class,_func) RegisterFunction(m_pSS,#_func,&_class::_func);
#define SCRIPT_REG_CONST_SS(_pSS, _const) m_pSS->SetGlobalValue(#_const, _const);
#define SCRIPT_REG_CONST(_const) SCRIPT_REG_CONST_SS(m_pScriptSystem,_const)

// Description:
//		This class map an 3d vector to a LUA table with x,y,z members.
class CScriptObjectVector : public SmartScriptObject
{
public:
  CScriptObjectVector() {}
  CScriptObjectVector(IScriptSystem* pSS, bool bCreateEmpty = false) : SmartScriptObject(pSS, bCreateEmpty) {}
  void Set(const Vec3& v)
  {
    CScriptSetGetChain chain(*this);
    chain.SetValue("x", v.x);
    chain.SetValue("y", v.y);
    chain.SetValue("z", v.z);
  }
  Vec3 Get()
  {
    Vec3 v(0, 0, 0);
    CScriptSetGetChain chain(*this);
    chain.GetValue("x", v.x);
    chain.GetValue("y", v.y);
    chain.GetValue("z", v.z);
    return v;
  }
  CScriptObjectVector& operator=(const Vec3& v3) { Set(v3); return *this; }
};

//! This class map an "color" to a LUA table with indexed 3 numbers [1],[2],[3] members.
class CScriptObjectColor : public SmartScriptObject
{
public:
  CScriptObjectColor() {}
  CScriptObjectColor(IScriptSystem* pSS, bool bCreateEmpty = false) : SmartScriptObject(pSS, bCreateEmpty) {}
  void Set(const Vec3& v)
  {
    IScriptObject* pObject = *this;
    pObject->SetAt(1, v.x);
    pObject->SetAt(2, v.y);
    pObject->SetAt(3, v.z);
  }
  Vec3 Get()
  {
    IScriptObject* pObject = *this;
    Vec3 v(0, 0, 0);
    pObject->GetAt(1, v.x);
    pObject->GetAt(2, v.y);
    pObject->GetAt(3, v.z);
    return v;
  }
  CScriptObjectColor& operator=(const Vec3& v3) { Set(v3); return *this; }
};

/////////////////////////////////////////////////////////////////////////////
#endif //_SCRIPTABLE_H_
