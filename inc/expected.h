//!
//! @file expected.h
//!
//! @brief
//!   A shameless clone of llvm::ErrorOr
//!

// This file is a very minimally changed derivative of the 
// llvm/Support/ErrorOr.h header. As such, I'm including the LLVM release
// license here:

/*

==============================================================================
LLVM Release License
==============================================================================
University of Illinois/NCSA
Open Source License

Copyright (c) 2003-2018 University of Illinois at Urbana-Champaign.
All rights reserved.

Developed by:

    LLVM Team

    University of Illinois at Urbana-Champaign

    http://llvm.org

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal with
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimers.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimers in the
      documentation and/or other materials provided with the distribution.

    * Neither the names of the LLVM Team, University of Illinois at
      Urbana-Champaign, nor the names of its contributors may be used to
      endorse or promote products derived from this Software without specific
      prior written permission.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
SOFTWARE.

*/

#if !defined(JVS_EXPECTED_H_)
#define JVS_EXPECTED_H_


#include <cassert>
#include <functional>
#include <system_error>
#include <type_traits>
#include <utility>

namespace jvs
{

/// \brief Represents either an error or a value T.
///
/// Expected<T> is a pointer-like class that represents the result of an
/// operation. The result is either an error, or a value of type T. This is
/// designed to emulate the usage of returning a pointer where nullptr indicates
/// failure. However instead of just knowing that the operation failed, we also
/// have an error_code and optional user data that describes why it failed.
///
/// It is used like the following.
/// \code
///   Expected<Buffer> getBuffer();
///
///   auto buffer = getBuffer();
///   if (error_code ec = buffer.error())
///     return ec;
///   buffer->write("adena");
/// \endcode
///
///
/// Implicit conversion to bool returns true if there is a usable value. The
/// unary * and -> operators provide pointer like access to the value. Accessing
/// the value when there is an error has undefined behavior.
///
/// When T is a reference type the behavior is slightly different. The reference
/// is held in a std::reference_wrapper<std::remove_reference<T>::type>, and
/// there is special handling to make operator -> work as if T was not a
/// reference.
///
/// T cannot be a rvalue reference.
template <typename T>
class Expected
{
  template <typename OtherT>
  friend class Expected;

  static const bool isRef = std::is_reference<T>::value;

  using wrap = std::reference_wrapper<std::remove_reference_t<T>>;

  template <typename U>
  using AlignedStorage = std::aligned_storage_t<sizeof(U), alignof(U)>;

public:
  using storage_type = typename std::conditional<isRef, wrap, T>::type;

private:
  using reference = typename std::remove_reference<T>::type &;
  using const_reference = const typename std::remove_reference<T>::type &;
  using pointer = typename std::remove_reference<T>::type *;
  using const_pointer = const typename std::remove_reference<T>::type *;

public:
  template <typename E>
  Expected(E errorCode,
    std::enable_if_t<std::is_error_code_enum_v<E> ||
    std::is_error_condition_enum_v<E>,
    void*> = nullptr)
    : has_error_(true)
  {
    new (error_storage()) std::error_code(make_error_code(errorCode));
  }

  Expected(std::error_code ec) 
    : has_error_(true)
  {
    new (error_storage()) std::error_code(ec);
  }

  template <typename OtherT>
  Expected(OtherT&& val,
    std::enable_if_t<std::is_convertible_v<OtherT, T>>
    * = nullptr)
    : has_error_(false)
  {
    new (storage()) storage_type(std::forward<OtherT>(val));
  }

  Expected(const Expected& other)
  {
    copyConstruct(other);
  }

  template <typename OtherT>
  Expected(const Expected<OtherT>& other,
    std::enable_if_t<std::is_convertible_v<OtherT, T>>* = nullptr)
  {
    copyConstruct(other);
  }

  template <typename OtherT>
  explicit Expected(const Expected<OtherT>& other,
    std::enable_if_t<!std::is_convertible_v<OtherT, const T&>>* = nullptr)
  {
    copyConstruct(other);
  }

  Expected(Expected&& other)
  {
    moveConstruct(std::move(other));
  }

  template <typename OtherT>
  Expected(Expected<OtherT>&& other,
    std::enable_if_t<std::is_convertible_v<OtherT, T>>* = nullptr)
  {
    moveConstruct(std::move(other));
  }

