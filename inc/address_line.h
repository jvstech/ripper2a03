#if !defined(JVS_RIPPER2A03_ADDRESS_LINE_H_)
#define JVS_RIPPER2A03_ADDRESS_LINE_H_

#include <cstdint>

namespace jvs
{

class AddressLine
{
public:

  virtual ~AddressLine() = default;

  std::uint16_t address() const
  {
    return address_;
  }

  operator std::uint8_t()
  {
    return Peek();
  }

  virtual std::uint8_t Peek() = 0;
  
  virtual void Poke(std::uint8_t value) = 0;

protected:
  AddressLine(std::uint16_t address)
    : address_(address)
  {
  }

private:
  std::uint16_t address_;
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_ADDRESS_LINE_H_