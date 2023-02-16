// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstdint>
#include <string>

namespace hg {

class BinaryData;

struct DocumentReader {
	enum ValueType {
		Null, Bool, Int32, UInt32, Float, String
	};

	virtual ~DocumentReader() {}

	virtual std::string GetScopeName() const = 0;

	/// Get the number of scope children with a specific name.
	uint32_t GetChildCount();
	uint32_t GetChildCount(const std::string &name);

	/**
		@short Enter a child scope with a specific name.
		@note This helper function simply wraps EnterFirstChild and EnterSibling
		calls.
	*/
	bool EnterScope(const std::string &name);
	bool EnterScopeMultiple(const std::string &name);
	bool ExitScopeMultiple(uint32_t count);

	/**
		@short Enter the first child scope to the current scope.
		If no such scope exists this function returns false.
	*/
	virtual bool EnterFirstChild() = 0;
	/**
		@short Enter the sibling scope to the current scope.
		If no such scope exists the parent scope is entered and this function
		returns false.
	*/
	virtual bool EnterSibling() = 0;
	/// Exit current scope.
	virtual bool ExitScope() = 0;

	/**
		@short Return the first key in the current scope.
		Returns false if the current scope has no key/value pair.
	*/
	virtual bool GetFirstKey(std::string &key, ValueType &type) = 0;
	virtual bool GetNextKey(std::string &key, ValueType &type) = 0;

	virtual bool Read(const std::string &name, bool &) = 0;
	virtual bool Read(const std::string &name, int32_t &) = 0;
	virtual bool Read(const std::string &name, uint32_t &) = 0;
	virtual bool Read(const std::string &name, float &) = 0;
	virtual bool Read(const std::string &name, std::string &) = 0;

	bool Read(const std::string &name, char &v); // stored as int32_t
	bool Read(const std::string &name, uint8_t &v); // stored as uint32_t
	bool Read(const std::string &name, short &v); // stored as int32_t
	bool Read(const std::string &name, uint16_t &v); // stored as uint32_t

	virtual bool HasBinarySupport() const { return false; }
	virtual bool Read(const std::string &, BinaryData &) { return false; }

	virtual bool Load(const std::string &path) = 0;
};

struct DocumentWriter {
	virtual ~DocumentWriter() {}

	bool EnterScopeMultiple(const std::string &name);
	bool ExitScopeMultiple(uint32_t count);

	virtual bool EnterScope(const std::string &name) = 0;
	virtual bool ExitScope() const = 0;

	virtual bool Write(const std::string &name, bool) = 0;
	virtual bool Write(const std::string &name, int32_t) = 0;
	virtual bool Write(const std::string &name, uint32_t) = 0;
	virtual bool Write(const std::string &name, float) = 0;
	virtual bool Write(const std::string &name, const std::string &) = 0;

	bool Write(const std::string &name, char); // stored as int32_t
	bool Write(const std::string &name, uint8_t); // stored as uint32_t
	bool Write(const std::string &name, short); // stored as int32_t
	bool Write(const std::string &name, uint16_t); // stored as uint32_t

	bool Write(const std::string &name, const char *v) { return Write(name, std::string(v)); }

	virtual bool HasBinarySupport() const { return false; }
	virtual bool Write(const std::string &, const BinaryData &) { return false; }

	virtual bool Save(const std::string &path) = 0;
};

class WriteScopeGuard {
public:
	WriteScopeGuard(const WriteScopeGuard &) = delete;
	WriteScopeGuard(DocumentWriter &d) : doc(d) {}
	WriteScopeGuard(DocumentWriter &d, const std::string &name) : doc(d) { has_entered = doc.EnterScope(name); }
	~WriteScopeGuard() { Exit(); }

	/// Return true if this guard will exit scope when destroyed.
	inline bool HasEnteredScope() const { return has_entered; }
	/// Explicitly enter a scope.
	bool Enter(const std::string &name) { return has_entered ? false : (has_entered = doc.EnterScope(name)); }

	/// Exit the guarded scope, returns false if no scope was exited.
	bool Exit();

private:
	DocumentWriter &doc;
	bool has_entered{false};
};

enum DocumentFormat {
	DocumentFormatUnknown,
	DocumentFormatXML,
	DocumentFormatJSON,
	DocumentFormatBinary
};

DocumentFormat GetDocumentReadFormat(const std::string &path);
DocumentFormat GetDocumentWriteFormat(const std::string &path);

DocumentFormat GetDocumentFormatFromString(const std::string &document);

} // namespace hg
