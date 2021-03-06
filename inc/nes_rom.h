#if !defined(JVS_RIPPER2A03_NES_ROM_H_)
#define JVS_RIPPER2A03_NES_ROM_H_

#include <cstdint>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <system_error>
#include <vector>

#include "expected.h"
#include "ines_header.h"
#include "interrupt_vectors.h"

namespace jvs
{

class NesRom
{
public:
  const INesHeader& header() const
  {
    return header_;
  }

  const InterruptVectors& interrupt_vectors() const
  {
    return interrupt_vectors_;
  }

  static Expected<NesRom> FromFile(const std::string& fileName);

  static Expected<NesRom> FromStream(std::istream& is);

private:
  std::istream* input_stream_{nullptr};
  std::unique_ptr<std::istream> owned_input_stream_{};
  INesHeader header_{};
  InterruptVectors interrupt_vectors_{};

  std::istream& input_stream()
  {
    return *input_stream_;
  }

  std::error_code init(std::istream* is);
};

Expected<INesHeader> ReadHeader(std::istream& is);

Expected<std::vector<std::uint8_t>> ReadPrgRomPage(std::istream& is,
  const INesHeader& header, unsigned int prgPageNumber);

//!
//! @brief
//!   Reads the interrupt vectors from the given PRG ROM page
//!
Expected<InterruptVectors> ReadInterruptVectors(std::istream& is,
  const INesHeader& header, unsigned int prgPageNumber = ~0U);

//!
//! @brief
//!   Reads the interrupt vectors from all PRG ROM pages
//!
Expected<std::vector<InterruptVectors>> ReadAllInterruptVectors(
  std::istream& is, const INesHeader& header);

std::error_code WritePrgRomData(std::istream& is, const INesHeader& header,
  unsigned int prgPageNumber, std::ostream& os);

} // namespace jvs

#endif // !JVS_RIPPER2A03_NES_ROM_H_
