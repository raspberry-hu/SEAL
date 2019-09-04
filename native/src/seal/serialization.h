// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#pragma once

#include <iostream>
#include <cstdint>
#include <functional>
#include "seal/util/defines.h"

namespace seal
{
    /**
    A type to describe the compression algorithm applied to serialized data.
    Ciphertext and key data consist of a large number of 64-bit words storing
    integers modulo prime numbers much smaller than the word size, resulting in
    a large number of zero bytes in the output. Any compression algorithm should
    be able to clean up these zero bytes and hence compress both ciphertext and
    key data.
    */
    enum class compr_mode_type : std::uint8_t
    {
        none = 0,
#ifdef SEAL_USE_ZLIB
        zlib = 1
#endif
    };

    /**
    The compression mode used by default.
    */
#ifdef SEAL_USE_ZLIB
    constexpr compr_mode_type compr_mode_default = compr_mode_type::zlib;
#else
    constexpr compr_mode_type compr_mode_default = compr_mode_type::none;
#endif
    /**
    Class to provide functionality for compressed serialization. Most users of
    the library should never have to call these functions explicitly, as they are
    called internally by functions such as Ciphertext::save and Ciphertext::load.
    */
    class Serialization
    {
    public:
        /**
        Evaluates save_members and compresses the output according to the given
        compr_mode_type. The resulting data is written to stream and is prepended
        by the given compr_mode_type and the total size of the data to facilitate
        deserialization. In typical use-cases save_members would be a function
        that serializes the member variables of an object to the given stream.

        @param[in] save_members A function taking an std::ostream reference as an
        argument, possibly writing some number of bytes into it
        @param[out] stream The stream to write to
        @param[in] compr_mode The desired compression mode
        */
        static std::streamoff Save(
            std::function<void(std::ostream &stream)> save_members,
            std::ostream &stream,
            compr_mode_type compr_mode);

        /**
        Deserializes data from stream that was serialized by Save. Once stream has
        been decompressed (depending on compression mode), load_members is applied
        to the decompressed stream. In typical use-cases load_members would be
        a function that deserializes the member variables of an object from the
        given stream.

        @param[in] load_members A function taking an std::istream reference as an
        argument, possibly reading some number of bytes from it
        @param[in] stream The stream to read from
        */
        static std::streamoff Load(
            std::function<void(std::istream &stream)> load_members,
            std::istream &stream);

        /**
        Evaluates save_members and compresses the output according to the given
        compr_mode_type. The resulting data is written to a given memory location
        and is prepended by the given compr_mode_type and the total size of the
        data to facilitate deserialization. In typical use-cases save_members would
        be a function that serializes the member variables of an object to the
        given stream.

        @param[in] save_members A function that takes an std::ostream reference as
        an argument and writes some number of bytes into it
        @param[out] out The memory location to write to
        @param[in] size The number of bytes available in the given memory location
        @param[in] compr_mode The desired compression mode
        */
        static std::streamoff Save(
            std::function<void(std::ostream &stream)> save_members,
            SEAL_BYTE *out,
            std::size_t size,
            compr_mode_type compr_mode);

        /**
        Deserializes data from a memory location that was serialized by Save.
        Once the data has been decompressed (depending on compression mode),
        load_members is applied to the decompressed stream. In typical use-cases
        load_members would be a function that deserializes the member variables
        of an object from the given stream.

        @param[in] load_members A function that takes an std::istream reference as
        an argument and reads some number of bytes from it
        @param[in] in The memory location to read from
        @param[in] size The number of bytes available in the given memory location
        */
        static std::streamoff Load(
            std::function<void(std::istream &stream)> load_members,
            const SEAL_BYTE *in,
            std::size_t size);

    private:
        Serialization() = delete;
    };
}
