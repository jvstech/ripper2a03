//!
//! @file cl_options.h
//!
//! This is meant to be a wrapper around cxxopts with logic similar to that of
//! the LLVM command line library (llvm::cl). At the current point, it's not
//! polished or completely ready for production use, but it works fine for
//! ripper2a03 (for now).
//!

#if !defined(JVS_CL_OPTIONS_H_)
#define JVS_CL_OPTIONS_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "cxxopts.hpp"

namespace jvs
{
namespace cl
{
namespace detail
{

class OptionRegistrar;

class BasicOption
{
public:
  virtual ~BasicOption() = default;

  virtual const std::string& name() const
  {
    return name_;
  }

  virtual const std::string& description() const
  {
    return description_;
  }

  virtual const std::string& group() const
  {
    return group_;
  }

  virtual const std::string& value_description() const
  {
    return value_description_;
  }

  virtual bool positional() const
  {
    return positional_;
  }

protected:
  friend class OptionRegistrar;

  virtual void set_name(const std::string& value)
  {
    name_ = value;
  }

  virtual void set_description(const std::string& value)
  {
    description_ = value;
  }

  virtual void set_group(const std::string& value)
  {
    group_ = value;
  }

  virtual void set_value_description(const std::string& value)
  {
    value_description_ = value;
  }

  virtual void set_positional(bool value = true)
  {
    positional_ = value;
  }

  virtual void AddOption(cxxopts::OptionAdder& optAdder) = 0;
  virtual void SetDefaultValue() = 0;

private:
  std::string name_{};
  std::string description_{};
  std::string group_{};
  std::string value_description_{};
  bool positional_{false};
};

class OptionRegistrar
{
public:
  static bool RegisterOption(BasicOption& option);
  static std::optional<std::pair<std::string, bool>> ParseArguments(
    const std::string& program, const std::string& description, int argc, 
    char** argv);
};

template <typename T>
class OptStorage
{
  static constexpr bool IsRef = std::is_reference_v<T>;
  using RefWrap = std::reference_wrapper<std::remove_reference_t<T>>;

public:
  using type = typename std::conditional_t<IsRef, RefWrap, T>;

  using reference = typename std::remove_reference_t<T>&;
  using const_reference = const typename std::remove_reference_t<T>&;
  using pointer = typename std::remove_reference_t<T>*;
  using const_pointer = const typename std::remove_reference_t<T>*;

public:

  OptStorage() = default;

  OptStorage(reference value)
    : value_(value)
  {
  }
  
  const_reference value() const
  {
    return value_;
  }
  
  reference value()
  {
    return value_;
  }
  
  OptStorage& set_value(const_reference value)
  {
    *ptr() = value;
    return *this;
  }

private:
  type value_;

  const_pointer ptr() const
  {
    if constexpr (IsRef)
    {
      return std::addressof(value_.get());
    }
    else
    {
      return std::addressof(value_);
    }
  }

  pointer ptr()
  {
    if constexpr (IsRef)
    {
      return std::addressof(value_.get());
    }
    else
    {
      return std::addressof(value_);
    }
  }
};


template <typename T>
struct DefaultOptValue
{
  DefaultOptValue(const T& value)
    : value_(value)
  {
  }

  DefaultOptValue(T&& value)
    : value_(std::move(value))
  {
  }

  const T& value() const
  {
    return value_;
  }

private:
  T value_;
};

} // namespace detail


//template <typename T>
//detail::ExternalReference<T> External(T& value)
//{
//  detail::ExternalReference<T> externalRef{value};
//  return externalRef;
//}

[[maybe_unused]]
static detail::DefaultOptValue<std::string> DefaultValue(
  const std::string& value)
{
  detail::DefaultOptValue<std::string> defaultValue{value};
  return defaultValue;
}

[[maybe_unused]]
static detail::DefaultOptValue<std::string> DefaultValue(const char* value)
{
  detail::DefaultOptValue<std::string> defaultValue{value};
  return defaultValue;
}

template <typename T>
detail::DefaultOptValue<T> DefaultValue(T value)
{
  detail::DefaultOptValue<T> defaultValue{value};
  return defaultValue;
}

struct Desc
{
  Desc(const std::string& desc)
    : description_(desc)
  {
  }

