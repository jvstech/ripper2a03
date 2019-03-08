//!
//! @file fnv1a.h
//!
//! Provides a simple implementation of the Fowler-Noll-Vo 1a hash function
//!

#include <cstddef>
#include <limits>

namespace jvs
{
namespace fnv1a
{

#if defined(__SIZE_WIDTH__) && __SIZE_WIDTH__ == 64
static constexpr std::size_t Prime = 1099511628211; // 2^40 + 2^8 + 0xb3
static constexpr std::size_t Basis = 0xcbf29ce484222325;
#else
// Must assume 32-bit
static constexpr std::size_t Prime = 16777619; // 2^24 + 2^8 + 0x93
static constexpr std::size_t Basis = 0x811c9dc5;
#endif

struct HashCode
{
  std::size_t value() const
  {
    return this->value_;
  }

  HashCode& value(std::size_t& outValue)
  {
    outValue = this->value_;
    return *this;
  }

  HashCode& operator()(std::size_t o)
  {
    value_ ^= o;
    value_ *= Prime;
    return *this;
  }

  operator std::size_t() const
  {
    return value_;
  }
  
private:
  std::size_t value_{Basis};
};

} // namespace fnv1a

using HashCode = jvs::fnv1a::HashCode;

} // namespace jvs