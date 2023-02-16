// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/document.h"
#include <memory>
#include <string>
#include <cstring>
#include <vector>

namespace hg {

class BinaryData;

enum BinaryChunkType {
	Type_Scope,
	Type_Bool,
	Type_Int,
	Type_Unsigned,
	Type_Float,
	Type_String,
	Type_Binary
};

/// Reader for binary document
class BinaryDocumentReader : public DocumentReader {
public:
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

	bool HasBinarySupport() const override { return true; }
	bool Read(const std::string &name, BinaryData &out) override;

	bool Load(const BinaryData &data);
	bool Load(const std::string &path) override;

private:
	std::vector<char> data;
	size_t cursor{0};

	bool LoadInternal();

	struct Chunk;
	typedef std::vector<std::unique_ptr<Chunk>> ChunkList;

	struct Chunk {
		uint16_t name_index{0};
		const char *data{nullptr};
		size_t offset{0}, size{0};
		BinaryChunkType type{Type_Scope};

		ChunkList chunks;
		ChunkList::iterator current_scope_index;
	};

	size_t chunk_child_index{0};

	char *OffsetToAddress(size_t o) { return &data[o]; }

	std::vector<const char *> dictionary;
	const char *GetString(uint16_t index) const;
	bool LoadDictionary(size_t, size_t);

	bool ParseChunk(Chunk *);
	bool ParseChunkTree(size_t, size_t, ChunkList &);

	ChunkList &GetCurrentChunks();
	ChunkList::iterator &GetCurrentScopeIndex();

	Chunk *CurrentChunk() const;
	Chunk *CurrentChunkChild(const std::string &name);

	mutable ChunkList::iterator current_scope_index;
	bool EnterChunk(Chunk *s);

	ChunkList root_chunks;
	mutable std::vector<Chunk *> stack;
};

/// Writer for binary document
class BinaryDocumentWriter : public DocumentWriter {
public:
	bool EnterScope(const std::string &name) override;
	bool ExitScope() const override;

	bool Write(const std::string &name, bool) override;
	bool Write(const std::string &name, int32_t) override;
	bool Write(const std::string &name, uint32_t) override;
	bool Write(const std::string &name, float) override;
	bool Write(const std::string &name, const std::string &) override;
	/// Prevent implicit cast from const char * to bool.
	bool Write(const std::string &name, const char *v) { return Write(name, std::string(v)); }

	bool HasBinarySupport() const override { return true; }
	bool Write(const std::string &name, const BinaryData &data) override;

	bool Save(BinaryData &out);
	bool Save(const std::string &path) override;

private:
	struct Scope {
		size_t o_scope{0};
		size_t o_scope_size{0};
	};

	mutable std::vector<Scope> stack;

	mutable std::vector<char> data;
	size_t cursor{0};

	template <typename T>
	bool TWrite(const T &v) {
		while (cursor + sizeof(T) > data.size())
			data.resize(data.size() + 32768);

		*reinterpret_cast<T *>(&data[cursor]) = v;
		cursor += sizeof(T);

		return true;
	}

	bool BWrite(const void *p, size_t size) {
		if (cursor + size > data.size())
			data.resize(data.size() + size + 32768);

		memcpy(&data[cursor], p, size);
		cursor += size;
		return true;
	}

	inline char *OffsetToAddress(size_t o) const { return &data[o]; }

	std::vector<std::string> dictionary;

	bool OutputString(const std::string &name);
	bool SaveDictionary(BinaryData &data) const;
};

} // namespace hg
