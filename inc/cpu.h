#if !defined(JVS_RIPPER2A03_CPU_H_)
#define JVS_RIPPER2A03_CPU_H_

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "address_line.h"
#include "internal_ram_address_line.h"
#include "registers.h"
#include "system.h"

namespace jvs
{

class CPU
{
public:

  CPU(System& system)
    : system_(system)
  {
  }
  
  Registers& registers() const
  {
    return this->registers_;
  }

private:
  std::reference_wrapper<System> system_;
  Registers registers_{};
  // TODO: determine if this is the best way to handle address lines (as opposed
  // to a vector of unique_ptr<AddressLine>)
  std::vector<AddressLine*> memory_map_{};
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_CPU_H_
