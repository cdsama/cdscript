// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include <iostream>
#include "utils.hpp"
namespace cd
{
namespace serialize
{
enum EArchiveType
{
    Reader,
    Writer
};

template <EArchiveType ArchiveType>
struct Traits
{
};

template <>
struct Traits<Reader>
{
    inline static const bool Loading = true;
    typedef std::istream stream_type;
};

template <>
struct Traits<Writer>
{
    inline static const bool Loading = false;
    typedef std::ostream stream_type;
};

template <EArchiveType ArchiveType>
class Archive
{
  public:
    inline static const bool Loading = Traits<ArchiveType>::Loading;
    inline static const bool Saving = !Loading;

    Archive(typename Traits<ArchiveType>::stream_type &_stream)
        : stream(_stream)
    {
    }

    void Binary(void *const data, std::size_t size)
    {
        if constexpr (Loading)
        {
            auto const readSize = static_cast<std::size_t>(stream.rdbuf()->sgetn(reinterpret_cast<char *>(data), size));

            if (readSize != size)
            {
                throw Exception("Failed to read ", size, " bytes from input stream! Read ", readSize);
            }
        }
        else
        {
            auto const writtenSize = static_cast<std::size_t>(stream.rdbuf()->sputn(reinterpret_cast<const char *>(data), size));
            if (writtenSize != size)
            {
                throw Exception("Failed to write ", size, " bytes to output stream! Wrote ", writtenSize);
            }
        }
    }

    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, Archive>::type &operator<<(T &v)
    {
        Binary(std::addressof(v), sizeof(T));
        return *this;
    }

  private:
    typename Traits<ArchiveType>::stream_type &stream;
};

} // namespace serialize
} // namespace cd
