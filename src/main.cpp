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

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

#include "cxxopts.hpp"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

#include "cl_options.h"
#include "convert_cast.h"
#include "expected.h"
#include "ines_header.h"
#include "instruction.h"
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

int dumpData(const std::string& outFileName,
  const std::vector<std::uint8_t>& data)
{
  std::ofstream os(outFileName, std::ios::binary);
  if (!os.good())
  {
    std::cerr << "Failed to open output file " << outFileName << "\n";
    return 1;
  }

  os.write(reinterpret_cast<const char*>(data.data()), data.size());
  if (!os.good())
  {
    std::cerr << "Failed to write data to " << outFileName << "\n";
    return 1;
  }

  os.flush();
  os.close();
  return 0;
}

int dumpPrgRomData(const std::string& outFileName, std::ifstream &is, 
  const jvs::INesHeader& romHeader, unsigned int prgPageNum,
  const std::string& inFileName)
{
  std::ofstream os(outFileName, std::ios::binary);
  if (!os.good())
  {
    std::cerr << "Failed to open output file " << outFileName << "\n";
    return 1;
  }

  if (auto writeError = jvs::WritePrgRomData(is, romHeader, prgPageNum, os))
  {
    if (!os.good())
    {
      std::cerr << "Failed to write PRG ROM page " << prgPageNum << " to "
        << outFileName << ":\n" << writeError.message() << "\n";
    }
    else if (!is.good())
    {
      std::cerr << "Failed to read PRG ROM page " << prgPageNum << " from "
        << inFileName << ":\n" << writeError.message() << "\n";
    }
    else if (writeError == std::errc::argument_out_of_domain)
    {
      std::cerr << "Invalid PRG ROM page number: " << prgPageNum << "\n";
    }

    return writeError.value();
  }

  os.flush();
  os.close();
  return 0;
}

