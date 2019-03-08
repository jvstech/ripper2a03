#if !defined(JVS_RIPPER2A03_INTERNAL_RAM_ADDRESS_LINE_H_)
#define JVS_RIPPER2A03_INTERNAL_RAM_ADDRESS_LINE_H_

#include <cstddef>
#include <cstdint>
#include <functional>

#include "address_line.h"

namespace jvs
{

class InternalRAMAddressLine final : public AddressLine
{
public:
  static constexpr std::uint16_t InternalRAMSize = 0x0800; // 2 KB page
  static constexpr std::size_t EffectiveRAMSize = 0x2000;  // 8 KB of mapping

  InternalRAMAddressLine(std::vector<std::uint8_t>& buffer, 
    std::uint16_t address)
    : AddressLine(address),
    buffer_(buffer)
  {
  }

  std::uint8_t Peek() const
  {
    return value();
  }

  std::uint8_t Peek() override
  {
    return value();
  }

  void Poke(std::uint8_t val) override
  {
    value() = val;
  }

  InternalRAMAddressLine& operator=(std::uint8_t val)
  {
    Poke(val);
    return *this;
  }

private:
  std::reference_wrapper<std::vector<std::uint8_t>> buffer_;

  std::uint8_t value() const
  {
    return buffer_.get()[this->address() % InternalRAMSize];
  }
  
  std::uint8_t& value()
  {
    return buffer_.get()[this->address() % InternalRAMSize];
  }
};

std::vector<InternalRAMAddressLine> CreateInternalRAMMapping(
  std::vector<std::uint8_t>& buffer);

} // namespace jvs

#endif // !JVS_RIPPER2A03_INTERNAL_RAM_ADDRESS_LINE_H_