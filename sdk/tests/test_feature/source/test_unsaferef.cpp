#include "utils.h"
#include "../../../add_on/scriptmath3d/scriptmath3d.h"

namespace TestUnsafeRef
{

static const char * const TESTNAME = "TestUnsafeRef";

static const char *script1 =
"void Test()                            \n"
"{                                      \n"
"   int[] arr = {0};                    \n"
"   TestRefInt(arr[0]);                 \n"
"   Assert(arr[0] == 23);               \n"
"   int a = 0;                          \n"
"   TestRefInt(a);                      \n"
"   Assert(a == 23);                    \n"
"   string[] sa = {\"\"};               \n"
"   TestRefString(sa[0]);               \n"
"   Assert(sa[0] == \"ref\");           \n"
"   string s = \"\";                    \n"
"   TestRefString(s);                   \n"
"   Assert(s == \"ref\");               \n"
"}                                      \n"
"void TestRefInt(int &ref)              \n"
"{                                      \n"
"   ref = 23;                           \n"
"}                                      \n"
"void TestRefString(string &ref)        \n"
"{                                      \n"
"   ref = \"ref\";                      \n"
"}                                      \n";

bool Test()
{
	bool fail = false;
	int r;

	COutStream out;

 	asIScriptEngine *engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	engine->SetEngineProperty(asEP_ALLOW_UNSAFE_REFERENCES, 1);
	engine->SetMessageCallback(asMETHOD(COutStream,Callback), &out, asCALL_THISCALL);
	RegisterScriptArray(engine, true);
	RegisterScriptString(engine);

	r = engine->RegisterGlobalFunction("void Assert(bool)", asFUNCTION(Assert), asCALL_GENERIC); assert( r >= 0 );

	asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
	mod->AddScriptSection(TESTNAME, script1);
	r = mod->Build();
	if( r < 0 )
	{
		TEST_FAILED;
		printf("%s: Failed to compile the script\n", TESTNAME);
	}
	asIScriptContext *ctx = engine->CreateContext();
	r = ExecuteString(engine, "Test()", mod, ctx);
	if( r != asEXECUTION_FINISHED )
	{
		TEST_FAILED;
		printf("%s: Execution failed: %d\n", TESTNAME, r);
	}

	if( ctx ) ctx->Release();

	engine->Release();

	// Test value class with unsafe ref
	{
		asIScriptEngine *engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetEngineProperty(asEP_ALLOW_UNSAFE_REFERENCES, 1);
		engine->SetMessageCallback(asMETHOD(COutStream,Callback), &out, asCALL_THISCALL);
		RegisterScriptMath3D(engine);

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection(TESTNAME, 
			"class Good \n"
			"{ \n"
			"  vector3 _val; \n"
			"  Good(const vector3& in val) \n"
			"  { \n"
			"    _val = val; \n"
			"  }  \n"
			"};  \n"
			"class Bad  \n"
			"{  \n"
			"  vector3 _val;  \n"
			"  Bad(const vector3& val)  \n"
			"  {  \n"
			"    _val = val;  \n"
			"  }  \n"
			"}; \n"
			"void test()  \n"
			"{ \n"
			"  // runs fine  \n"
			"  for (int i = 0; i < 2; i++)  \n"
			"    Good(vector3(1, 2, 3));  \n"
			"  // causes vm stack corruption  \n"
			"  for (int i = 0; i < 2; i++)  \n"
			"    Bad(vector3(1, 2, 3));  \n"
			"} \n");

		r = mod->Build();
		if( r < 0 )
			TEST_FAILED;

		r = ExecuteString(engine, "test()", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	// Success
	return fail;
}

} // namespace
