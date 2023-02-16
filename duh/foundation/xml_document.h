// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/document.h"
#include <memory>

namespace hg {

struct XML_reader_pimpl;

class XMLDocumentReader : public DocumentReader {
public:
	XMLDocumentReader();
	~XMLDocumentReader();

	std::string GetScopeName() const override;

	bool EnterFirstChild() override;
	bool EnterSibling() override;
	bool ExitScope() override;

	bool GetFirstKey(std::string &key, ValueType &type) override;
	bool GetNextKey(std::string &key, ValueType &type) override;

	bool Read(const std::string &name, bool &) override;
	bool Read(const std::string &name, int32_t &) override;
	bool Read(const std::string &name, uint32_t &) override;
	bool Read(const std::string &name, float &) override;
	bool Read(const std::string &name, std::string &) override;

	bool Load(const std::string &path) override;
	bool FromString(const std::string &xml);

private:
	std::unique_ptr<XML_reader_pimpl> reader;
};

struct XML_writer_pimpl;

class XMLDocumentWriter : public DocumentWriter {
public:
	XMLDocumentWriter();
	~XMLDocumentWriter();

	bool EnterScope(const std::string &name) override;
	bool ExitScope() const override;

	bool Write(const std::string &name, bool) override;
	bool Write(const std::string &name, int32_t) override;
	bool Write(const std::string &name, uint32_t) override;
	bool Write(const std::string &name, float) override;
	bool Write(const std::string &name, const std::string &) override;
	/// Prevent implicit cast from const char * to bool.
	bool Write(const std::string &name, const char *v) { return Write(name, std::string(v)); }

	bool Save(const std::string &path) override;
	std::string ToString() const;

private:
	std::unique_ptr<XML_writer_pimpl> writer;
};

} // namespace hg
