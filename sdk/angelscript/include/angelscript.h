/*
   AngelCode Scripting Library
   Copyright (c) 2003-2009 Andreas Jonsson

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any
   purpose, including commercial applications, and to alter it and
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
      distribution.

   The original version of this library can be located at:
   http://www.angelcode.com/angelscript/

   Andreas Jonsson
   andreas@angelcode.com
*/


//
// angelscript.h
//
// The script engine interface
//


#ifndef ANGELSCRIPT_H
#define ANGELSCRIPT_H

#include <stddef.h>

#ifdef AS_USE_NAMESPACE
 #define BEGIN_AS_NAMESPACE namespace AngelScript {
 #define END_AS_NAMESPACE }
#else
 #define BEGIN_AS_NAMESPACE
 #define END_AS_NAMESPACE
#endif

BEGIN_AS_NAMESPACE

// AngelScript version

#define ANGELSCRIPT_VERSION        21603
#define ANGELSCRIPT_VERSION_MAJOR  2
#define ANGELSCRIPT_VERSION_MINOR  16
#define ANGELSCRIPT_VERSION_BUILD  3
#define ANGELSCRIPT_VERSION_STRING "2.16.3"

// Data types

class asIScriptEngine;
class asIScriptModule;
class asIScriptContext;
class asIScriptGeneric;
class asIScriptObject;
class asIScriptArray;
class asIObjectType;
class asIScriptFunction;
class asIBinaryStream;

#ifdef AS_DEPRECATED
typedef asIScriptObject asIScriptStruct;
#endif

// Enumerations and constants

// Engine properties
enum asEEngineProp
{
	asEP_ALLOW_UNSAFE_REFERENCES      = 1,
	asEP_OPTIMIZE_BYTECODE            = 2,
	asEP_COPY_SCRIPT_SECTIONS         = 3,
	asEP_MAX_STACK_SIZE               = 4,
	asEP_USE_CHARACTER_LITERALS       = 5,
	asEP_ALLOW_MULTILINE_STRINGS      = 6,
	asEP_ALLOW_IMPLICIT_HANDLE_TYPES  = 7,
	asEP_BUILD_WITHOUT_LINE_CUES      = 8,
	asEP_INIT_GLOBAL_VARS_AFTER_BUILD = 9,
	asEP_REQUIRE_ENUM_SCOPE           = 10,
	asEP_SCRIPT_SCANNER               = 11
};

// Calling conventions
enum asECallConvTypes
{
	asCALL_CDECL            = 0,
	asCALL_STDCALL          = 1,
	asCALL_THISCALL         = 2,
	asCALL_CDECL_OBJLAST    = 3,
	asCALL_CDECL_OBJFIRST   = 4,
	asCALL_GENERIC          = 5
};

// Object type flags
enum asEObjTypeFlags
{
	asOBJ_REF                   = 0x01,
	asOBJ_VALUE                 = 0x02,
	asOBJ_GC                    = 0x04,
	asOBJ_POD                   = 0x08,
	asOBJ_NOHANDLE              = 0x10,
	asOBJ_SCOPED                = 0x20,
	asOBJ_TEMPLATE              = 0x40,
	asOBJ_APP_CLASS             = 0x100,
	asOBJ_APP_CLASS_CONSTRUCTOR = 0x200,
	asOBJ_APP_CLASS_DESTRUCTOR  = 0x400,
	asOBJ_APP_CLASS_ASSIGNMENT  = 0x800,
	asOBJ_APP_CLASS_C           = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR),
	asOBJ_APP_CLASS_CD          = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR + asOBJ_APP_CLASS_DESTRUCTOR),
	asOBJ_APP_CLASS_CA          = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR + asOBJ_APP_CLASS_ASSIGNMENT),
	asOBJ_APP_CLASS_CDA         = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_CONSTRUCTOR + asOBJ_APP_CLASS_DESTRUCTOR + asOBJ_APP_CLASS_ASSIGNMENT),
	asOBJ_APP_CLASS_D           = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_DESTRUCTOR),
	asOBJ_APP_CLASS_A           = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_ASSIGNMENT),
	asOBJ_APP_CLASS_DA          = (asOBJ_APP_CLASS + asOBJ_APP_CLASS_DESTRUCTOR + asOBJ_APP_CLASS_ASSIGNMENT),
	asOBJ_APP_PRIMITIVE         = 0x1000,
	asOBJ_APP_FLOAT             = 0x2000,
	asOBJ_MASK_VALID_FLAGS      = 0x3F7F,
	asOBJ_SCRIPT_OBJECT         = 0x10000
};

// Behaviours
enum asEBehaviours
{
	// Value object memory management
	asBEHAVE_CONSTRUCT,
	asBEHAVE_DESTRUCT,

	// Reference object memory management
	asBEHAVE_FACTORY,
	asBEHAVE_ADDREF,
	asBEHAVE_RELEASE,

	// Object operators
	asBEHAVE_VALUE_CAST,
	asBEHAVE_IMPLICIT_VALUE_CAST,
	asBEHAVE_INDEX,
	asBEHAVE_NEGATE,

