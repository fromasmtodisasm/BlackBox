#include <Game.hpp>
#include <ScriptObjects/ScriptObjectTest.hpp>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_DECLARE_SCRIPTABLEEX(CScriptObjectTest)

class TestClass
{
  public:
	TestClass() = default;
	void TestMethod(int i, char c)
	{
	
	}

};

CScriptObjectTest::CScriptObjectTest()
{
}

CScriptObjectTest::~CScriptObjectTest()
{
}

void CScriptObjectTest::InitializeTemplate(IScriptSystem* pSS)
{
  _ScriptableEx<CScriptObjectTest>::InitializeTemplate(pSS);
	#undef SCRIPT_REG_CLASSNAME 
	#define SCRIPT_REG_CLASSNAME CScriptObjectTest 
	SCRIPT_REG_TEMPLFUNC(TestMethod, "123");
	SCRIPT_REG_TEMPLFUNC(tm2, "123");
	SCRIPT_REG_TEMPLFUNC(tm3, "123");
}

void CScriptObjectTest::Init(IScriptSystem* pScriptSystem, CGame* pGame)
{
  InitGlobal(pScriptSystem, "TestObject", this);
}
