#if !defined(JVS_RIPPER2A03_NES_ROM_H_)
#define JVS_RIPPER2A03_NES_ROM_H_

#include <cstdint>
#include <istream>
#include <vector>

#include "expected.h"
#include "ines_header.h"
#include "interrupt_vectors.h"

namespace jvs
{

Expected<INesHeader> ReadHeader(std::istream& is);
Expected<std::vector<std::uint8_t>> ReadPrgRomPage(std::istream& is,
  const INesHeader& header, unsigned int prgPageNumber);

//!
//! @brief
//!   Reads the interrupt vectors from the last PRG ROM page only
//!
Expected<InterruptVectors> ReadInterruptVectors(std::istream& is,
  const INesHeader& header, unsigned int prgPageNumber = ~0U);

} // namespace jvs

#endif // !JVS_RIPPER2A03_NES_ROM_H_
