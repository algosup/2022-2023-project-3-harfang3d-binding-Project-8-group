// HARFANG�3D Copyright (C) 2019 Emmanuel Julien, Movida Production. Released under GPL/LGPL/Commercial Licence, see licence.txt for details.

#pragma once

#include <cstdint>
#include <cstddef>

namespace hg {

/**
    @short	Perform UUEncoding.

    @param	in			Input binary stream.
    @param	len			Input binary stream length.
    @param	out			Output buffer.
    @param	max			Output buffer length.
    @return				The number of bytes written to the output buffer.

    @note	If yenc is nullptr the function can be used to check the size
            of the encoded stream prior to allocating an output buffer.
*/
uint32_t UUEncode(const void *in, size_t len, void *out = nullptr, size_t max = 0);

/**
    @short	Perform UUDecoding.

    @param	in			UUEncoded byte stream.
    @param	len			UUEncoded byte stream length.
    @param	out			Output buffer.
    @param	max			Output buffer length.
    @return				The number of bytes written to the output buffer.

    @note	If bin is nullptr the function can be used to check the size
            of the decoded stream prior to allocating an output buffer.
*/
uint32_t UUDecode(const void *in, size_t len, void *out = nullptr, size_t max = 0);
/**
    @short	Perform yEncoding.

    @param	in			Input binary stream.
    @param	len			Input binary stream length.
    @param	out			Output buffer.
    @param	max			Output buffer length.
    @param	line_len	Number of character to output before a line-feed character is sent.
    @return				The number of bytes written to the output buffer.

    @note	If yenc is nullptr the function can be used to check the size
            of the encoded stream prior to allocating an output buffer.
*/
uint32_t yEncode(const void *in, size_t len, void *out = nullptr, size_t max = 0, uint32_t line_len = 64);

/**
    @short	Perform yDecoding.

    @param	in			yEncoded byte stream.
    @param	len			yEncoded byte stream length.
    @param	out			Output buffer.
    @param	max			Output buffer length.
    @return				The number of bytes written to the output buffer.

    @note	If bin is nullptr the function can be used to check the size
            of the decoded stream prior to allocating an output buffer.
*/
uint32_t yDecode(const void *in, size_t len, void *out = nullptr, size_t max = 0);

} // namespace hg