  const std::string& description() const
  {
    return description_;
  }

private:
  std::string description_{};
};

struct ValueDesc
{
  ValueDesc(const std::string& desc)
    : description_(desc)
  {
  }

  const std::string& description() const
  {
    return description_;
  }

private:
  std::string description_{};
};

struct Group
{
  Group(const std::string& groupName)
    : group_(groupName)
  {
  }

  const std::string& group() const
  {
    return group_;
  }

private:
  std::string group_{};
};

struct Positional
{
};

template <typename T, bool ExternalStorage = false>
class OptBase : public detail::BasicOption
{
public:
  virtual ~OptBase() = default;

  template <typename... ConstructorArgsT>
  OptBase(const std::string& name, ConstructorArgsT&&... opts)
    : detail::BasicOption()
  {
    set_name(name);
    if constexpr (sizeof...(opts) > 0)
    {
      init(std::forward<ConstructorArgsT>(opts)...);
    }

    detail::OptionRegistrar::RegisterOption(*this);
  }

  const T& value() const
  {
    return storage_.value();
  }

  T& value()
  {
    return storage_.value();
  }

protected:
  void SetDefaultValue() override
  {
    storage_.set_value(default_value_.value());
  }

  void AddOption(cxxopts::OptionAdder& optAdder) override
  {
    optAdder(name(), description(), cxxopts::value(storage_.value()),
      (value_description().empty() ? "" : value_description()));
  }

private:
  detail::OptStorage<T> storage_{};
  detail::OptStorage<T> default_value_{};
  bool positional_{false};

  void init(const Desc& item)
  {
    set_description(item.description());
  }

  void init(const ValueDesc& item)
  {
    set_value_description(item.description());
  }

  void init(const Group& item)
  {
    set_group(item.group());
  }

  void init(const detail::DefaultOptValue<T>& item)
  {
    default_value_.set_value(item.value());
  }

  void init(Positional)
  {
    set_positional();
  }

  template <typename U, typename... ConstructorArgsT>
  void init(const U& item, ConstructorArgsT&&... opts)
  {
    init(item);
    init(std::forward<ConstructorArgsT>(opts)...);
  }

  // I don't really like macros, but this is much faster for defining multiple
  // functions that take the same arguments.
//#define HandleInit(argType) \
//  template <typename... ConstructorArgsT> \
//  void init(argType item, ConstructorArgsT&&... opts) \
//  { \
//    init(item); \
//    if constexpr (sizeof...(opts) > 0) \
//    { \
//      init(std::forward<ConstructorArgsT>(opts)...); \
//    } \
//  } \
//  void init(argType item)

  //HandleInit(const detail::ExternalReference<T>&)
  //{
  //  storage_.set_value(item);
  //}


//#undef HandleInit
};

template <typename T>
class Opt : public OptBase<T>
{
public:
  template <typename... ConstructorArgsT>
  Opt(const std::string& name, ConstructorArgsT&&... opts)
    : OptBase<T>(name, std::forward<ConstructorArgsT>(opts)...)
  {
  }
};

template <>
class Opt<std::string> : public OptBase<std::string>
{
public:
  template <typename... ConstructorArgsT>
  Opt(const std::string& name, ConstructorArgsT&&... opts)
    : OptBase<std::string>(name, std::forward<ConstructorArgsT>(opts)...)
  {
  }

  operator const std::string&() const
  {
    return value();
  }

  operator std::string&()
  {
    return value();
  }
};

template <>
class Opt<bool> : public OptBase<bool>
{
public:
  template <typename... ConstructorArgsT>
  Opt(const std::string& name, ConstructorArgsT&&... opts)
    : OptBase<bool>(name, std::forward<ConstructorArgsT>(opts)...)
  {
  }

  operator bool() const
  {
    return value();
  }
};

std::optional<std::pair<std::string, bool>> ParseArguments(
  const std::string& program, const std::string& description, int argc, 
  char** argv);

} // namespace cl
} // namespace jvs


#endif // !JVS_CL_OPTIONS_H_