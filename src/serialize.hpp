// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include <iostream>
#include <cstdint>
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

using serialize_size_t = std::uint64_t;

template <EArchiveType ArchiveType>
class Archive
{
    typename Traits<ArchiveType>::stream_type &stream;

  public:
    inline static const bool Loading = Traits<ArchiveType>::Loading;
    inline static const bool Saving = !Loading;

    Archive(typename Traits<ArchiveType>::stream_type &_stream)
        : stream(_stream)
    {
    }

    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, Archive>::type &operator<<(T &v)
    {
        BinaryIO(std::addressof(v), sizeof(T));
        return *this;
    }

    template <class _Elem, class _Traits, class _Alloc>
    Archive &operator<<(std::basic_string<_Elem, _Traits, _Alloc> &str)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            str.resize(static_cast<std::size_t>(size));
            BinaryIO(const_cast<_Elem *>(str.data()), static_cast<std::size_t>(size) * sizeof(_Elem));
        }
        else
        {
            serialize_size_t size = str.size();
            *this << size;
            BinaryIO(str.data(), size * sizeof(_Elem));
        }
        return *this;
    }

    template <class _Ty, class _Alloc>
    Archive &operator<<([[maybe_unused]] std::vector<_Ty, _Alloc> &vec)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            vec.resize(static_cast<std::size_t>(size));
            if constexpr (std::is_same<_Ty, bool>::value)
            {
                for (auto &&v : vec)
                {
                    bool b;
                    *this << b;
                    v = b;
                }
            }
            else if constexpr (std::is_arithmetic<_Ty>::value)
            {
                BinaryIO(vec.data(), static_cast<std::size_t>(size) * sizeof(_Ty));
            }
            else
            {
                for (auto &&v : vec)
                {
                    *this << v;
                }
            }
        }
        else
        {
            serialize_size_t size = vec.size();
            *this << size;
            if constexpr (std::is_same<_Ty, bool>::value)
            {
                for (auto &&v : vec)
                {
                    bool b = static_cast<bool>(v);
                    *this << b;
                }
            }
            else if constexpr (std::is_arithmetic<_Ty>::value)
            {
                BinaryIO(vec.data(), size * sizeof(_Ty));
            }
            else
            {
                for (auto &&v : vec)
                {
                    *this << v;
                }
            }
        }
        return *this;
    }

  private:
    void BinaryIO(void *const data, std::size_t size)
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
};

} // namespace serialize
} // namespace cd
