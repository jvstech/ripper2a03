#include "nes_rom.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <istream>
#include <string>
#include <system_error>
#include <vector>

#include "expected.h"
#include "ines_header.h"
#include "interrupt_vectors.h"


jvs::Expected<jvs::NesRom> jvs::NesRom::FromFile(const std::string& fileName)
{
  namespace fs = std::filesystem;
  fs::path filePath = fileName;
  if (!fs::exists(filePath))
  {
    return std::make_error_code(std::errc::no_such_file_or_directory);
  }

  NesRom rom{};
  std::unique_ptr<std::istream> fileStream{new std::ifstream(filePath, 
    std::ios::binary)};
  if (!fileStream->good())
  {
    return std::make_error_code(std::errc::io_error);
  }

  rom.owned_input_stream_ = std::move(fileStream);
  auto ec = rom.init(rom.owned_input_stream_.get());
  if (ec)
  {
    return ec;
  }

  return rom;
}

jvs::Expected<jvs::NesRom> jvs::NesRom::FromStream(std::istream& is)
{
  NesRom rom{};
  auto ec = rom.init(std::addressof(is));
  if (ec)
  {
    return ec;
  }

  return rom;
}

std::error_code jvs::NesRom::init(std::istream* is)
{
  input_stream_ = is;
  if (!input_stream_)
  {
    return std::make_error_code(std::errc::no_stream_resources);
  }

  auto hdr = ReadHeader(input_stream());
  if (!hdr)
  {
    return hdr.error();
  }
  
  if (!hdr->IsValid())
  {
    return std::make_error_code(std::errc::illegal_byte_sequence);
  }

  header_ = std::move(*hdr);

  auto interruptVectors = ReadInterruptVectors(input_stream(), header_);
  if (!interruptVectors)
  {
    return interruptVectors.error();
  }

  interrupt_vectors_ = std::move(*interruptVectors);
  return {};
}

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