	// Assignment operators
	asBEHAVE_FIRST_ASSIGN,
	 asBEHAVE_ASSIGNMENT = asBEHAVE_FIRST_ASSIGN,
	 asBEHAVE_ADD_ASSIGN,
	 asBEHAVE_SUB_ASSIGN,
	 asBEHAVE_MUL_ASSIGN,
	 asBEHAVE_DIV_ASSIGN,
	 asBEHAVE_MOD_ASSIGN,
	 asBEHAVE_OR_ASSIGN,
	 asBEHAVE_AND_ASSIGN,
	 asBEHAVE_XOR_ASSIGN,
	 asBEHAVE_SLL_ASSIGN,
	 asBEHAVE_SRL_ASSIGN,
	 asBEHAVE_SRA_ASSIGN,
	asBEHAVE_LAST_ASSIGN = asBEHAVE_SRA_ASSIGN,

	// Global operators
	asBEHAVE_FIRST_DUAL,
	 asBEHAVE_ADD = asBEHAVE_FIRST_DUAL,
	 asBEHAVE_SUBTRACT,
	 asBEHAVE_MULTIPLY,
	 asBEHAVE_DIVIDE,
	 asBEHAVE_MODULO,
	 asBEHAVE_EQUAL,
	 asBEHAVE_NOTEQUAL,
	 asBEHAVE_LESSTHAN,
	 asBEHAVE_GREATERTHAN,
	 asBEHAVE_LEQUAL,
	 asBEHAVE_GEQUAL,
	 asBEHAVE_BIT_OR,
	 asBEHAVE_BIT_AND,
	 asBEHAVE_BIT_XOR,
	 asBEHAVE_BIT_SLL,
	 asBEHAVE_BIT_SRL,
	 asBEHAVE_BIT_SRA,
	asBEHAVE_LAST_DUAL = asBEHAVE_BIT_SRA,
	asBEHAVE_REF_CAST,
	asBEHAVE_IMPLICIT_REF_CAST,

	// Garbage collection behaviours
	asBEHAVE_FIRST_GC,
	 asBEHAVE_GETREFCOUNT = asBEHAVE_FIRST_GC,
	 asBEHAVE_SETGCFLAG,
	 asBEHAVE_GETGCFLAG,
	 asBEHAVE_ENUMREFS,
	 asBEHAVE_RELEASEREFS,
	asBEHAVE_LAST_GC = asBEHAVE_RELEASEREFS
};

// Return codes
enum asERetCodes
{
	asSUCCESS                              =  0,
	asERROR                                = -1,
	asCONTEXT_ACTIVE                       = -2,
	asCONTEXT_NOT_FINISHED                 = -3,
	asCONTEXT_NOT_PREPARED                 = -4,
	asINVALID_ARG                          = -5,
	asNO_FUNCTION                          = -6,
	asNOT_SUPPORTED                        = -7,
	asINVALID_NAME                         = -8,
	asNAME_TAKEN                           = -9,
	asINVALID_DECLARATION                  = -10,
	asINVALID_OBJECT                       = -11,
	asINVALID_TYPE                         = -12,
	asALREADY_REGISTERED                   = -13,
	asMULTIPLE_FUNCTIONS                   = -14,
	asNO_MODULE                            = -15,
	asNO_GLOBAL_VAR                        = -16,
	asINVALID_CONFIGURATION                = -17,
	asINVALID_INTERFACE                    = -18,
	asCANT_BIND_ALL_FUNCTIONS              = -19,
	asLOWER_ARRAY_DIMENSION_NOT_REGISTERED = -20,
	asWRONG_CONFIG_GROUP                   = -21,
	asCONFIG_GROUP_IS_IN_USE               = -22,
	asILLEGAL_BEHAVIOUR_FOR_TYPE           = -23,
	asWRONG_CALLING_CONV                   = -24,
	asMODULE_IS_IN_USE                     = -25,
	asBUILD_IN_PROGRESS                    = -26,
	asINIT_GLOBAL_VARS_FAILED              = -27
};

// Context states
enum asEContextState
{
    asEXECUTION_FINISHED      = 0,
    asEXECUTION_SUSPENDED     = 1,
    asEXECUTION_ABORTED       = 2,
    asEXECUTION_EXCEPTION     = 3,
    asEXECUTION_PREPARED      = 4,
    asEXECUTION_UNINITIALIZED = 5,
    asEXECUTION_ACTIVE        = 6,
    asEXECUTION_ERROR         = 7
};

// ExecuteString flags
enum asEExecStrFlags
{
	asEXECSTRING_ONLY_PREPARE   = 1,
	asEXECSTRING_USE_MY_CONTEXT = 2
};

// Message types
enum asEMsgType
{
    asMSGTYPE_ERROR       = 0,
    asMSGTYPE_WARNING     = 1,
    asMSGTYPE_INFORMATION = 2
};

// Garbage collector flags
enum asEGCFlags
{
	asGC_FULL_CYCLE      = 1,
	asGC_ONE_STEP        = 2,
	asGC_DESTROY_GARBAGE = 4,
	asGC_DETECT_GARBAGE  = 8
};

// Token classes
enum asETokenClass
{
	asTC_UNKNOWN    = 0,
	asTC_KEYWORD    = 1,
	asTC_VALUE      = 2,
	asTC_IDENTIFIER = 3,
	asTC_COMMENT    = 4,
	asTC_WHITESPACE = 5
};

// Prepare flags
const int asPREPARE_PREVIOUS = -1;

// Config groups
const char * const asALL_MODULES = (const char * const)-1;

