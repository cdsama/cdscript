// Copyright (c) 2019 chendi
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once
#include <cstdint>
#include <iostream>
#include <typeindex>
#include <unordered_map>
#include "utils.hpp"
namespace cd::serialize
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

template <typename T>
class Constructor
{
    std::unique_ptr<T> t;

  public:
    template <typename... Args>
    void operator()(Args... args)
    {
        t = std::make_unique<T>(args...);
    }

    T *operator->()
    {
        return t.get();
    }

    T &operator*()
    {
        return *t;
    }

    template <typename D = std::default_delete<T>>
    std::unique_ptr<T, D> get_unique(D &&d = std::default_delete<T>())
    {
        std::unique_ptr<T, D> res(t.release(), d);
        return res;
    }
    std::shared_ptr<T> get_shared()
    {
        std::shared_ptr<T> res(t.release());
        return res;
    }
};

class RTTI
{
    RTTI() {}
    RTTI(RTTI const &) = delete;
    RTTI operator=(RTTI const &) = delete;

  public:
    std::unordered_map<std::type_index, std::string> NameMap;
    std::unordered_map<std::type_index, std::function<void(void *, void *)>> SaverMap;
    std::unordered_map<std::string, std::function<void(void *, void *)>> LoaderMap;

    static RTTI &Instance()
    {
        static RTTI ins;
        return ins;
    }

    template <typename Archive>
    void Save(Archive *ar, const std::type_index &index, void *v)
    {
        auto itr = SaverMap.find(index);
        *ar << NameMap[index];
        itr->second(ar, v);
    }
    template <typename Archive>
    void Load(Archive *ar, const std::string &type, void *v)
    {
        auto itr = LoaderMap.find(type);
        itr->second(ar, v);
    }
};

#define REGIST_TYPE(__TYPE_NAME__) REGIST_TYPE_WITH_ALIAS_NAME(__TYPE_NAME__, __TYPE_NAME__)

