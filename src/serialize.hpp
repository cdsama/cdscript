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
        BinaryIO(std::addressof(const_cast<typename std::remove_cv<T>::type &>(v)), sizeof(T));
        return *this;
    }

    template <class _ElemT, class _Traits, class _Allocator>
    Archive &operator<<(std::basic_string<_ElemT, _Traits, _Allocator> &str)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            str.resize(static_cast<std::size_t>(size));
            BinaryIO(const_cast<_ElemT *>(str.data()), static_cast<std::size_t>(size) * sizeof(_ElemT));
        }
        else
        {
            serialize_size_t size = str.size();
            *this << size;
            BinaryIO(str.data(), size * sizeof(_ElemT));
        }
        return *this;
    }

    template <class _ElemT, class _Allocator>
    Archive &operator<<(std::vector<_ElemT, _Allocator> &vec)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            vec.resize(static_cast<std::size_t>(size));
            if constexpr (std::is_same<_ElemT, bool>::value)
            {
                for (auto &&v : vec)
                {
                    bool b;
                    *this << b;
                    v = b;
                }
            }
            else if constexpr (std::is_arithmetic<_ElemT>::value)
            {
                BinaryIO(vec.data(), static_cast<std::size_t>(size) * sizeof(_ElemT));
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
            if constexpr (std::is_same<_ElemT, bool>::value)
            {
                for (auto &&v : vec)
                {
                    bool b = static_cast<bool>(v);
                    *this << b;
                }
            }
            else if constexpr (std::is_arithmetic<_ElemT>::value)
            {
                BinaryIO(vec.data(), size * sizeof(_ElemT));
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

    template <class _ElemT, std::size_t _Size>
    Archive &operator<<(std::array<_ElemT, _Size> &arr)
    {
        if constexpr (Loading)
        {
            if constexpr (std::is_arithmetic<_ElemT>::value)
            {
                BinaryIO(arr.data(), sizeof(arr));
            }
            else
            {
                for (auto &&v : arr)
                {
                    *this << v;
                }
            }
        }
        else
        {
            if constexpr (std::is_arithmetic<_ElemT>::value)
            {
                BinaryIO(arr.data(), sizeof(arr));
            }
            else
            {
                for (auto &&v : arr)
                {
                    *this << v;
                }
            }
        }
        return *this;
    }

    template <class _ElemT, class _Allocator>
    Archive &operator<<(std::list<_ElemT, _Allocator> &li)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            li.resize(static_cast<std::size_t>(size));
            for (auto &&v : li)
            {
                *this << v;
            }
        }
        else
        {
            serialize_size_t size = li.size();
            *this << size;
            for (auto &&v : li)
            {
                *this << v;
            }
        }
        return *this;
    }
    template <class _ElemT, class _Compare, class _Allocator>
    Archive &operator<<(std::set<_ElemT, _Compare, _Allocator> &s)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            s.clear();
            auto hint = s.begin();
            _ElemT v;
            for (serialize_size_t i = 0; i < size; i++)
            {
                *this << v;
                hint = s.emplace_hint(hint, std::move(v));
            }
        }
        else
        {
            serialize_size_t size = s.size();
            *this << size;
            for (auto &&v : s)
            {
                *this << v;
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