int main(int argc, char** argv)
{
  namespace cl = jvs::cl;
  cl::Opt<std::string> inFileName("infile", cl::Desc("Input NES ROM file"),
    cl::Positional());
  cl::Opt<std::string> outFileName("outfile", cl::Desc("Output file"),
    cl::Positional());
  cl::Opt<bool> extractPrg("extract-prg", cl::Desc("Extract PRG ROM pages"),
    cl::Group("Extraction"));
  cl::Opt<bool> extractChr("extract-chr", cl::Desc("Extract CHR ROM pages"),
    cl::Group("Extraction"));
  cl::Opt<std::string> prgExtension("prg-ext",
    cl::Desc("PRG ROM output file extension"), cl::DefaultValue("prg"),
    cl::Group("Extraction"));
  cl::Opt<std::string> chrExtension("chr-ext",
    cl::Desc("CHR ROM output file extension"), cl::DefaultValue("chr"),
    cl::Group("Extraction"));
  cl::Opt<bool> fixedFirstPrg("first-fixed-prg",
    cl::Desc("MMC1 - First PRG ROM page is fixed instead of the last one"),
    cl::Group("Extraction"));
  cl::Opt<bool> jmpReset("jmp-prg",
    cl::Desc("Add code to jump to the reset vector when extracting only "
      "PRG ROM pages"),
    cl::Group("Extraction"));
  // Future options:
  // - emit assembly for PRG ROM pages
  // - provide some analysis pass options (such as identification of bank
  //   switch code)
  auto optParseResult = cl::ParseArguments("ripper2A03",
    "NES ROM file code and data analyzer/extractor", argc, argv);
  if (optParseResult)
  {
    auto[msg, wasError] = *optParseResult;
    std::cerr << msg << "\n";
    if (wasError)
    {
      return 1;
    }

    return 0;
  }

  if (inFileName.value().empty())
  {
    std::cerr << "Input NES ROM file not specified.\n";
    return 1;
  }

  std::string outFilePath{};
  if ((extractPrg || extractChr) && outFileName.value().empty())
  {
    outFilePath = inFileName.value();
  }
  else
  {
    outFilePath = outFileName.value();
  }

  std::ifstream is(inFileName, std::ios::binary);
  if (!is.good())
  {
    std::cerr << "Error loading file " << inFileName.value() << "\n";
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

  jvs::INesHeader& romHeader = *romHeaderBuf;
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

  // TODO: start disassembly and analysis here

  // Perform extraction
  if (extractPrg)
  {
    // I focus most of my efforts on Zelda II which is uses MMC1. I'd like to
    // add logical extraction code for the other mappers, but I just don't have
    // the time right now.
    if (romHeader.mapper_number() != 1)
    {
      if (romHeader.mapper_number() == 0 && romHeader.prg_rom_page_count() > 2)
      {
        std::cerr << "Invalid NES ROM: Mapper specified as NROM (0) but more "
          "than two PRG ROM pages\n"
          "are present.\n";
        return 1;
      }

      for (unsigned int i = 0; i < romHeader.prg_rom_page_count(); i++)
      {
        std::string outName = fmt::format("{}.{}.{}", outFilePath, i,
          prgExtension.value());
        int errCode = dumpPrgRomData(outName, is, romHeader, i,
          inFileName);
        if (errCode)
        {
          return errCode;
        }
      }
    }
    else
    {
      // MMC1 logical extraction based on options
      constexpr std::size_t firstBankOffset = 0x8000;
      constexpr std::size_t secondBankOffset = 0xC000;
      bool fixedLastBank = !fixedFirstPrg;
      std::vector<std::uint8_t> jmpResetCode{};
      if (jmpReset)
      {
        auto resetVector = interruptVectors->reset();
        jmpResetCode.resize(3);
        jmpResetCode[0] = 0x4c;
        std::memcpy(&jmpResetCode[1], &resetVector, 2);
      }

      std::vector<std::uint8_t> firstBank{};
      std::vector<std::uint8_t> secondBank{};
      auto fixedBankPageNum = (fixedLastBank 
        ? romHeader.prg_rom_page_count() - 1
        : 0);
      auto* fixedBank = (fixedLastBank ? &secondBank : &firstBank);
      auto* mutableBank = (fixedLastBank ? &firstBank : &secondBank);
      auto startPageNum = (fixedLastBank ? 0 : 1);
      auto fixedBankOffset = (fixedLastBank
        ? secondBankOffset
        : firstBankOffset);
      auto mutableBankOffset = (fixedLastBank
        ? firstBankOffset
        : secondBankOffset);
      auto lastPageNum = (fixedLastBank
        ? romHeader.prg_rom_page_count() - 2
        : romHeader.prg_rom_page_count() - 1);
      std::string outFileNameFmt = outFilePath + (fixedLastBank 
        ? ".{0}-{1}." : ".{1}-{0}.") + prgExtension.value();
      auto pageRead = jvs::ReadPrgRomPage(is, romHeader, fixedBankPageNum);
      if (!pageRead)
      {
        std::cerr << "Failed to read PRG ROM page " << fixedBankPageNum 
          << " from " << inFileName.value() << "\n";
        return pageRead.error().value();
      }

      *fixedBank = *pageRead;

      for (std::size_t i = startPageNum; i <= lastPageNum; i++)
      {
        // fill bank vector buffer with NOPs
        std::vector<std::uint8_t> prgBankBuffer(
          static_cast<std::size_t>(0x10000),
          static_cast<std::uint8_t>(0xEA));
        // add jmp reset code if requested
        if (!jmpResetCode.empty())
        {
          std::copy(jmpResetCode.begin(), jmpResetCode.end(),
            prgBankBuffer.begin());
        }

        auto mutablePageRead = jvs::ReadPrgRomPage(is, romHeader, i);
        if (!mutablePageRead)
        {
          std::cerr << "Failed to read PRG ROM page " << i << " from "
            << inFileName.value() << "\n";
          return mutablePageRead.error().value();
        }

        *mutableBank = *mutablePageRead;
        std::copy(mutableBank->begin(), mutableBank->end(),
          prgBankBuffer.begin() + mutableBankOffset);
        std::copy(fixedBank->begin(), fixedBank->end(),
          prgBankBuffer.begin() + fixedBankOffset);
        std::string outName = fmt::format(outFileNameFmt,  
          static_cast<int>(i), static_cast<int>(fixedBankPageNum));
        if (auto writeRet = dumpData(outName, prgBankBuffer))
        {
          return writeRet;
        }
      }
    }

    std::cout << "Wrote PRG ROM banks.\n";
  }

  return 0;
}