#define REGIST_TYPE_WITH_ALIAS_NAME(__TYPE_NAME__, __ALIAS_NAME__)              \
    template <>                                                                 \
    struct cd::serialize::TypeName<__TYPE_NAME__>                               \
    {                                                                           \
        constexpr static const char *name = #__ALIAS_NAME__;                    \
    };                                                                          \
    struct RegClass##__TYPE_NAME__##__ALIAS_NAME__                              \
    {                                                                           \
        RegClass##__TYPE_NAME__##__ALIAS_NAME__()                               \
        {                                                                       \
            cd::serialize::TypeSerializerRegister<__TYPE_NAME__>::BindName();   \
            cd::serialize::TypeSerializerRegister<__TYPE_NAME__>::BindSaver();  \
            cd::serialize::TypeSerializerRegister<__TYPE_NAME__>::BindLoader(); \
        }                                                                       \
    };                                                                          \
    RegClass##__TYPE_NAME__##__ALIAS_NAME__ Ins##__TYPE_NAME__##__ALIAS_NAME__

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

    template <typename T>
    typename std::enable_if<std::is_pointer<T>::value, Archive>::type &operator<<(T &)
    {
        static_assert(!std::is_pointer<T>::value, "serializer does not support serializing raw pointers, should use a smart pointer");
        return *this;
    }

    template <typename _ElemT, typename _Traits, typename _Allocator>
    Archive &operator<<(std::basic_string<_ElemT, _Traits, _Allocator> &str)
    {
        if constexpr (Loading)
        {
            serialize_size_t size;
            *this << size;
            str.resize(static_cast<std::size_t>(size));
            BinaryIO(str.data(), static_cast<std::size_t>(size) * sizeof(_ElemT));
        }
        else
        {
            serialize_size_t size = str.size();
            *this << size;
            BinaryIO(str.data(), size * sizeof(_ElemT));
        }
        return *this;
    }

    template <typename _ElemT, typename _Allocator>
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

    template <typename _ElemT, std::size_t _Size>
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

    template <typename _ElemT, typename _Allocator>
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
    template <typename _ElemT, typename _Compare, typename _Allocator>
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
                *this << const_cast<_ElemT &>(v);
            }
        }
        return *this;
    }

    template <typename _ElemT, typename _Hasher, typename _Compare, typename _Allocator>
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
                *this << const_cast<_ElemT &>(v);
            }
        }
        return *this;
    }

    template <typename _ElemT, typename _Allocator>
    Archive &operator<<(std::deque<_ElemT, _Allocator> &container)
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

    template <template <typename...> typename _MapT, typename... _Args, typename = typename _MapT<_Args...>::mapped_type>
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
                *this << const_cast<typename _MapT<_Args...>::key_type &>(itr.first);
                *this << itr.second;
            }
        }
        return *this;
    }

    template <typename _ElemT, typename _Container>
    struct StackContainerHelper : public std::stack<_ElemT, _Container>
    {
        _Container &GetContainer()
        {
            return this->c;
        }
    };

    template <typename _ElemT, typename _Container>
    Archive &operator<<(std::stack<_ElemT, _Container> &container)
    {
        *this << reinterpret_cast<StackContainerHelper<_ElemT, _Container> *>(&container)->GetContainer();
        return *this;
    }

    template <typename T, typename Deleter>
    Archive &operator<<(std::unique_ptr<T, Deleter> &ptr)
    {
        if (Loading)
        {
            ptr.reset();
            bool valid;
            *this << valid;
            if (valid)
            {
                Constructor<T> constructor;
                if constexpr (std::is_polymorphic<T>::value)
                {
                    std::string TypeNameString;
                    *this << TypeNameString;
                    RTTI::Instance().Load(this, TypeNameString, &constructor);
                    ptr = constructor.get_unique();
                }
                else
                {
                    if constexpr (std::is_default_constructible<T>::value)
                    {
                        constructor();
                        ptr = constructor.get_unique();
                        *this << *ptr.get();
                    }
                    else
                    {
                        *this << constructor;
                        ptr = constructor.get_unique();
                    }
                }
            }
        }
        else
        {
            bool valid = (ptr.get() != nullptr);
            *this << valid;
            if (valid)
            {
                if constexpr (std::is_polymorphic<T>::value)
                {
                    const auto &RealType = std::type_index(typeid(*(ptr.get())));
                    RTTI::Instance().Save(this, RealType, ptr.get());
                }
                else
                {
                    *this << *ptr.get();
                }
            }
        }
        return *this;
    }

  private:
    void BinaryIO(const void *data, std::size_t size)
    {
        if constexpr (Loading)
        {
            auto const readSize = static_cast<std::size_t>(stream.rdbuf()->sgetn(reinterpret_cast<char *>(const_cast<void *>(data)), size));
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

template <typename T>
struct TypeName
{
};

template <typename T>
struct TypeSerializerRegister
{
    static void BindName()
    {
        auto &NameMap = ::cd::serialize::RTTI::Instance().NameMap;
        auto TypeIndex = std::type_index(typeid(T));
        NameMap[TypeIndex] = TypeName<T>::name;
    }

    static void BindSaver()
    {
        auto &SaverMap = ::cd::serialize::RTTI::Instance().SaverMap;
        auto TypeIndex = std::type_index(typeid(T));
        SaverMap[TypeIndex] = [](void *ar, void *ptr) {
            Archive<Writer> *arT = static_cast<Archive<Writer> *>(ar);
            T *ptrT = static_cast<T *>(ptr);
            *arT << *ptrT;
        };
    }

    template <typename T>
    static void LoaderFunction(void *ar, void *ptr)
    {
        Archive<Reader> *arT = static_cast<Archive<Reader> *>(ar);
        Constructor<T> &constructor = (*(static_cast<Constructor<T> *>(ptr)));
        if constexpr (std::is_default_constructible<T>::value || std::is_trivially_default_constructible<T>::value)
        {
            constructor();
            *arT << *constructor;
        }
        else
        {
            *arT << constructor;
        }
    }

    static void BindLoader()
    {
        auto &LoaderMap = ::cd::serialize::RTTI::Instance().LoaderMap;
        LoaderMap[TypeName<T>::name] = [](void *ar, void *ptr) {
            LoaderFunction<T>(ar, ptr);
        };
    }
};

}  // namespace cd::serialize
