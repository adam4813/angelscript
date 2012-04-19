#ifndef SCRIPTARRAY_H
#define SCRIPTARRAY_H

#ifndef ANGELSCRIPT_H 
// Avoid having to inform include path if header is already include before
#include <angelscript.h>
#endif

BEGIN_AS_NAMESPACE

struct SArrayBuffer;

class CScriptArray
{
public:
	CScriptArray(asUINT length, asIObjectType *ot);
	CScriptArray(asUINT length, void *defVal, asIObjectType *ot);
	virtual ~CScriptArray();

	void AddRef() const;
	void Release() const;

	// Type information
	asIObjectType *GetArrayObjectType() const;
	int            GetArrayTypeId() const;
	int            GetElementTypeId() const;

	void   Reserve(asUINT maxElements);
	void   Resize(asUINT numElements);
	asUINT GetSize() const;
	bool   IsEmpty() const;

	// Get a pointer to an element. Returns 0 if out of bounds
	void       *At(asUINT index);
	const void *At(asUINT index) const;

	CScriptArray &operator=(const CScriptArray&);
	bool operator==(const CScriptArray &) const;

	void InsertAt(asUINT index, void *value);
	void RemoveAt(asUINT index);
	void InsertLast(void *value);
	void RemoveLast();
	void SortAsc();
	void SortDesc();
	void SortAsc(asUINT index, asUINT count);
	void SortDesc(asUINT index, asUINT count);
	void Sort(asUINT index, asUINT count, bool asc);
	void Reverse();
	int  Find(void *value) const;
	int  Find(asUINT index, void *value) const;

	// GC methods
	int  GetRefCount();
	void SetFlag();
	bool GetFlag();
	void EnumReferences(asIScriptEngine *engine);
	void ReleaseAllHandles(asIScriptEngine *engine);

protected:
	mutable int       refCount;
	mutable bool      gcFlag;
	asIObjectType    *objType;
	SArrayBuffer     *buffer;
	int               elementSize;
	int               cmpFuncId;
	int               eqFuncId;
	int               subTypeId;

	bool  Less(const void *a, const void *b, bool asc, asIScriptContext *ctx);
	void *GetArrayItemPointer(int index);
	void *GetDataPointer(void *buffer);
	void  Copy(void *dst, void *src);
	void  Precache();
	bool  CheckMaxSize(asUINT numElements);
	void  Resize(int delta, asUINT at);
	void  SetValue(asUINT index, void *value);
	void  CreateBuffer(SArrayBuffer **buf, asUINT numElements);
	void  DeleteBuffer(SArrayBuffer *buf);
	void  CopyBuffer(SArrayBuffer *dst, SArrayBuffer *src);
	void  Construct(SArrayBuffer *buf, asUINT start, asUINT end);
	void  Destruct(SArrayBuffer *buf, asUINT start, asUINT end);
	bool  Equals(const void *a, const void *b, asIScriptContext *ctx) const;
};

void RegisterScriptArray(asIScriptEngine *engine, bool defaultArray);

END_AS_NAMESPACE

#endif
