#if !defined(JVS_RIPPER2A03_MIRROED_ADDRESS_LINE_H_)
#define JVS_RIPPER2A03_MIRROED_ADDRESS_LINE_H_

#include <functional>
#include <vector>

#include "address_line.h"

namespace jvs
{

class MirroredAddressLine : public AddressLine
{
public:
  virtual ~MirroredAddressLine() = default;

  MirroredAddressLine(std::uint16_t address, std::vector<AddressLine*>& mirror)
    : AddressLine(address),
    mirror_(mirror)
  {
  }

  virtual std::uint8_t Peek() override
  {
    return line().Peek();
  }

  virtual void Poke(std::uint8_t value) override
  {
    line().Poke(value);
  }

private:
  // TODO: determine if this is the best way to handle address lines (as opposed
  // to a vector of unique_ptr<AddressLine>)
  std::reference_wrapper<std::vector<AddressLine*>> mirror_;

  std::vector<AddressLine*>& mirror()
  {
    return mirror_.get();
  }

  AddressLine& mirror(std::size_t index)
  {
    return *mirror()[index];
  }

  AddressLine& line()
  {
    return mirror((this->address() % mirror().size()) - 
      mirror().front()->address());
  }
};

std::vector<AddressLine*> CreateMirroredAddressLines(std::uint16_t startAddress,
  std::vector<AddressLine*>& linesToMirror);

} // namespace jvs

#endif // !JVS_RIPPER2A03_MIRROED_ADDRESS_LINE_H_