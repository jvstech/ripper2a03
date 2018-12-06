/*

Copyright 2018-2019 Jonathan Smith

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.

*/

#if defined(_MSC_VER)
// Until fmtlib uses std::invoke_result instead of std::result_of, this needs to
// be here.
#define _SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING
#endif

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "cxxopts.hpp"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

#include "convert_cast.h"
#include "ines_header.h"
#include "mirroring_kind.h"
#include "nes_rom.h"

// namespace injection for ConvertCast specialization
namespace jvs
{

template <>
struct ConvertCast<bool, std::string>
{
  std::string operator()(bool value) const
  {
    return (value
      ? "yes"
      : "no");
  }
};

} // namespace jvs

static std::string hexAddr(std::uint16_t addr)
{
  return fmt::format("{}{:04X}", "0x", addr);
}

template <typename T>
std::string tableRow(const std::string& label, T value)
{
  return fmt::format("{:<20}: {}\n", label, value);
}

int main(int argc, char** argv)
{

  cxxopts::Options options("ripper2A03", 
    "NES ROM file code and data extractor");
  options.add_options()
    ("i,infile", "Input NES ROM file", cxxopts::value<std::string>())
    ("o,outfile", "Output file", cxxopts::value<std::string>())
    ("r,rom", "Extract all ROM banks", cxxopts::value<bool>())
    ("p,prg", "Extract PRG ROM banks", cxxopts::value<bool>())
    ("c,chr", "Extract CHR ROM banks", cxxopts::value<bool>())
    ;
  options.parse_positional({"infile", "outfile"});
  auto args = options.parse(argc, argv);
  if (args.count("infile") == 0)
  {
    std::cerr << "Input NES ROM file not specified.\n"
      << options.help();
    return 1;
  }

  std::string inFileName = args["infile"].as<std::string>();
  std::string outFileName{};
  if (args.count("outfile") == 0)
  {
    outFileName = inFileName + ".prg-rom";
  }
  else
  {
    outFileName = args["outfile"].as<std::string>();
  }

  // TODO: retrieve remaining command-line arguments

  std::ifstream is(inFileName, std::ios::binary);
  if (!is.good())
  {
    std::cerr << "Error loading file " << inFileName << "\n";
    return 1;
  }

  // read the iNES header
  auto romHeaderBuf = jvs::ReadHeader(is);
  if (!romHeaderBuf)
  {
    std::cerr << "Failed to read ROM header: "
      << romHeaderBuf.error().message() << "\n";
    return romHeaderBuf.error().value();
  }

  jvs::INesHeader& romHeader = romHeaderBuf.get();
  if (!romHeader.IsValid())
  {
    std::cerr << "Bad iNES ROM signature.\n";
    return 1;
  }

  std::cout << tableRow("NES version", romHeader.version())
    << tableRow("PRG ROM page count", 
      fmt::format("{} ({} bytes)", romHeader.prg_rom_page_count(), 
        romHeader.prg_rom_byte_count()))
    << tableRow("CHR ROM page count",
      fmt::format("{} ({} bytes)", romHeader.chr_rom_page_count(),
        romHeader.chr_rom_byte_count()))
    << tableRow("Battery", jvs::ConvertTo<std::string>(romHeader.has_battery()))
    << tableRow("Four-screen mode", 
      jvs::ConvertTo<std::string>(romHeader.is_four_screen_mode()))
    << tableRow("Mapper", romHeader.mapper_number())
    << tableRow("Mirroring", jvs::ConvertTo<std::string>(romHeader.mirroring()))
    << tableRow("Playchoice 10", 
      jvs::ConvertTo<std::string>(romHeader.is_playchoice_10()))
    << tableRow("Trainer", jvs::ConvertTo<std::string>(romHeader.has_trainer()))
    << tableRow("VS UniSystem", 
      jvs::ConvertTo<std::string>(romHeader.is_vs_unisystem()));

  auto interruptVectors = jvs::ReadInterruptVectors(is, romHeader);
  if (!interruptVectors)
  {
    std::cerr << "Couldn't read interrupt vectors: "
      << interruptVectors.error().message() << "\n";
    return interruptVectors.error().value();
  }

  std::cout << tableRow("NMI vector", hexAddr(interruptVectors->nmi()))
    << tableRow("Reset vector", hexAddr(interruptVectors->reset()))
    << tableRow("IRQ/BRK vector", hexAddr(interruptVectors->irq()));

  // FIXME: Legacy code follows.

  
  //auto prgBytesToRead = romHeader.prg_rom_byte_count();
  //std::vector<std::uint8_t> prgRomData{};
  //prgRomData.resize(prgBytesToRead, 0);
  //is.read(reinterpret_cast<char*>(&prgRomData[0]), prgRomData.size());
  //if (!is.good())
  //{
  //  std::cerr << "Failed to read all PRG ROM data.\n";
  //  return 1;
  //}

  //is.close();
  //std::ofstream os(outFileName, std::ios::binary);
  //os.write(reinterpret_cast<const char*>(prgRomData.data()), prgBytesToRead);
  //if (!os.good())
  //{
  //  std::cerr << "Failed to write PRG ROM data to " << outFileName << "\n";
  //  return 1;
  //}

  //os.flush();
  //os.close();

  //std::cout << "Write PRG ROM data to " << outFileName << "\n";
  return 0;
}
