// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

// Note: The reflection registry is static for the whole library lifetime.

#pragma once

#include "foundation/assert.h"
#include "foundation/cext.h"
#include "foundation/document.h"
#include "foundation/get_type_name.h"
#include "foundation/global_object.h"
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace hg {

typedef std::hash<std::string>::result_type TypeId;

template <typename T>
TypeId GetTypeId() { // coherent between DLLs, hash must not collide
	static auto type_id = std::hash<std::string>{}(GetTypeName<T>());
	return type_id;
}

class Type;
class TypeRegistry;

extern global_object<TypeRegistry> g_type_registry;

template <typename T>
void RegisterTypeReflection(TypeRegistry &registry) { __ASSERT__(false); }

//
template <typename T>
struct TSerializationStackEntry {
	const Type *type;
	T i;

	TSerializationStackEntry(const Type *t, T o) : type(t), i(o) {}
};

typedef TSerializationStackEntry<const void *> SerializationStackEntry;
typedef TSerializationStackEntry<void *> DeserializationStackEntry;

template <typename T>
struct TSerializationStack : std::vector<T> {
	T *FindType(const Type *type) {
		auto i = std::find_if(this->begin(), this->end(), [type](const T &t) { return t.type == type; });
		return i != this->end() ? &(*i) : nullptr;
	}
};

struct DeserializationStack : TSerializationStack<DeserializationStackEntry> {
	template <class C>
	C *FindInstance() const;
};

struct SerializationStack : TSerializationStack<SerializationStackEntry> {
	template <class C>
	const C *FindInstance() const;
};

template <class S>
class TSerializationStackGuard {
	S *stack;

public:
	explicit TSerializationStackGuard(S *s) : stack(s) {}

	~TSerializationStackGuard() {
		if (stack)
			stack->pop_back();
	}
};

typedef TSerializationStackGuard<SerializationStack> SerializationStackGuard;
typedef TSerializationStackGuard<DeserializationStack> DeserializationStackGuard;

//
struct DocumentReader;
struct DocumentWriter;

struct SerializationContext {
	virtual ~SerializationContext() {}
	virtual const std::string &GetType() const {
		static std::string type = "SerializationContext";
		return type;
	}
	mutable SerializationStack stack;
	std::string dependencies_dir; // directory to write dependencies to
};

struct DeserializationContext {
	virtual ~DeserializationContext() {}
	virtual const std::string &GetType() const {
		static std::string type = "DeserializationContext";
		return type;
	}
	mutable DeserializationStack stack;
	std::string dependencies_dir; // directory to read dependencies from
};

struct TypeSerializer {
	TypeSerializer() {}
	virtual ~TypeSerializer() {}

	virtual bool Deserialize(DocumentReader &doc, void *o, const Type *type, DeserializationContext *ctx = nullptr, const std::string &name = std::string()) = 0;
	virtual bool Serialize(DocumentWriter &doc, const void *o, const Type *type, SerializationContext *ctx = nullptr, const std::string &name = std::string()) = 0;

	/// Return the type under which the input type will be serialized.
	virtual const Type *GetSerializeType() const { return nullptr; }
};

//
#define DeclarePropertyAccessor(Name, Class, Type, Getter, Setter)       \
	struct Name {                                                        \
		static inline Type Get(const Class &i) { return i.Getter(); }    \
		static inline void Set(Class &i, const Type &v) { i.Setter(v); } \
	};

//
struct TypeFactory {
	virtual ~TypeFactory() {}
	virtual void *New() const = 0;
	virtual void Delete(void *) const = 0;
	virtual void Copy(void *, const void *) const = 0;
};

template <typename T>
struct ConcreteTypeFactory : TypeFactory {
	explicit ConcreteTypeFactory(const Type *t) : type(t) {}

	void *New() const override { return new T; }
	void Delete(void *o) const override { delete reinterpret_cast<T *>(o); }
	void Copy(void *d, const void *s) const override { *reinterpret_cast<T *>(d) = *reinterpret_cast<const T *>(s); }

	const Type *type;
};

class TypeField;

/// Registry of reflected C++ types
class TypeRegistry {
public:
	typedef std::map<std::string, std::unique_ptr<Type>> TypeMap;
	typedef std::map<std::string, Type *> AliasMap;

	~TypeRegistry();

	Type *HasType(const std::string &name);
	Type *GetType(const std::string &name);
	inline const TypeMap &GetTypes() const { return types; }
	inline const AliasMap &GetAliases() const { return aliases; }

	template <typename T>
	Type *HasType();
	template <typename T>
	Type *GetType();

	template <typename T>
	Type &CreateAbstractType();
	template <typename T>
	Type &CreateType();

	Type &CreateAlias(const std::string &alias, Type *);

	/*
		There are so few fields that need skipping serialization that it makes no
		sense to store a list of those inside the concerned type.
	*/
	std::map<Type *, std::vector<TypeField *>> unserialized_type_fields;
	std::map<Type *, std::vector<TypeField *>> muted_type_fields;

private:
	TypeMap types;
	AliasMap aliases;
};

//
class TypeValue {
public:
	TypeValue() = default;
	TypeValue(TypeValue &&v);
	TypeValue(const TypeValue &v);
	TypeValue(const Type *t);
	TypeValue(const Type *t, const void *o);
	TypeValue(const char* type_name);
	TypeValue(const char* type_name, const void *o);
	template <typename T>
	TypeValue(const T &v);

	~TypeValue();

	//
	inline const Type *GetType() const { return type; }
	inline void *GetData() const { return data; }

	//
	void Clear();
	void Set(const Type *t);
	void Set(const Type *t, const void *in);

	//
	template <typename T>
	void Set(const T *);

	template <typename T>
	T &Cast(const Type *to) const;
	template <typename T>
	T &CastDefault(const Type *to, T &on_fail) const;
	template <typename T>
	const T &CastDefault(const Type *to, const T &on_fail) const;
	template <typename T>
	T &Cast() const;
	template <typename T>
	T &CastDefault(T &on_fail) const;
	template <typename T>
	const T &CastDefault(const T &on_fail) const;

	template <typename T>
	void Get(T &out) const { out = Cast<T>(); }

	//
	TypeValue &operator=(const TypeValue &v);

private:
	const Type *type{nullptr};
	void *data{nullptr};
};

//
struct TypeValueComparator {
	virtual ~TypeValueComparator() {}
	virtual bool Compare(const TypeValue &a, const TypeValue &b) const = 0;
};

template <typename T>
struct ConcreteTypeValueComparator : public TypeValueComparator {
	virtual bool Compare(const TypeValue &a, const TypeValue &b) const;
};

//
struct TypeFieldSerializationModifier {
	virtual ~TypeFieldSerializationModifier() {}

	virtual TypeValue Modify(const Type *field_type, const void *data, const SerializationContext *ctx) = 0; // prior to serialization
	virtual void Restore(const Type *field_type, void *data, const DeserializationContext *ctx) = 0; // after deserialization, work in place
};

//
class TypeField {
public:
	friend class Type;

	TypeField() : type(nullptr) {}
	explicit TypeField(const std::string &hint_) : type(nullptr), hint(hint_) {}
	virtual ~TypeField() {}

	const Type *GetType() const { return type; }
	const std::string &GetTypeName() const { return type_name; }
	const std::string &GetHint() const { return hint; }

	virtual void Get(const void *i, void *v) const = 0;
	virtual void Set(void *i, const void *v) const = 0;

	TypeValue Get(const void *i) const;
	bool Set(void *i, const TypeValue &v) const;

	template <typename T>
	bool GetValue(const void *i, T *out) const;
	template <typename T>
	bool SetValue(void *i, const T &v) const;

	template <typename T>
	void SetDefaultValue(const T &v);
	bool HasDefaultValue() const { return default_value.GetType() != nullptr; }
	const TypeValue &GetDefaultValue() const { return default_value; }

	bool IsDefault(const void *i) const { return default_value_comparator ? default_value_comparator->Compare(Get(i), default_value) : false; }
	bool SetDefault(void *i) const { return default_value.GetType() && Set(i, default_value); }

	void SetSerializationModifier(TypeFieldSerializationModifier *m) { serialization_modifier.reset(m); }
	TypeFieldSerializationModifier *GetSerializationModifier() const { return serialization_modifier.get(); }

	virtual void *GetAddress(void *) const { return nullptr; }
	virtual const void *GetAddress(const void *) const { return nullptr; }

protected:
	const Type *type;
	std::string type_name;

private:
	const std::string hint;

	TypeValue default_value;
	std::unique_ptr<TypeValueComparator> default_value_comparator;
	std::unique_ptr<TypeFieldSerializationModifier> serialization_modifier;
};

template <typename Class, typename Type>
struct ClassField : TypeField {
	ClassField(Type Class::*f);
	ClassField(Type Class::*f, const std::string &hint);

	void Get(const void *i, void *v) const override;
	void Set(void *i, const void *v) const override;

	void *GetAddress(void *o) const override;
	const void *GetAddress(const void *o) const override;

	Type Class::*p;
	size_t size;
};

template <class Class, typename Type, class Accessor>
struct ClassProperty : TypeField {
	ClassProperty();
	explicit ClassProperty(const std::string &hint);

	void Get(const void *i, void *v) const override;
	void Set(void *i, const void *v) const override;
};

//
struct TypeMethod {
	virtual ~TypeMethod() {}

	virtual uint32_t GetArgumentCount() const = 0;
	virtual const Type *GetReturnType() const = 0;
	virtual const Type *GetArgumentType(uint32_t) const = 0;

	virtual bool Call(void *o, uint32_t arg_count = 0, TypeValue **args = nullptr, TypeValue *r_value = nullptr) = 0;
};

template <typename ClassType>
class ClassVoidMethodNoArg : public TypeMethod {
public:
	explicit ClassVoidMethodNoArg(void (ClassType::*m)()) : method(m) {}

	uint32_t GetArgumentCount() const override { return 0; }
	const Type *GetReturnType() const override { return nullptr; }
	const Type *GetArgumentType(uint32_t) const override { return nullptr; }

	bool Call(void *o, uint32_t arg_count, TypeValue **args, TypeValue *r_value = nullptr) override {
		if (arg_count)
			return false;
		ClassType *i = reinterpret_cast<ClassType *>(o);
		(i->*method)();
		return true;
	}

private:
	void (ClassType::*method)();
};

template <typename ClassType, typename ReturnType>
class ClassConstMethod : public TypeMethod {
public:
	ClassConstMethod(TypeRegistry &r, ReturnType (ClassType::*m)() const) : r_type(r.GetType<ReturnType>()), method(m) {}

	uint32_t GetArgumentCount() const override { return 0; }
	const Type *GetReturnType() const override { return r_type; }
	const Type *GetArgumentType(uint32_t) const override { return nullptr; }

	bool Call(void *o, uint32_t arg_count, TypeValue **unused(args), TypeValue *r_value = nullptr) override {
		if (arg_count)
			return false;
		auto i = reinterpret_cast<ClassType *>(o);
		ReturnType r = (i->*method)();
		if (r_value)
			r_value->Set(r_type, &r);
		return true;
	}

private:
	const Type *r_type;
	ReturnType (ClassType::*method)() const;
};

//
template <typename T>
class EnumTypeSerializer : public TypeSerializer {
public:
	explicit EnumTypeSerializer(TypeRegistry &r) : registry(r) {}

	bool Deserialize(DocumentReader &doc, void *o, const Type *type, DeserializationContext *ctx = nullptr, const std::string &name = std::string()) override;
	bool Serialize(DocumentWriter &doc, const void *o, const Type *type, SerializationContext *ctx = nullptr, const std::string &name = std::string()) override;

	const Type *GetSerializeType() const override {
		static const Type *string_type = registry.GetType<std::string>();
		return string_type;
	}

private:
	TypeRegistry &registry;
};

//
struct ContainerReadInterface {
	explicit ContainerReadInterface(const Type *t) : wrapped_type(t) {}
	virtual ~ContainerReadInterface() {}

	inline const Type *GetWrappedType() const { return wrapped_type; }

	virtual size_t GetSize() const = 0;
	virtual TypeValue Get(size_t) const = 0;
	virtual const void *GetAddress(size_t) const = 0;

protected:
	const Type *wrapped_type;
};

struct ContainerWriteInterface {
	explicit ContainerWriteInterface(const Type *t) : wrapped_type(t) {}
	virtual ~ContainerWriteInterface() {}

	inline const Type *GetWrappedType() const { return wrapped_type; }

	virtual bool Reserve(size_t) = 0;
	virtual void Set(size_t, const TypeValue &) = 0;
	virtual void *GetAddress(size_t) const = 0;

protected:
	const Type *wrapped_type;
};

class ContainerInterfaceFactory {
public:
	explicit ContainerInterfaceFactory(const Type *t) : wrapped_type(t) {}
	virtual ~ContainerInterfaceFactory() {}

	virtual ContainerReadInterface *MakeReadInterface(const void *o) const = 0;
	virtual ContainerWriteInterface *MakeWriteInterface(void *o) const = 0;

protected:
	const Type *wrapped_type;
};

//
struct DictionaryReadInterface {
	explicit DictionaryReadInterface(const Type *type) : wrapped_type(type) {}
	virtual ~DictionaryReadInterface() {}

	inline const Type *GetWrappedType() const { return wrapped_type; }

	virtual size_t GetSize() const = 0; // number of keys in the dictionary
	virtual std::string GetKey(size_t) const = 0;
	virtual TypeValue GetValue(const std::string &) const = 0;
	virtual bool IsDefault(const TypeValue &) const = 0; // get the dictionary default value

protected:
	const Type *wrapped_type;
};

struct DictionaryWriteInterface {
	explicit DictionaryWriteInterface(const Type *type) : wrapped_type(type) {}
	virtual ~DictionaryWriteInterface() {}

	inline const Type *GetWrappedType() const { return wrapped_type; }

	virtual void Clear() = 0;
	virtual bool Set(const std::string &, const TypeValue &) = 0;

protected:
	const Type *wrapped_type;
};

class DictionaryInterfaceFactory {
public:
	explicit DictionaryInterfaceFactory(const Type *t) : wrapped_type(t) {}
	virtual ~DictionaryInterfaceFactory() {}

	virtual DictionaryReadInterface *MakeReadInterface(const void *o) const = 0;
	virtual DictionaryWriteInterface *MakeWriteInterface(void *o) const = 0;

protected:
	const Type *wrapped_type;
};

//
struct EnumCast {
	EnumCast() {}
	virtual ~EnumCast() {}

	virtual TypeValue ToEnum(uint32_t) = 0;
	virtual uint32_t FromEnum(const TypeValue &) = 0;
};

template <typename T>
struct ConcreteEnumCast : public EnumCast {
	explicit ConcreteEnumCast(TypeRegistry &r) : registry(r) {}

	TypeValue ToEnum(uint32_t v) override {
		T e = static_cast<T>(v);
		return TypeValue(registry.GetType<T>(), &e);
	}

	uint32_t FromEnum(const TypeValue &v) override { return uint32_t(v.Cast<T>()); }

	TypeRegistry &registry;
};

//
struct SharedPtrAccessor {
	virtual ~SharedPtrAccessor() {}

	virtual void *Get(const void *s_ptr) const = 0;
	virtual void Set(void *s_ptr, void *new_s_ptr) = 0; // *** new_s_ptr must point to std::shared_ptr<T> ***
	virtual Type *GetType() const = 0;
};

template <typename T>
class ConcreteSharedPtrAccessor : public SharedPtrAccessor {
public:
	explicit ConcreteSharedPtrAccessor(Type *wrapped_type) : type(wrapped_type) {}

	void *Get(const void *s_ptr) const override { return reinterpret_cast<const std::shared_ptr<T> *>(s_ptr)->get(); }
	void Set(void *s_ptr, void *new_value) override { *reinterpret_cast<std::shared_ptr<T> *>(s_ptr) = *reinterpret_cast<std::shared_ptr<T> *>(new_value); }
	Type *GetType() const override { return type; }

private:
	Type *type;
};

//
bool CompareType(const Type *a, const Type *b);

/// Native C++ type as seen by the reflection system
class Type {
	friend class TypeRegistry;

public:
	typedef std::map<std::string, std::unique_ptr<TypeField>> FieldMap;
	typedef std::map<std::string, std::unique_ptr<TypeMethod>> MethodMap;
	typedef std::map<std::string, uint32_t> EnumMap;

private:
	TypeRegistry *registry;
	std::unique_ptr<TypeFactory> factory;

	std::string name;
	std::string cpp_name; // the type name as known to C++
	size_t size;

	struct TypeCaster {
		virtual void *Upcast(void *) = 0;
		virtual const void *Upcast(const void *) = 0;
		virtual void *Downcast(void *) = 0;
		virtual const void *Downcast(const void *) = 0;
		virtual ~TypeCaster() {}
	};

	template <typename T, typename B>
	struct ConcreteTypeCaster : public TypeCaster {
		virtual void *Upcast(void *t) { return static_cast<B *>(reinterpret_cast<T *>(t)); }
		virtual const void *Upcast(const void *t) { return static_cast<const B *>(reinterpret_cast<const T *>(t)); }
		virtual void *Downcast(void *b) { return static_cast<T *>(reinterpret_cast<B *>(b)); }
		virtual const void *Downcast(const void *b) { return static_cast<const T *>(reinterpret_cast<const B *>(b)); }
	};

	struct BaseType {
		BaseType(Type *t, TypeCaster *c) : type(t), type_caster(c) {}
		BaseType(BaseType &&t) : type(t.type), type_caster(std::move(t.type_caster)) {}

		Type *type;
		std::unique_ptr<TypeCaster> type_caster;
	};

	std::vector<BaseType> base_types;

	std::unique_ptr<SharedPtrAccessor> shared_ptr_accessor;

	FieldMap fields;
	MethodMap methods;

	EnumMap values;
	std::unique_ptr<EnumCast> enum_cast;

	std::unique_ptr<TypeSerializer> serializer;
	std::unique_ptr<ContainerInterfaceFactory> container_interface_factory;
	std::unique_ptr<DictionaryInterfaceFactory> dictionary_interface_factory;

	std::string root_scope; // root scope name when serialized

public:
	inline TypeRegistry *GetRegistry() const { return registry; }
	inline const TypeFactory *GetFactory() const { return factory.get(); }

	inline const std::string &GetName() const { return name; }
	inline const std::string &GetCppName() const { return cpp_name; }
	inline size_t GetSize() const { return size; }
	inline TypeId GetTypeId() const { return std::hash<std::string>{}(name); }

	//
	Type &SetCppName(const std::string &_name) {
		cpp_name = _name;
		registry->CreateAlias(_name, this);
		return *this;
	}

	//
	template <typename T>
	Type &SetSharedPtr() {
		shared_ptr_accessor.reset(new ConcreteSharedPtrAccessor<T>(registry->GetType<T>()));
		return *this;
	}

	SharedPtrAccessor *GetSharedPtrAccessor() const { return shared_ptr_accessor.get(); }

	//
	template <typename T, typename B>
	Type &SetBase() {
		base_types.emplace_back(registry->GetType<B>(), new ConcreteTypeCaster<T, B>);
		return *this;
	}

	template <typename T, typename B0, typename B1>
	Type &SetBase() {
		base_types.reserve(2);
		base_types.emplace_back(registry->GetType<B0>(), new ConcreteTypeCaster<T, B0>);
		base_types.emplace_back(registry->GetType<B1>(), new ConcreteTypeCaster<T, B1>);
		return *this;
	}

	template <typename T, typename B0, typename B1, typename B2>
	Type &SetBase() {
		base_types.reserve(3);
		base_types.emplace_back(registry->GetType<B0>(), new ConcreteTypeCaster<T, B0>);
		base_types.emplace_back(registry->GetType<B1>(), new ConcreteTypeCaster<T, B1>);
		base_types.emplace_back(registry->GetType<B2>(), new ConcreteTypeCaster<T, B2>);
		return *this;
	}

	inline size_t GetBaseTypeCount() const { return base_types.size(); }
	inline Type *GetBaseType(size_t index) const { return base_types[index].type; }

	bool Inherits(const Type *t) const {
		if (CompareType(this, t))
			return true;
		for (size_t n = 0; n < base_types.size(); ++n)
			if (base_types[n].type->Inherits(t))
				return true;
		return false;
	}

	template <typename T>
	bool Inherits() const { return Inherits(registry->GetType<T>()); }

	void *Downcast(void *p, const Type *from) const; // downcast from a base type to a derived type
	const void *Downcast(const void *p, const Type *from) const { return Downcast(const_cast<void *>(p), from); }

	template <typename T>
	void *Downcast(T *p) const { return Downcast(p, registry->GetType<T>()); }
	template <typename T>
	const void *Downcast(const T *p) const { return Downcast(p, registry->GetType<T>()); }

	void *Upcast(void *p, const Type *to) const; // upcast from derived to base type
	const void *Upcast(const void *p, const Type *to) const { return Upcast(const_cast<void *>(p), to); }

	template <typename T>
	void *Upcast(T *p) const { return Upcast(p, registry->GetType<T>()); }
	template <typename T>
	const void *Upcast(const T *p) const { return Upcast(p, registry->GetType<T>()); }

	void *Cast(void *p, const Type *to) const;
	const void *Cast(const void *p, const Type *to) const { return Cast(const_cast<void *>(p), to); }

	template <typename T>
	T *Cast(void *p) const {
		auto to = registry->GetType<T>();
		__ASSERT__(to != nullptr);
		return reinterpret_cast<T *>(Cast(p, to));
	}

	template <typename T>
	const T *Cast(const void *p) const { return Cast<T>(const_cast<void *>(p)); }

	//
	inline const std::string &GetRootScopeName() const { return root_scope; }

	Type &SetRootScopeName(const std::string &n) {
		root_scope = n;
		return *this;
	}

	//
	Type &AddField(const std::string &field_name, TypeField *field, bool serialized = true, bool muted = false) {
		field->type = registry->GetType(field->GetTypeName());
		fields[field_name].reset(field);
		if (!serialized)
			registry->unserialized_type_fields[this].push_back(field);
		if (muted)
			registry->muted_type_fields[this].push_back(field);
		return *this;
	}

	Type &AddFieldSetSerializationModifier(const std::string &field_name, TypeField *field, TypeFieldSerializationModifier *modifier, bool serialized = true, bool muted = false) {
		AddField(field_name, field, serialized, muted);
		field->SetSerializationModifier(modifier);
		return *this;
	}

	template <typename T>
	Type &AddFieldDefault(const std::string &field_name, TypeField *field, const T &default_value, bool serialized = true, bool muted = false) {
		AddField(field_name, field, serialized, muted);
		field->SetDefaultValue(default_value);
		return *this;
	}

	template <typename T>
	Type &AddFieldDefaultSetSerializationModifier(const std::string &field_name, TypeField *field, const T &default_value, TypeFieldSerializationModifier *modifier, bool serialized = true, bool muted = false) {
		AddField(field_name, field, serialized, muted);
		field->SetDefaultValue(default_value);
		field->SetSerializationModifier(modifier);
		return *this;
	}

	bool IsFieldSerialized(TypeField *field) const {
		auto &unserialized_fields = registry->unserialized_type_fields[const_cast<Type *>(this)];
		return std::find(unserialized_fields.begin(), unserialized_fields.end(), field) == unserialized_fields.end();
	}

	bool IsFieldMuted(TypeField *field) const {
		auto &muted_fields = registry->unserialized_type_fields[const_cast<Type *>(this)];
		return std::find(muted_fields.begin(), muted_fields.end(), field) != muted_fields.end();
	}

	TypeField *GetField(const std::string &field_name) const {
		auto i = fields.find(field_name);
		return i != fields.end() ? (*i).second.get() : nullptr;
	}

	FieldMap::const_iterator GetField(uint32_t idx) const {
		auto i = fields.begin();
		std::advance(i, idx);
		return i;
	}

	inline const FieldMap &GetFields() const { return fields; }

	//
	Type &AddMethod(const std::string &method_name, TypeMethod *method) {
		methods[method_name].reset(method);
		return *this;
	}

	TypeMethod *GetMethod(const std::string &method_name) const {
		auto i = methods.find(method_name);
		return i != methods.end() ? (*i).second.get() : nullptr;
	}

	inline const MethodMap &GetMethods() const { return methods; }

	//
	template <typename T>
	Type &SetEnum() {
		serializer.reset(new EnumTypeSerializer<T>(*registry));
		enum_cast.reset(new ConcreteEnumCast<T>(*registry));
		return *this;
	}

	inline bool IsEnum() const { return !values.empty(); }

	template <typename T>
	Type &AddValue(const std::string &enum_value_name, T enum_value) {
		values[enum_value_name] = uint32_t(enum_value);
		return *this;
	}

	inline const EnumMap &GetValues() const { return values; }

	inline EnumCast *GetEnumCast() const { return enum_cast.get(); }

	//
	inline bool IsContainer() const { return bool(container_interface_factory); }

	ContainerReadInterface *MakeContainerReadInterface(const void *o) const { return IsContainer() ? container_interface_factory->MakeReadInterface(o) : nullptr; }
	ContainerWriteInterface *MakeContainerWriteInterface(void *o) const { return IsContainer() ? container_interface_factory->MakeWriteInterface(o) : nullptr; }

	Type &SetContainer(ContainerInterfaceFactory *f) {
		container_interface_factory.reset(f);
		return *this;
	}

	//
	inline bool IsDictionary() const { return bool(dictionary_interface_factory); }

	DictionaryReadInterface *MakeDictionaryReadInterface(const void *o) const { return IsDictionary() ? dictionary_interface_factory->MakeReadInterface(o) : nullptr; }
	DictionaryWriteInterface *MakeDictionaryWriteInterface(void *o) const { return IsDictionary() ? dictionary_interface_factory->MakeWriteInterface(o) : nullptr; }

	Type &SetDictionary(DictionaryInterfaceFactory *f) {
		dictionary_interface_factory.reset(f);
		return *this;
	}

	//
	void SetSerializer(TypeSerializer *s) { serializer.reset(s); }
	inline TypeSerializer *GetSerializer() const { return serializer.get(); }

	//
	inline const Type *GetSerializeType() const { return serializer ? serializer->GetSerializeType() : this; }

	//
	Type(TypeRegistry *registry_, const std::string &name_) : registry(registry_), factory(nullptr), name(name_), cpp_name(name_), size(0) {}
};

//
inline bool CompareType(const Type *a, const Type *b) { return a == b || a->GetTypeId() == b->GetTypeId(); }

//
void TypeSetDefault(const Type *type, void *i);
/**
	@short Test if an instance of type is initialized with default values only.

	This test will pass if TypeSetDefault() called on a newly allocated instance
	of the same type would produce the same object in memory.
	This test does not consider default values set by a type constructor.
*/
bool TypeIsDefault(const Type *type, const void *i);

//
template <typename T>
Type *TypeRegistry::HasType() { return HasType(GetTypeName<T>()); }
template <typename T>
Type *TypeRegistry::GetType() { return GetType(GetTypeName<T>()); }
template <typename T>
Type &TypeRegistry::CreateAbstractType() { return *GetType(GetTypeName<T>()); }

template <typename T>
Type &TypeRegistry::CreateType() {
	Type &type = CreateAbstractType<T>();
	type.factory.reset(new ConcreteTypeFactory<T>(&type));
	type.size = sizeof(T);
	return type;
}

//
template <typename T>
bool TypeField::GetValue(const void *i, T *v) const {
	if (type != type->GetRegistry()->GetType<T>())
		return false;
	Get(i, v);
	return true;
}

template <typename T>
bool TypeField::SetValue(void *i, const T &v) const {
	if (type != type->GetRegistry()->GetType<T>())
		return false;
	Set(i, &v);
	return true;
}

template <typename T>
void TypeField::SetDefaultValue(const T &v) {
	__ASSERT_MSG__(type->GetTypeId() == GetTypeId<T>(), "incorrect field default value type");
	default_value = TypeValue(type, &v);
	default_value_comparator.reset(new ConcreteTypeValueComparator<T>());
}

//
template <typename Class, typename Type>
void ClassField<Class, Type>::Get(const void *i, void *v) const {
	*reinterpret_cast<Type *>(v) = reinterpret_cast<const Class *>(i)->*p;
}

template <typename Class, typename Type>
void ClassField<Class, Type>::Set(void *i, const void *v) const {
	reinterpret_cast<Class *>(i)->*p = *reinterpret_cast<const Type *>(v);
}

template <typename Class, typename Type>
void *ClassField<Class, Type>::GetAddress(void *o) const { return static_cast<void *>(&(reinterpret_cast<Class *>(o)->*p)); }
template <typename Class, typename Type>
const void *ClassField<Class, Type>::GetAddress(const void *o) const { return static_cast<const void *>(&(reinterpret_cast<const Class *>(o)->*p)); }

template <typename Class, typename Type>
ClassField<Class, Type>::ClassField(Type Class::*f) : p(f), size(sizeof(Type)) { type_name = hg::GetTypeName<Type>(); }
template <typename Class, typename Type>
ClassField<Class, Type>::ClassField(Type Class::*f, const std::string &h) : TypeField(h), p(f), size(sizeof(Type)) { type_name = hg::GetTypeName<Type>(); }

//
template <class Class, typename Type, class Accessor>
void ClassProperty<Class, Type, Accessor>::Get(const void *i, void *v) const {
	*reinterpret_cast<Type *>(v) = Accessor::Get(*reinterpret_cast<const Class *>(i));
}

template <typename Class, typename Type, class Accessor>
void ClassProperty<Class, Type, Accessor>::Set(void *i, const void *v) const {
	Accessor::Set(*reinterpret_cast<Class *>(i), *reinterpret_cast<const Type *>(v));
}

template <class Class, typename Type, class Accessor>
ClassProperty<Class, Type, Accessor>::ClassProperty() { type_name = hg::GetTypeName<Type>(); }
template <class Class, typename Type, class Accessor>
ClassProperty<Class, Type, Accessor>::ClassProperty(const std::string &h) : TypeField(h) {
	type_name = hg::GetTypeName<Type>();
}

//
template <typename T>
void TypeValue::Set(const T *in) { Set(g_type_registry.get().GetType<T>(), in); }

// TypeValue casts
template <typename T>
T &TypeValue::Cast(const Type *to) const {
	__ASSERT__(type != nullptr);
	__ASSERT_MSG__((to == nullptr) || (type == to), "invalid TypeValue cast");
	return *reinterpret_cast<T *>(data);
}

template <typename T>
T &TypeValue::CastDefault(const Type *to, T &on_fail) const {
	return type != nullptr && type == to ? *reinterpret_cast<T *>(data) : on_fail;
}

template <typename T>
const T &TypeValue::CastDefault(const Type *to, const T &on_fail) const {
	return type != nullptr && type == to ? *reinterpret_cast<T *>(data) : on_fail;
}

template <>
int &TypeValue::Cast<int>(const Type *to) const;

template <>
int &TypeValue::CastDefault<int>(const Type *to, int &on_fail) const;

template <>
const int &TypeValue::CastDefault<int>(const Type *to, const int &on_fail) const;

template <typename T>
T &TypeValue::Cast() const { return Cast<T>(type->GetRegistry()->GetType<T>()); }

template <typename T>
T &TypeValue::CastDefault(T &on_fail) const { return CastDefault<T>(type->GetRegistry()->GetType<T>(), on_fail); }

template <typename T>
const T &TypeValue::CastDefault(const T &on_fail) const { return CastDefault<T>(type->GetRegistry()->GetType<T>(), on_fail); }

//
template <typename T>
TypeValue::TypeValue(const T &v) : type(nullptr), data(nullptr) { Set(g_type_registry.get().GetType<T>(), &v); }

template <typename T>
TypeValue MakeTypeValue(TypeRegistry &r, const T &v) { return TypeValue(g_type_registry.get().GetType<T>(), &v); }

template <typename T>
TypeValue MakeTypeValue(const T &v) { return TypeValue(g_type_registry.get().GetType<T>(), &v); }

//
template <typename T>
bool ConcreteTypeValueComparator<T>::Compare(const TypeValue &a, const TypeValue &b) const { return a.GetType() == b.GetType() && a.GetType() == a.GetType()->GetRegistry()->GetType<T>() && *reinterpret_cast<const T *>(a.GetData()) == *reinterpret_cast<const T *>(b.GetData()); }

//
template <typename T>
bool EnumTypeSerializer<T>::Deserialize(DocumentReader &doc, void *o, const Type *type, DeserializationContext *unused(ctx), const std::string &name) {
	__ASSERT__(type->IsEnum());
	std::string v;
	if (!doc.Read(name, v))
		return false;

	auto p = type->GetValues().find(v);
	if (p == type->GetValues().end())
		return false;

	*reinterpret_cast<T *>(o) = static_cast<T>((*p).second);
	return true;
}

template <typename T>
bool EnumTypeSerializer<T>::Serialize(DocumentWriter &doc, const void *o, const Type *type, SerializationContext *unused(ctx), const std::string &name) {
	__ASSERT__(type->IsEnum());
	auto &values = type->GetValues();
	uint32_t v = static_cast<uint32_t>(*reinterpret_cast<const T *>(o));
	auto i = std::find_if(values.begin(), values.end(), [v](const std::pair<std::string, uint32_t> &pair) { return pair.second == v; });
	return i != values.end() && doc.Write(name, i->first);
}

//
template <class C>
C *DeserializationStack::FindInstance() const {
	static const auto type = g_type_registry.get().GetType<C>();
	auto entry = this->FindType(type);
	return entry ? reinterpret_cast<C *>(entry->i) : nullptr;
}

template <class C>
const C *SerializationStack::FindInstance() const {
	static const auto type = g_type_registry.get().GetType<C>();
	auto entry = this->FindType(type);
	return entry ? reinterpret_cast<const C *>(entry->i) : nullptr;
}

} // namespace hg