// Type id flags
enum asETypeIdFlags
{
	asTYPEID_VOID           = 0,
	asTYPEID_BOOL           = 1,
	asTYPEID_INT8           = 2,
	asTYPEID_INT16          = 3,
	asTYPEID_INT32          = 4,
	asTYPEID_INT64          = 5,
	asTYPEID_UINT8          = 6,
	asTYPEID_UINT16         = 7,
	asTYPEID_UINT32         = 8,
	asTYPEID_UINT64         = 9,
	asTYPEID_FLOAT          = 10,
	asTYPEID_DOUBLE         = 11,
	asTYPEID_OBJHANDLE      = 0x40000000,
	asTYPEID_HANDLETOCONST  = 0x20000000,
	asTYPEID_MASK_OBJECT    = 0x1C000000,
	asTYPEID_APPOBJECT      = 0x04000000,
#ifdef AS_DEPRECATED
	asTYPEID_SCRIPTSTRUCT   = 0x0C000000,
#endif
	asTYPEID_SCRIPTOBJECT   = 0x08000000,
	asTYPEID_SCRIPTARRAY    = 0x10000000,
	asTYPEID_MASK_SEQNBR    = 0x03FFFFFF
};

// Type modifiers
enum asETypeModifiers
{
	asTM_NONE     = 0,
	asTM_INREF    = 1,
	asTM_OUTREF   = 2,
	asTM_INOUTREF = 3
};

// GetModule flags
enum asEGMFlags
{
	asGM_ONLY_IF_EXISTS       = 0,
	asGM_CREATE_IF_NOT_EXISTS = 1,
	asGM_ALWAYS_CREATE        = 2
};

//
// asBYTE  =  8 bits
// asWORD  = 16 bits
// asDWORD = 32 bits
// asQWORD = 64 bits
// asPWORD = size of pointer
//
typedef unsigned char  asBYTE;
typedef unsigned short asWORD;
typedef unsigned int   asUINT;
typedef size_t         asPWORD;
#ifdef __LP64__
    typedef unsigned int  asDWORD;
    typedef unsigned long asQWORD;
    typedef long asINT64;
#else
    typedef unsigned long asDWORD;
  #if defined(__GNUC__) || defined(__MWERKS__)
    typedef unsigned long long asQWORD;
    typedef long long asINT64;
  #else
    typedef unsigned __int64 asQWORD;
    typedef __int64 asINT64;
  #endif
#endif

typedef void (*asFUNCTION_t)();
typedef void (*asGENFUNC_t)(asIScriptGeneric *);
typedef void *(*asALLOCFUNC_t)(size_t);
typedef void (*asFREEFUNC_t)(void *);

#define asFUNCTION(f) asFunctionPtr(f)
#if defined(_MSC_VER) && _MSC_VER <= 1200
// MSVC 6 has a bug that prevents it from properly compiling using the correct asFUNCTIONPR with operator >
// so we need to use ordinary C style cast instead of static_cast. The drawback is that the compiler can't 
// check that the cast is really valid.
#define asFUNCTIONPR(f,p,r) asFunctionPtr((void (*)())((r (*)p)(f)))
#else
#define asFUNCTIONPR(f,p,r) asFunctionPtr((void (*)())(static_cast<r (*)p>(f)))
#endif

#ifndef AS_NO_CLASS_METHODS

class asCUnknownClass;
typedef void (asCUnknownClass::*asMETHOD_t)();

struct asSFuncPtr
{
	union
	{
		// The largest known method point is 20 bytes (MSVC 64bit),
		// but with 8byte alignment this becomes 24 bytes. So we need
		// to be able to store at least that much.
		char dummy[25]; 
		struct {asMETHOD_t   mthd; char dummy[25-sizeof(asMETHOD_t)];} m;
		struct {asFUNCTION_t func; char dummy[25-sizeof(asFUNCTION_t)];} f;
	} ptr;
	asBYTE flag; // 1 = generic, 2 = global func, 3 = method
};

#define asMETHOD(c,m) asSMethodPtr<sizeof(void (c::*)())>::Convert((void (c::*)())(&c::m))
#define asMETHODPR(c,m,p,r) asSMethodPtr<sizeof(void (c::*)())>::Convert(static_cast<r (c::*)p>(&c::m))

#else // Class methods are disabled

struct asSFuncPtr
{
	union
	{
		char dummy[25]; // largest known class method pointer
		struct {asFUNCTION_t func; char dummy[25-sizeof(asFUNCTION_t)];} f;
	} ptr;
	asBYTE flag; // 1 = generic, 2 = global func
};

#endif

struct asSMessageInfo
{
	const char *section;
	int         row;
	int         col;
	asEMsgType  type;
	const char *message;
};


// API functions

// ANGELSCRIPT_EXPORT is defined when compiling the dll or lib
// ANGELSCRIPT_DLL_LIBRARY_IMPORT is defined when dynamically linking to the
// dll through the link lib automatically generated by MSVC++
// ANGELSCRIPT_DLL_MANUAL_IMPORT is defined when manually loading the dll
// Don't define anything when linking statically to the lib

#ifdef WIN32
  #ifdef ANGELSCRIPT_EXPORT
    #define AS_API __declspec(dllexport)
  #elif defined ANGELSCRIPT_DLL_LIBRARY_IMPORT
    #define AS_API __declspec(dllimport)
  #else // statically linked library
    #define AS_API
  #endif
