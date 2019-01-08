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
    Archive &operator<<(const T &v)
    {
        *this << const_cast<T &>(v);
        return *this;
    }

    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, Archive>::type &operator<<(T &v)
    {
        BinaryIO(std::addressof(v), sizeof(T));
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
    Archive &operator<<(std::vector<_ElemT, _Allocator> &container)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            container.resize(static_cast<std::size_t>(size));
            if constexpr (std::is_same<_ElemT, bool>::value)
            {
                for (auto &&v : container)
                {
                    bool b;
                    *this << b;
                    v = b;
                }
            }
            else if constexpr (std::is_arithmetic<_ElemT>::value)
            {
                BinaryIO(container.data(), static_cast<std::size_t>(size) * sizeof(_ElemT));
            }
            else
            {
                for (auto &&v : container)
                {
                    *this << v;
                }
            }
        }
        else
        {
            serialize_size_t size = container.size();
            *this << size;
            if constexpr (std::is_same<_ElemT, bool>::value)
            {
                for (auto &&v : container)
                {
                    bool b = static_cast<bool>(v);
                    *this << b;
                }
            }
            else if constexpr (std::is_arithmetic<_ElemT>::value)
            {
                BinaryIO(container.data(), size * sizeof(_ElemT));
            }
            else
            {
                for (auto &&v : container)
                {
                    *this << v;
                }
            }
        }
        return *this;
    }

    template <class _ElemT, std::size_t _Size>
    Archive &operator<<(std::array<_ElemT, _Size> &container)
    {
        if constexpr (Loading)
        {
            if constexpr (std::is_arithmetic<_ElemT>::value)
            {
                BinaryIO(container.data(), sizeof(container));
            }
            else
            {
                for (auto &&v : container)
                {
                    *this << v;
                }
            }
        }
        else
        {
            if constexpr (std::is_arithmetic<_ElemT>::value)
            {
                BinaryIO(container.data(), sizeof(container));
            }
            else
            {
                for (auto &&v : container)
                {
                    *this << v;
                }
            }
        }
        return *this;
    }

    template <class _ElemT, class _Allocator>
    Archive &operator<<(std::list<_ElemT, _Allocator> &container)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            container.resize(static_cast<std::size_t>(size));
            for (auto &&v : container)
            {
                *this << v;
            }
        }
        else
        {
            serialize_size_t size = container.size();
            *this << size;
            for (auto &&v : container)
            {
                *this << v;
            }
        }
        return *this;
    }
    template <class _ElemT, class _Compare, class _Allocator>
    Archive &operator<<(std::set<_ElemT, _Compare, _Allocator> &container)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            container.clear();
            auto hint = container.begin();
            _ElemT v;
            for (serialize_size_t i = 0; i < size; ++i)
            {
                *this << v;
                hint = container.emplace_hint(hint, std::move(v));
            }
        }
        else
        {
            serialize_size_t size = container.size();
            *this << size;
            for (auto &&v : container)
            {
                *this << v;
            }
        }
        return *this;
    }

    template <class _ElemT, class _Hasher, class _Compare, class _Allocator>
    Archive &operator<<(std::unordered_set<_ElemT, _Hasher, _Compare, _Allocator> &container)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            container.clear();
            container.reserve(static_cast<std::size_t>(size));
            _ElemT v;
            for (serialize_size_t i = 0; i < size; ++i)
            {
                *this << v;
                container.emplace(std::move(v));
            }
        }
        else
        {
            serialize_size_t size = container.size();
            *this << size;
            for (auto &&v : container)
            {
                *this << v;
            }
        }
        return *this;
    }

    template <template <typename...> class _MapT, typename... _Args, typename = typename _MapT<_Args...>::mapped_type>
    Archive &operator<<(_MapT<_Args...> &container)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            container.clear();
            auto hint = container.begin();
            typename _MapT<_Args...>::key_type k;
            typename _MapT<_Args...>::mapped_type v;
            for (serialize_size_t i = 0; i < size; ++i)
            {
                *this << k;
                *this << v;
                hint = container.emplace_hint(hint, std::move(k), std::move(v));
            }
        }
        else
        {
            serialize_size_t size = container.size();
            *this << size;
            for (auto &&itr : container)
            {
                *this << itr.first;
                *this << itr.second;
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
