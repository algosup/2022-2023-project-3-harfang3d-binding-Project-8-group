// HARFANG®3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include "foundation/binary_data.h"
#include "foundation/binary_document.h"
#include "foundation/json_document.h"
#include "foundation/log.h"
#include "foundation/type_to_document.h"
#include "foundation/xml_document.h"

namespace hg {

std::unique_ptr<DocumentReader> NewResourceDocumentReader(const std::string &path, DocumentFormat format = DocumentFormatUnknown);
std::unique_ptr<DocumentWriter> NewResourceDocumentWriter(const std::string &path, DocumentFormat format = DocumentFormatUnknown);

//
struct SerializedObject {
	BinaryData data;
	size_t GetSize() const { return data.GetDataSize(); }
	bool Save(const std::string &path) const;
};

/// Deserialize a resource object from an object.
template <class T>
bool LoadResourceFromObject(SerializedObject *object, T &o, DeserializationContext *ctx = nullptr) {
	BinaryDocumentReader doc;
	if (!object || !doc.Load(object->data))
		return false;
	return Deserialize(doc, &o, g_type_registry.get().GetType<T>(), ctx);
}

/// Serialize a resource object to disk.
template <class T>
SerializedObject *SaveResourceToObject(const T &o, SerializationContext *ctx = nullptr) {
	BinaryDocumentWriter doc;
	if (!Serialize(doc, &o, g_type_registry.get().GetType<T>(), ctx))
		return nullptr;
	std::unique_ptr<SerializedObject> object(new SerializedObject);
	return doc.Save(object->data) ? object.release() : nullptr;
}

//
/// Deserialize a resource object from disk.
template <class T>
bool LoadResourceFromPath(const std::string &path, T &o, DocumentFormat format = DocumentFormatUnknown, DeserializationContext *ctx = nullptr) {
	std::unique_ptr<DocumentReader> doc(NewResourceDocumentReader(path, format));
	if (!doc)
		doc.reset(new BinaryDocumentReader);
	return doc->Load(path) && Deserialize(*doc, &o, g_type_registry.get().GetType<T>(), ctx);
}

/// Serialize a resource object to disk.
template <class T>
bool SaveResourceToPath(const std::string &path, const T &o, DocumentFormat format = DocumentFormatUnknown, SerializationContext *ctx = nullptr) {
	std::unique_ptr<DocumentWriter> doc(NewResourceDocumentWriter(path, format));
	if (!doc)
		doc.reset(new BinaryDocumentWriter);
	return Serialize(*doc, &o, g_type_registry.get().GetType<T>(), ctx) && doc->Save(path);
}

//
/// Deserialize a resource object from a string.
template <class T>
bool LoadResourceFromString(const std::string &document, T &o, DocumentFormat format = DocumentFormatUnknown, DeserializationContext *ctx = nullptr) {
	switch (format) {
		default:
			break;

		case DocumentFormatJSON: {
			JSONDocumentReader doc;
			return doc.FromString(document) && Deserialize(doc, &o, g_type_registry.get().GetType<T>(), ctx);
		}

		case DocumentFormatXML: {
			XMLDocumentReader doc;
			return doc.FromString(document) && Deserialize(doc, &o, g_type_registry.get().GetType<T>(), ctx);
		}
	}
	return false;
}

/// Serialize a resource object to a string.
template <class T>
bool SaveResourceToString(std::string &out, const T &o, DocumentFormat format, SerializationContext *ctx = nullptr) {
	switch (format) {
		default:
			break;

		case DocumentFormatJSON: {
			JSONDocumentWriter doc;
			if (!Serialize(doc, &o, g_type_registry.get().GetType<T>(), ctx))
				return false;
			out = doc.ToString();
			return true;
		}

		case DocumentFormatXML: {
			XMLDocumentWriter doc;
			if (!Serialize(doc, &o, g_type_registry.get().GetType<T>(), ctx))
				return false;
			out = doc.ToString();
			return true;
		}
	}
	return false;
}

} // namespace hg