#else
  #define AS_API
#endif

#ifndef ANGELSCRIPT_DLL_MANUAL_IMPORT
extern "C"
{
	// Engine
	AS_API asIScriptEngine * asCreateScriptEngine(asDWORD version);
	AS_API const char * asGetLibraryVersion();
	AS_API const char * asGetLibraryOptions();

	// Context
	AS_API asIScriptContext * asGetActiveContext();

	// Thread support
	AS_API int asThreadCleanup();

	// Memory management
	AS_API int asSetGlobalMemoryFunctions(asALLOCFUNC_t allocFunc, asFREEFUNC_t freeFunc);
	AS_API int asResetGlobalMemoryFunctions();
}
#endif // ANGELSCRIPT_DLL_MANUAL_IMPORT

// Interface declarations

class asIScriptEngine
{
public:
	// Memory management
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	// Engine properties
	virtual int     SetEngineProperty(asEEngineProp property, asPWORD value) = 0;
	virtual asPWORD GetEngineProperty(asEEngineProp property) = 0;

	// Compiler messages
	virtual int SetMessageCallback(const asSFuncPtr &callback, void *obj, asDWORD callConv) = 0;
	virtual int ClearMessageCallback() = 0;
	virtual int WriteMessage(const char *section, int row, int col, asEMsgType type, const char *message) = 0;

	// Global functions
	virtual int RegisterGlobalFunction(const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv) = 0;
	virtual int GetGlobalFunctionCount() = 0;
	virtual int GetGlobalFunctionIdByIndex(asUINT index) = 0;

	// Global properties
	virtual int RegisterGlobalProperty(const char *declaration, void *pointer) = 0;
	virtual int GetGlobalPropertyCount() = 0;
	virtual int GetGlobalPropertyByIndex(asUINT index, const char **name, int *typeId = 0, bool *isConst = 0, const char **configGroup = 0, void **pointer = 0) = 0;

	// Object types
	virtual int            RegisterObjectType(const char *obj, int byteSize, asDWORD flags) = 0;
	virtual int            RegisterObjectProperty(const char *obj, const char *declaration, int byteOffset) = 0;
	virtual int            RegisterObjectMethod(const char *obj, const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv) = 0;
	virtual int            RegisterObjectBehaviour(const char *obj, asEBehaviours behaviour, const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv) = 0;
	virtual int            RegisterGlobalBehaviour(asEBehaviours behaviour, const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv) = 0;
	virtual int            RegisterInterface(const char *name) = 0;
	virtual int            RegisterInterfaceMethod(const char *intf, const char *declaration) = 0;
	virtual int            GetObjectTypeCount() = 0;
	virtual asIObjectType *GetObjectTypeByIndex(asUINT index) = 0;
	virtual int            GetGlobalBehaviourCount() = 0;
	virtual int            GetGlobalBehaviourByIndex(asUINT index, asEBehaviours *outBehaviour) = 0;

	// String factory
	virtual int RegisterStringFactory(const char *datatype, const asSFuncPtr &factoryFunc, asDWORD callConv) = 0;
	virtual int GetStringFactoryReturnTypeId() = 0;

	// Enums
	virtual int         RegisterEnum(const char *type) = 0;
	virtual int         RegisterEnumValue(const char *type, const char *name, int value) = 0;
	virtual int         GetEnumCount() = 0;
	virtual const char *GetEnumByIndex(asUINT index, int *enumTypeId, const char **configGroup = 0) = 0;
	virtual int         GetEnumValueCount(int enumTypeId) = 0;
	virtual const char *GetEnumValueByIndex(int enumTypeId, asUINT index, int *outValue) = 0;

	// Typedefs
	virtual int         RegisterTypedef(const char *type, const char *decl) = 0;
	virtual int         GetTypedefCount() = 0;
	virtual const char *GetTypedefByIndex(asUINT index, int *typeId, const char **configGroup = 0) = 0;

	// Configuration groups
	virtual int BeginConfigGroup(const char *groupName) = 0;
	virtual int EndConfigGroup() = 0;
	virtual int RemoveConfigGroup(const char *groupName) = 0;
	virtual int SetConfigGroupModuleAccess(const char *groupName, const char *module, bool hasAccess) = 0;

	// Script modules
	virtual asIScriptModule *GetModule(const char *module, asEGMFlags flag = asGM_ONLY_IF_EXISTS) = 0;
	virtual int              DiscardModule(const char *module) = 0;

	// Script functions
	virtual asIScriptFunction *GetFunctionDescriptorById(int funcId) = 0;

	// Type identification
	virtual asIObjectType *GetObjectTypeById(int typeId) = 0;
	virtual int            GetTypeIdByDecl(const char *decl) = 0;
	virtual const char    *GetTypeDeclaration(int typeId) = 0;
	virtual int            GetSizeOfPrimitiveType(int typeId) = 0;

	// Script execution
	virtual asIScriptContext *CreateContext() = 0;
	virtual void             *CreateScriptObject(int typeId) = 0;
	virtual void             *CreateScriptObjectCopy(void *obj, int typeId) = 0;
	virtual void              CopyScriptObject(void *dstObj, void *srcObj, int typeId) = 0;
	virtual void              ReleaseScriptObject(void *obj, int typeId) = 0;
	virtual void              AddRefScriptObject(void *obj, int typeId) = 0;
	virtual bool              IsHandleCompatibleWithObject(void *obj, int objTypeId, int handleTypeId) = 0;
	virtual int               CompareScriptObjects(bool &result, int behaviour, void *leftObj, void *rightObj, int typeId) = 0;

