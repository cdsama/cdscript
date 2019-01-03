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
class ArchiveBinaryLoad
{
  public:
    inline static const bool Loading = true;
    inline static const bool Saving = false;

    ArchiveBinaryLoad(std::istream &_is)
        : is(_is)
    {
    }
    void load(void *const data, std::size_t size)
    {
        auto const readSize = static_cast<std::size_t>(is.rdbuf()->sgetn(reinterpret_cast<char *>(data), size));

        if (readSize != size)
        {
            throw Exception("Failed to read ", size, " bytes from input stream! Read ", readSize);
        }
    }

    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, ArchiveBinaryLoad>::type &operator<<(T &v)
    {
        load(std::addressof(v), sizeof(T));
        return *this;
    }

  private:
    std::istream &is;
};

class ArchiveBinarySave
{
  public:
    inline static const bool Loading = false;
    inline static const bool Saving = true;
    ArchiveBinarySave(std::ostream &_os)
        : os(_os)
    {
    }
    void save(const void *data, std::size_t size)
    {
        auto const writtenSize = static_cast<std::size_t>(os.rdbuf()->sputn(reinterpret_cast<const char *>(data), size));
        if (writtenSize != size)
        {
            throw Exception("Failed to write ", size, " bytes to output stream! Wrote ", writtenSize);
        }
    }
    template <typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, ArchiveBinarySave>::type &operator<<(T &v)
    {
        save(std::addressof(v), sizeof(T));
        return *this;
    }

  private:
    std::ostream &os;
};
} // namespace serialize
} // namespace cd
