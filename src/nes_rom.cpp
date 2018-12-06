#include "nes_rom.h"

#include <cstdint>
#include <istream>
#include <string>
#include <system_error>
#include <vector>

#include "expected.h"
#include "ines_header.h"
#include "interrupt_vectors.h"


jvs::Expected<jvs::INesHeader> jvs::ReadHeader(std::istream& is)
{
  INesHeader header{};
  is.read(reinterpret_cast<char*>(&header), sizeof(header));
  if (is.good())
  {
    return header;
  }
  
  return std::make_error_code(std::errc::io_error);
}

jvs::Expected<std::vector<std::uint8_t>> jvs::ReadPrgRomPage(std::istream& is, 
  const INesHeader& header, unsigned int prgPageNumber)
{
  if (prgPageNumber >= header.prg_rom_page_count())
  {
    return std::make_error_code(std::errc::argument_out_of_domain);
  }

  auto offset = header.GetPrgRomPageOffset(prgPageNumber);
  is.seekg(offset, std::ios::beg);
  if (!is.good())
  {
    return std::make_error_code(std::errc::io_error);
  }

  std::vector<std::uint8_t> prgData(PrgRomPageSize);
  is.read(reinterpret_cast<char*>(&prgData[0]), PrgRomPageSize);
  if (!is.good())
  {
    return std::make_error_code(std::errc::io_error);
  }

  return prgData;
}

jvs::Expected<jvs::InterruptVectors> jvs::ReadInterruptVectors(std::istream& is,
  const INesHeader& header, unsigned int prgPageNumber)
{
  if (prgPageNumber == ~0U)
  {
    prgPageNumber = static_cast<unsigned int>(header.prg_rom_page_count() - 1);
  }

  if (prgPageNumber > header.prg_rom_page_count())
  {
    // requested page number is too high
    return std::make_error_code(std::errc::argument_out_of_domain);
  }

  auto prgPage = ReadPrgRomPage(is, header, prgPageNumber);
  if (!prgPage)
  {
    // couldn't read the page
    return prgPage.error();
  }

  // The interrupt vectors are the last 6 bytes of the last PRG ROM bank, but
  // they can usually be found in the last 6 bytes of *all* the PRG ROM banks,
  // too.
  InterruptVectors ret{};
  const std::uint16_t* vectors = reinterpret_cast<const std::uint16_t*>(
    prgPage->data() + (prgPage->size() - 6));
  std::memcpy(&ret, vectors, 6);
  return ret;
}