	// String interpretation
	virtual asETokenClass ParseToken(const char *string, size_t stringLength = 0, int *tokenLength = 0) = 0;
	virtual int           ExecuteString(const char *module, const char *script, asIScriptContext **ctx = 0, asDWORD flags = 0) = 0;

	// Garbage collection
	virtual int  GarbageCollect(asDWORD flags = asGC_FULL_CYCLE) = 0;
	virtual void GetGCStatistics(asUINT *currentSize, asUINT *totalDestroyed = 0, asUINT *totalDetected = 0) = 0;
	virtual void NotifyGarbageCollectorOfNewObject(void *obj, int typeId) = 0;
	virtual void GCEnumCallback(void *reference) = 0;

	// User data
	virtual void *SetUserData(void *data) = 0;
	virtual void *GetUserData() = 0;

#ifdef AS_DEPRECATED
	virtual int                AddScriptSection(const char *module, const char *name, const char *code, size_t codeLength = 0, int lineOffset = 0) = 0;
	virtual int                Build(const char *module) = 0;
    virtual int                Discard(const char *module) = 0;
	virtual int                ResetModule(const char *module) = 0;
	virtual int                GetFunctionCount(const char *module) = 0;
	virtual int                GetFunctionIDByIndex(const char *module, int index) = 0;
	virtual int                GetFunctionIDByName(const char *module, const char *name) = 0;
	virtual int                GetFunctionIDByDecl(const char *module, const char *decl) = 0;
	virtual asIScriptFunction *GetFunctionDescriptorByIndex(const char *module, int index) = 0;
	virtual int                GetGlobalVarCount(const char *module) = 0;
	virtual int                GetGlobalVarIndexByName(const char *module, const char *name) = 0;
	virtual int                GetGlobalVarIndexByDecl(const char *module, const char *decl) = 0;
	virtual const char        *GetGlobalVarDeclaration(const char *module, int index, int *length = 0) = 0;
	virtual const char        *GetGlobalVarName(const char *module, int index, int *length = 0) = 0;
	virtual void              *GetAddressOfGlobalVar(const char *module, int index) = 0;
	virtual int                GetTypeIdByDecl(const char *module, const char *decl) = 0;
	virtual int                GetImportedFunctionCount(const char *module) = 0;
	virtual int                GetImportedFunctionIndexByDecl(const char *module, const char *decl) = 0;
	virtual const char        *GetImportedFunctionDeclaration(const char *module, int importIndex, int *length = 0) = 0;
	virtual const char        *GetImportedFunctionSourceModule(const char *module, int importIndex, int *length = 0) = 0;
	virtual int                BindImportedFunction(const char *module, int importIndex, int funcId) = 0;
	virtual int                UnbindImportedFunction(const char *module, int importIndex) = 0;
	virtual int                BindAllImportedFunctions(const char *module) = 0;
	virtual int                UnbindAllImportedFunctions(const char *module) = 0;
	virtual int                GetObjectsInGarbageCollectorCount() = 0;
	virtual int                SaveByteCode(const char *module, asIBinaryStream *out) = 0;
	virtual int                LoadByteCode(const char *module, asIBinaryStream *in) = 0;
#endif

protected:
	virtual ~asIScriptEngine() {}
};

class asIScriptModule
{
public:
	virtual asIScriptEngine *GetEngine() = 0;
	virtual void             SetName(const char *name) = 0;
	virtual const char      *GetName() = 0;

	// Compilation
    virtual int  AddScriptSection(const char *name, const char *code, size_t codeLength = 0, int lineOffset = 0) = 0;
	virtual int  Build() = 0;

	// Functions
	virtual int                GetFunctionCount() = 0;
	virtual int                GetFunctionIdByIndex(int index) = 0;
	virtual int                GetFunctionIdByName(const char *name) = 0;
	virtual int                GetFunctionIdByDecl(const char *decl) = 0;
	virtual asIScriptFunction *GetFunctionDescriptorByIndex(int index) = 0;
	virtual asIScriptFunction *GetFunctionDescriptorById(int funcId) = 0;

	// Global variables
	virtual int         ResetGlobalVars() = 0;
	virtual int         GetGlobalVarCount() = 0;
	virtual int         GetGlobalVarIndexByName(const char *name) = 0;
	virtual int         GetGlobalVarIndexByDecl(const char *decl) = 0;
	virtual const char *GetGlobalVarDeclaration(int index) = 0;
	virtual const char *GetGlobalVarName(int index) = 0;
	virtual int         GetGlobalVarTypeId(int index, bool *isConst = 0) = 0;
	virtual void       *GetAddressOfGlobalVar(int index) = 0;

	// Type identification
	virtual int            GetObjectTypeCount() = 0;
	virtual asIObjectType *GetObjectTypeByIndex(asUINT index) = 0;
	virtual int            GetTypeIdByDecl(const char *decl) = 0;

