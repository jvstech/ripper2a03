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

auto jvs::ReadAllInterruptVectors(std::istream& is, const INesHeader& header)
  -> jvs::Expected<std::vector<jvs::InterruptVectors>>
{
  std::vector<InterruptVectors> ret{};
  ret.reserve(header.prg_rom_page_count());
  for (unsigned int i = 0; i < header.prg_rom_page_count(); i++)
  {
    auto ivBuf = ReadInterruptVectors(is, header, i);
    if (!ivBuf)
    {
      return ivBuf.error();
    }

    ret.push_back(*ivBuf);
  }

  return ret;
}

std::error_code jvs::WritePrgRomData(std::istream& is, 
  const INesHeader& header, unsigned int prgPageNumber, std::ostream& os)
{
  if (!is.good() || !os.good())
  {
    return std::make_error_code(std::errc::io_error);
  }

  auto prgPage = ReadPrgRomPage(is, header, prgPageNumber);
  if (!prgPage)
  {
    return prgPage.error();
  }

  os.write(reinterpret_cast<const char*>(prgPage->data()), prgPage->size());
  if (!os.good())
  {
    return std::make_error_code(std::errc::io_error);
  }

  os.flush();
  return {};
}