  // This might eventually need SFINAE but it's more complex than is_convertible
  // & I'm too lazy to write it right now.
  template <typename OtherT>
  explicit Expected(Expected<OtherT>&& other,
    std::enable_if_t<!std::is_convertible_v<OtherT, T>>* = nullptr)
  {
    moveConstruct(std::move(other));
  }

  Expected& operator=(const Expected &other)
  {
    copyAssign(other);
    return *this;
  }

  Expected& operator=(Expected&& other)
  {
    moveAssign(std::move(other));
    return *this;
  }

  ~Expected()
  {
    if (!has_error_)
    {
      storage()->~storage_type();
    }
  }

  //!
  //! @brief
  //!   Return false if there is an error.
  //!
  explicit operator bool() const
  {
    return !has_error_;
  }

  reference get()
  {
    return *storage();
  }

  const_reference get() const
  {
    return const_cast<Expected<T>*>(this)->get();
  }

  std::error_code error() const
  {
    return has_error_ ? *error_storage() : std::error_code();
  }

  pointer operator->()
  {
    return toPointer(storage());
  }

  const_pointer operator->() const
  {
    return toPointer(storage());
  }

  reference operator*()
  {
    return *storage();
  }

  const_reference operator*() const
  {
    return *storage();
  }

private:
  template <typename OtherT>
  void copyConstruct(const Expected<OtherT>& other)
  {
    if (!other.has_error_)
    {
      // Get the other value.
      has_error_ = false;
      new (storage()) storage_type(*other.storage());
    }
    else
    {
      // Get other's error.
      has_error_ = true;
      new (error_storage()) std::error_code(other.error());
    }
  }

  template <typename T1>
  static bool compareThisIfSameType(const T1& a, const T1& b)
  {
    return &a == &b;
  }

  template <typename T1, class T2>
  static bool compareThisIfSameType(const T1& a, const T2& b)
  {
    return false;
  }

  template <typename OtherT>
  void copyAssign(const Expected<OtherT>& other)
  {
    if (compareThisIfSameType(*this, other))
    {
      return;
    }

    this->~Expected();
    new (this) Expected(other);
  }

  template <typename OtherT>
  void moveConstruct(Expected<OtherT>&& other)
  {
    if (!other.has_error_)
    {
      // Get the other value.
      has_error_ = false;
      new (storage()) storage_type(std::move(*other.storage()));
    }
    else
    {
      // Get other's error.
      has_error_ = true;
      new (error_storage()) std::error_code(other.error());
    }
  }

  template <typename OtherT>
  void moveAssign(Expected<OtherT>&& other)
  {
    if (compareThisIfSameType(*this, other))
    {
      return;
    }

    this->~Expected();
    new (this) Expected(std::move(other));
  }

  pointer toPointer(pointer val)
  {
    return val;
  }

  const_pointer toPointer(const_pointer val) const
  {
    return val;
  }

  pointer toPointer(wrap* val)
  {
    return &val->get();
  }

  const_pointer toPointer(const wrap* val) const
  {
    return &val->get();
  }

  storage_type* storage()
  {
    assert(!has_error_ && "Cannot get value when an error exists!");
    return reinterpret_cast<storage_type*>(&storage_);
  }

  const storage_type* storage() const
  {
    assert(!has_error_ && "Cannot get value when an error exists!");
    return reinterpret_cast<const storage_type*>(&storage_);
  }

  std::error_code* error_storage()
  {
    assert(has_error_ && "Cannot get error when a value exists!");
    return reinterpret_cast<std::error_code*>(&error_storage_);
  }

  const std::error_code* error_storage() const
  {
    return const_cast<Expected<T> *>(this)->error_storage();
  }

  union
  {
    AlignedStorage<storage_type> storage_;
    AlignedStorage<std::error_code> error_storage_;
  };

  bool has_error_ : 1;
};

template <typename T, class E>
auto operator==(const Expected<T>& err, E code)
  -> std::enable_if_t<std::is_error_code_enum_v<E> ||
    std::is_error_condition_enum_v<E>, bool>
{
  return err.error() == code;
}

} // namespace jvs

#endif // !JVS_EXPECTED_H_