	// Enums
	virtual int         GetEnumCount() = 0;
	virtual const char *GetEnumByIndex(asUINT index, int *enumTypeId) = 0;
	virtual int         GetEnumValueCount(int enumTypeId) = 0;
	virtual const char *GetEnumValueByIndex(int enumTypeId, asUINT index, int *outValue) = 0;

	// Typedefs
	virtual int         GetTypedefCount() = 0;
	virtual const char *GetTypedefByIndex(asUINT index, int *typeId) = 0;

	// Dynamic binding between modules
	virtual int         GetImportedFunctionCount() = 0;
	virtual int         GetImportedFunctionIndexByDecl(const char *decl) = 0;
	virtual const char *GetImportedFunctionDeclaration(int importIndex) = 0;
	virtual const char *GetImportedFunctionSourceModule(int importIndex) = 0;
	virtual int         BindImportedFunction(int importIndex, int funcId) = 0;
	virtual int         UnbindImportedFunction(int importIndex) = 0;
	virtual int         BindAllImportedFunctions() = 0;
	virtual int         UnbindAllImportedFunctions() = 0;

	// Bytecode saving and loading
	virtual int SaveByteCode(asIBinaryStream *out) = 0;
	virtual int LoadByteCode(asIBinaryStream *in) = 0;

protected:
	virtual ~asIScriptModule() {}
};

class asIScriptContext
{
public:
	// Memory management
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	// Miscellaneous
	virtual asIScriptEngine *GetEngine() = 0;

	// Execution
	virtual int             Prepare(int funcId) = 0;
	virtual int             Unprepare() = 0;
	virtual int             SetObject(void *obj) = 0;
	virtual int             Execute() = 0;
	virtual int             Abort() = 0;
	virtual int             Suspend() = 0;
	virtual asEContextState GetState() = 0;

	// Arguments
	virtual int   SetArgByte(asUINT arg, asBYTE value) = 0;
	virtual int   SetArgWord(asUINT arg, asWORD value) = 0;
	virtual int   SetArgDWord(asUINT arg, asDWORD value) = 0;
	virtual int   SetArgQWord(asUINT arg, asQWORD value) = 0;
	virtual int   SetArgFloat(asUINT arg, float value) = 0;
	virtual int   SetArgDouble(asUINT arg, double value) = 0;
	virtual int   SetArgAddress(asUINT arg, void *addr) = 0;
	virtual int   SetArgObject(asUINT arg, void *obj) = 0;
	virtual void *GetArgPointer(asUINT arg) = 0;

	// Return value
	virtual asBYTE  GetReturnByte() = 0;
	virtual asWORD  GetReturnWord() = 0;
	virtual asDWORD GetReturnDWord() = 0;
	virtual asQWORD GetReturnQWord() = 0;
	virtual float   GetReturnFloat() = 0;
	virtual double  GetReturnDouble() = 0;
	virtual void   *GetReturnAddress() = 0;
	virtual void   *GetReturnObject() = 0;
	virtual void   *GetAddressOfReturnValue() = 0;

	// Exception handling
	virtual int         SetException(const char *string) = 0;
	virtual int         GetExceptionLineNumber(int *column = 0) = 0;
	virtual int         GetExceptionFunction() = 0;
	virtual const char *GetExceptionString() = 0;
	virtual int         SetExceptionCallback(asSFuncPtr callback, void *obj, int callConv) = 0;
	virtual void        ClearExceptionCallback() = 0;

	// Debugging
	virtual int         SetLineCallback(asSFuncPtr callback, void *obj, int callConv) = 0;
	virtual void        ClearLineCallback() = 0;
	virtual int         GetCurrentLineNumber(int *column = 0) = 0;
	virtual int         GetCurrentFunction() = 0;
	virtual int         GetCallstackSize() = 0;
	virtual int         GetCallstackFunction(int index) = 0;
	virtual int         GetCallstackLineNumber(int index, int *column = 0) = 0;
	virtual int         GetVarCount(int stackLevel = -1) = 0;
	virtual const char *GetVarName(int varIndex, int stackLevel = -1) = 0;
	virtual const char *GetVarDeclaration(int varIndex, int stackLevel = -1) = 0;
	virtual int         GetVarTypeId(int varIndex, int stackLevel = -1) = 0;
	virtual void       *GetAddressOfVar(int varIndex, int stackLevel = -1) = 0;
	virtual int         GetThisTypeId(int stackLevel = -1) = 0;
	virtual void       *GetThisPointer(int stackLevel = -1) = 0;

	// User data
	virtual void *SetUserData(void *data) = 0;
	virtual void *GetUserData() = 0;

#ifdef AS_DEPRECATED
	virtual void *GetReturnPointer() = 0;
	virtual void *GetVarPointer(int varIndex, int stackLevel = -1) = 0;
#endif

protected:
	virtual ~asIScriptContext() {}
};

class asIScriptGeneric
{
public:
	// Miscellaneous
	virtual asIScriptEngine *GetEngine() = 0;
	virtual int              GetFunctionId() = 0;

	// Object
	virtual void   *GetObject() = 0;
	virtual int     GetObjectTypeId() = 0;

	// Arguments
	virtual int     GetArgCount() = 0;
	virtual int     GetArgTypeId(asUINT arg) = 0;
	virtual asBYTE  GetArgByte(asUINT arg) = 0;
	virtual asWORD  GetArgWord(asUINT arg) = 0;
	virtual asDWORD GetArgDWord(asUINT arg) = 0;
	virtual asQWORD GetArgQWord(asUINT arg) = 0;
	virtual float   GetArgFloat(asUINT arg) = 0;
	virtual double  GetArgDouble(asUINT arg) = 0;
	virtual void   *GetArgAddress(asUINT arg) = 0;
	virtual void   *GetArgObject(asUINT arg) = 0;
	virtual void   *GetAddressOfArg(asUINT arg) = 0;

	// Return value
	virtual int     GetReturnTypeId() = 0;
	virtual int     SetReturnByte(asBYTE val) = 0;
	virtual int     SetReturnWord(asWORD val) = 0;
	virtual int     SetReturnDWord(asDWORD val) = 0;
	virtual int     SetReturnQWord(asQWORD val) = 0;
	virtual int     SetReturnFloat(float val) = 0;
	virtual int     SetReturnDouble(double val) = 0;
	virtual int     SetReturnAddress(void *addr) = 0;
	virtual int     SetReturnObject(void *obj) = 0;
	virtual void   *GetAddressOfReturnLocation() = 0;

#ifdef AS_DEPRECATED
	virtual void   *GetReturnPointer() = 0;
	virtual void   *GetArgPointer(asUINT arg) = 0;
#endif

protected:
	virtual ~asIScriptGeneric() {}
};

class asIScriptObject
{
public:
	// Memory management
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	// Type info
	virtual int            GetTypeId() const = 0;
	virtual asIObjectType *GetObjectType() const = 0;

	// Class properties
	virtual int         GetPropertyCount() const = 0;
	virtual int         GetPropertyTypeId(asUINT prop) const = 0;
	virtual const char *GetPropertyName(asUINT prop) const = 0;
	virtual void       *GetPropertyPointer(asUINT prop) = 0;

	virtual asIScriptEngine *GetEngine() const = 0;
	virtual int              CopyFrom(asIScriptObject *other) = 0;

#ifdef AS_DEPRECATED
	virtual int            GetStructTypeId() const = 0;
#endif

protected:
	virtual ~asIScriptObject() {}
};

class asIScriptArray
{
public:
	virtual asIScriptEngine *GetEngine() const = 0;

	// Memory management
	virtual int AddRef() = 0;
	virtual int Release() = 0;

	// Array type
	virtual int GetArrayTypeId() = 0;

	// Elements
	virtual int    GetElementTypeId() = 0;
	virtual asUINT GetElementCount() = 0;
	virtual void * GetElementPointer(asUINT index) = 0;
	virtual void   Resize(asUINT size) = 0;
	virtual int    CopyFrom(asIScriptArray *other) = 0;

protected:
	virtual ~asIScriptArray() {}
};

class asIObjectType
{
public:
	virtual asIScriptEngine *GetEngine() const = 0;
	virtual const char      *GetConfigGroup() const = 0;

	// Type info
	virtual const char      *GetName() const = 0;
	virtual asIObjectType   *GetBaseType() const = 0;
	virtual asDWORD          GetFlags() const = 0;
	virtual asUINT           GetSize() const = 0;

	// Interfaces
	virtual int              GetInterfaceCount() const = 0;
	virtual asIObjectType   *GetInterface(asUINT index) const = 0;

	// Factories
	virtual int                GetFactoryCount() const = 0;
	virtual int                GetFactoryIdByIndex(int index) const = 0;
	virtual int                GetFactoryIdByDecl(const char *decl) const = 0;

	// Methods
	virtual int                GetMethodCount() const = 0;
	virtual int                GetMethodIdByIndex(int index) const = 0;
	virtual int                GetMethodIdByName(const char *name) const = 0;
	virtual int                GetMethodIdByDecl(const char *decl) const = 0;
	virtual asIScriptFunction *GetMethodDescriptorByIndex(int index) const = 0;

	// Properties
	virtual int         GetPropertyCount() const = 0;
	virtual int         GetPropertyTypeId(asUINT prop) const = 0;
	virtual const char *GetPropertyName(asUINT prop) const = 0;
	virtual int         GetPropertyOffset(asUINT prop) const = 0;

	// Behaviours
	virtual int GetBehaviourCount() const = 0;
	virtual int GetBehaviourByIndex(asUINT index, asEBehaviours *outBehaviour) const = 0;

#ifdef AS_DEPRECATED
	virtual asIObjectType   *GetSubType() const = 0;
	virtual bool             IsInterface() const = 0;
#endif

protected:
	virtual ~asIObjectType() {}
};

class asIScriptFunction
{
public:
	virtual asIScriptEngine *GetEngine() const = 0;
	virtual const char      *GetModuleName() const = 0;
	virtual const char      *GetScriptSectionName() const = 0;
	virtual const char      *GetConfigGroup() const = 0;
	virtual asIObjectType   *GetObjectType() const = 0;
	virtual const char      *GetObjectName() const = 0;
	virtual const char      *GetName() const = 0;
	virtual const char      *GetDeclaration(bool includeObjectName = true) const = 0;
	virtual bool             IsClassMethod() const = 0;
	virtual bool             IsInterfaceMethod() const = 0;

	virtual int              GetParamCount() const = 0;
	virtual int              GetParamTypeId(int index, asDWORD *flags = 0) const = 0;
	virtual int              GetReturnTypeId() const = 0;

protected:
	virtual ~asIScriptFunction() {};
};

class asIBinaryStream
{
public:
	virtual void Read(void *ptr, asUINT size) = 0;
	virtual void Write(const void *ptr, asUINT size) = 0;

public:
	virtual ~asIBinaryStream() {}
};

//-----------------------------------------------------------------
// Function pointers

// Use our own memset() and memcpy() implementations for better portability
inline void asMemClear(void *_p, int size)
{
	char *p = (char *)_p;
	const char *e = p + size;
	for( ; p < e; p++ )
		*p = 0;
}

inline void asMemCopy(void *_d, const void *_s, int size)
{
	char *d = (char *)_d;
	const char *s = (const char *)_s;
	const char *e = s + size;
	for( ; s < e; d++, s++ )
		*d = *s;
}

// Template function to capture all global functions,
// except the ones using the generic calling convention
template <class T>
inline asSFuncPtr asFunctionPtr(T func)
{
	asSFuncPtr p;
	asMemClear(&p, sizeof(p));
	p.ptr.f.func = (asFUNCTION_t)(size_t)func;

	// Mark this as a global function
	p.flag = 2;

	return p;
}

// Specialization for functions using the generic calling convention
template<>
inline asSFuncPtr asFunctionPtr<asGENFUNC_t>(asGENFUNC_t func)
{
	asSFuncPtr p;
	asMemClear(&p, sizeof(p));
	p.ptr.f.func = (asFUNCTION_t)func;

	// Mark this as a generic function
	p.flag = 1;

	return p;
}

#ifndef AS_NO_CLASS_METHODS

// Method pointers

// Declare a dummy class so that we can determine the size of a simple method pointer
class asCSimpleDummy {};
typedef void (asCSimpleDummy::*asSIMPLEMETHOD_t)();
const int SINGLE_PTR_SIZE = sizeof(asSIMPLEMETHOD_t);

// Define template
template <int N>
struct asSMethodPtr
{
	template<class M>
	static asSFuncPtr Convert(M Mthd)
	{
		// This version of the function should never be executed, nor compiled,
		// as it would mean that the size of the method pointer cannot be determined.

		int ERROR_UnsupportedMethodPtr[N-100];
		return 0;
	}
};

// Template specialization
template <>
struct asSMethodPtr<SINGLE_PTR_SIZE>
{
	template<class M>
	static asSFuncPtr Convert(M Mthd)
	{
		asSFuncPtr p;
		asMemClear(&p, sizeof(p));

		asMemCopy(&p, &Mthd, SINGLE_PTR_SIZE);

		// Mark this as a class method
		p.flag = 3;

		return p;
	}
};

#if defined(_MSC_VER) && !defined(__MWERKS__)

// MSVC and Intel uses different sizes for different class method pointers
template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+1*sizeof(int)>
{
	template <class M>
	static asSFuncPtr Convert(M Mthd)
	{
		asSFuncPtr p;
		asMemClear(&p, sizeof(p));

		asMemCopy(&p, &Mthd, SINGLE_PTR_SIZE+sizeof(int));

		// Mark this as a class method
		p.flag = 3;

		return p;
	}
};

template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+2*sizeof(int)>
{
	template <class M>
	static asSFuncPtr Convert(M Mthd)
	{
		// This is where a class with virtual inheritance falls, or
		// on 64bit platforms with 8byte data alignments.

		// Method pointers for virtual inheritance is not supported,
		// as it requires the location of the vbase table, which is 
		// only available to the C++ compiler, but not in the method
		// pointer. 

		// You can get around this by forward declaring the class and
		// storing the sizeof its method pointer in a constant. Example:

		// class ClassWithVirtualInheritance;
		// const int ClassWithVirtualInheritance_workaround = sizeof(void ClassWithVirtualInheritance::*());

		// This will force the compiler to use the unknown type
		// for the class, which falls under the next case

		// TODO: We need to try to identify if this is really a method pointer
		//       with virtual inheritance, or just a method pointer for multiple 
		//       inheritance with pad bytes to produce a 16byte structure.

		asSFuncPtr p;
		asMemClear(&p, sizeof(p));

		asMemCopy(&p, &Mthd, SINGLE_PTR_SIZE+2*sizeof(int));

		// Mark this as a class method
		p.flag = 3;

		return p;
	}
};

template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+3*sizeof(int)>
{
	template <class M>
	static asSFuncPtr Convert(M Mthd)
	{
		asSFuncPtr p;
		asMemClear(&p, sizeof(p));

		asMemCopy(&p, &Mthd, SINGLE_PTR_SIZE+3*sizeof(int));

		// Mark this as a class method
		p.flag = 3;

		return p;
	}
};

template <>
struct asSMethodPtr<SINGLE_PTR_SIZE+4*sizeof(int)>
{
	template <class M>
	static asSFuncPtr Convert(M Mthd)
	{
		// On 64bit platforms with 8byte data alignment
		// the unknown class method pointers will come here.

		asSFuncPtr p;
		asMemClear(&p, sizeof(p));

		asMemCopy(&p, &Mthd, SINGLE_PTR_SIZE+4*sizeof(int));

		// Mark this as a class method
		p.flag = 3;

		return p;
	}
};

#endif

#endif // AS_NO_CLASS_METHODS

END_AS_NAMESPACE

#endif
