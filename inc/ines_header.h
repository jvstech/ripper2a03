#if !defined(JVS_RIPPER2A03_INES_HEADER_H_)
#define JVS_RIPPER2A03_INES_HEADER_H_

#include <cstdint>

#include "mirroring_kind.h"

namespace jvs
{

constexpr int PrgRomPageSize = 16384;   // 16 KB
constexpr int ChrRomPageSize = 8192;    // 8 KB

struct INesHeader final
{
  unsigned int prg_rom_page_count() const
  {
    return this->prg_rom_page_count_;
  }

  INesHeader& prg_rom_page_count(unsigned int& value)
  {
    value = this->prg_rom_page_count_;
    return *this;
  }

  INesHeader& set_prg_rom_page_count(unsigned int value)
  {
    this->prg_rom_page_count_ = value;
    return *this;
  }

  unsigned int chr_rom_page_count() const
  {
    return this->chr_rom_page_count_;
  }

  INesHeader& chr_rom_page_count(unsigned int& value)
  {
    value = this->chr_rom_page_count_;
    return *this;
  }

  INesHeader& set_chr_rom_page_count(std::uint8_t value)
  {
    this->chr_rom_page_count_ = value;
    return *this;
  }

  std::uint8_t flags_6() const
  {
    return this->flags_6_;
  }

  INesHeader& flags_6(std::uint8_t& value)
  {
    value = this->flags_6_;
    return *this;
  }

  INesHeader& set_flags_6(std::uint8_t value)
  {
    this->flags_6_ = value;
    return *this;
  }

  std::uint8_t flags_7() const
  {
    return this->flags_7_;
  }

  INesHeader& flags_7(std::uint8_t& value)
  {
    value = this->flags_7_;
    return *this;
  }

  INesHeader& set_flags_7(std::uint8_t value)
  {
    this->flags_7_ = value;
    return *this;
  }

  std::uint8_t flags_8() const
  {
    return this->flags_8_;
  }

  std::uint8_t& flags_8()
  {
    return this->flags_8_;
  }

  INesHeader& flags_8(std::uint8_t& value)
  {
    value = this->flags_8_;
    return *this;
  }

  INesHeader& set_flags_8(std::uint8_t value)
  {
    this->flags_8_ = value;
    return *this;
  }

  std::uint8_t flags_9() const
  {
    return this->flags_9_;
  }

  std::uint8_t& flags_9()
  {
    return this->flags_9_;
  }

  INesHeader& flags_9(std::uint8_t& value)
  {
    value = this->flags_9_;
    return *this;
  }

  INesHeader& set_flags_9(std::uint8_t value)
  {
    this->flags_9_ = value;
    return *this;
  }

  std::uint8_t flags_10() const
  {
    return this->flags_10_;
  }

  std::uint8_t& flags_10()
  {
    return this->flags_10_;
  }

  INesHeader& flags_10(std::uint8_t& value)
  {
    value = this->flags_10_;
    return *this;
  }

  INesHeader& set_flags_10(std::uint8_t value)
  {
    this->flags_10_ = value;
    return *this;
  }

  std::uint8_t flags_11() const
  {
    return this->flags_11_;
  }

  std::uint8_t& flags_11()
  {
    return this->flags_11_;
  }

  INesHeader& flags_11(std::uint8_t& value)
  {
    value = this->flags_11_;
    return *this;
  }

  INesHeader& set_flags_11(std::uint8_t value)
  {
    this->flags_11_ = value;
    return *this;
  }

  std::uint8_t flags_12() const
  {
    return this->flags_12_;
  }

  std::uint8_t& flags_12()
  {
    return this->flags_12_;
  }

  INesHeader& flags_12(std::uint8_t& value)
  {
    value = this->flags_12_;
    return *this;
  }

  INesHeader& set_flags_12(std::uint8_t value)
  {
    this->flags_12_ = value;
    return *this;
  }

  std::uint8_t flags_13() const
  {
    return this->flags_13_;
  }

  std::uint8_t& flags_13()
  {
    return this->flags_13_;
  }

  INesHeader& flags_13(std::uint8_t& value)
  {
    value = this->flags_13_;
    return *this;
  }

  INesHeader& set_flags_13(std::uint8_t value)
  {
    this->flags_13_ = value;
    return *this;
  }

  std::uint8_t flags_14() const
  {
    return this->flags_14_;
  }

  std::uint8_t& flags_14()
  {
    return this->flags_14_;
  }

  INesHeader& flags_14(std::uint8_t& value)
  {
    value = this->flags_14_;
    return *this;
  }

  INesHeader& set_flags_14(std::uint8_t value)
  {
    this->flags_14_ = value;
    return *this;
  }

  unsigned int prg_rom_byte_count() const
  {
    // 16 KB pages
    return this->prg_rom_page_count_ * 16384;
  }

  INesHeader& prg_rom_byte_count(int& value)
  {
    value = this->prg_rom_byte_count();
    return *this;
  }

  unsigned int chr_rom_byte_count() const
  {
    // 8 KB pages
    return this->chr_rom_page_count_ * 8192;
  }

  INesHeader& chr_rom_byte_count(unsigned int& value)
  {
    value = this->chr_rom_byte_count();
    return *this;
  }

  int mapper_number() const
  {
    return ((this->flags_6_ >> 4) | (this->flags_7_ & 0xf0) |
      ((this->flags_8_ & 0xf) << 8));
  }

  INesHeader& mapper_number(int& value)
  {
    value = this->mapper_number();
    return *this;
  }

  INesHeader& set_mapper_number(int value);

  bool is_four_screen_mode() const
  {
    return ((this->flags_6_ >> 3) & 0x1);
  }

  INesHeader& is_four_screen_mode(bool& value)
  {
    value = this->is_four_screen_mode();
    return *this;
  }

  INesHeader& set_is_four_screen_mode(bool value);

  bool has_trainer() const
  {
    return ((this->flags_6_ >> 2) & 0x1);
  }

  INesHeader& has_trainer(bool& value)
  {
    value = this->has_trainer();
    return *this;
  }

  INesHeader& set_has_trainer(bool value);

  bool has_battery() const
  {
    return ((this->flags_6_ >> 1) & 0x1);
  }

  INesHeader& has_battery(bool& value)
  {
    value = this->has_battery();
    return *this;
  }

  INesHeader& set_has_battery(bool value);

  MirroringKind mirroring() const
  {
    return (static_cast<MirroringKind>(this->flags_6_ & 0x1));
  }

  INesHeader& mirroring(MirroringKind& value)
  {
    value = this->mirroring();
    return *this;
  }

  INesHeader& set_mirroring(MirroringKind value);

  int version() const
  {
    return (((flags_7_ >> 2) & 0x3) != 2
      ? 1
      : 2);
  }

  INesHeader& set_version(int value);

  bool is_playchoice_10() const
  {
    return ((flags_7_ >> 1) & 0x1);
  }

  INesHeader& set_is_playchoice_10(bool value);

  bool is_vs_unisystem() const
  {
    return (flags_7_ & 0x1);
  }

  INesHeader& set_is_vs_unisystem(bool value);

  int submapper_number() const
  {
    return ((flags_8_ >> 4) & 0xf);
  }

  INesHeader& submapper_number(int& value)
  {
    value = this->submapper_number();
    return *this;
  }

  INesHeader& set_submapper_number(int value);
  
  bool IsValid();

  unsigned int GetPrgRomOffset() const;

  unsigned int GetPrgRomPageOffset(unsigned int pageNumber) const;

  unsigned int GetChrRomOffset() const;

  unsigned int GetChrRomPageOffset(unsigned int pageNumber) const;

private:
  char signature_1_[3];
  std::uint8_t signature_2_{0};
  std::uint8_t prg_rom_page_count_{0};
  std::uint8_t chr_rom_page_count_{0};

  // 7       0
  // ---------
  // NNNN FTBM
  // N: Lower 4 bits of the mapper number
  // F: Four screen mode (0 = no, 1 = yes; if set, M ignored)
  // T: Trainer (0 = no, 1 = yes, 512 byte trainer at $7000 - $71FF
  // M: Mirroring (0 = horizontal, 1 = vertical)
  std::uint8_t flags_6_{0};
  
  // 7       0
  // ---------
  // NNNN xxPV
  // N: Upper 4 bits of the mapper number
  // P: Playchoice 10. When set, this is a PC-10 game
  // V: Vs. Unisystem. When set, this is a Vs. game
  // x: (unused in iNES)
  //
  // For NES 2.0:
  //
  // 7       0
  // ---------
  // NNNN SSPV
  // N: Upper 4 bits of the mapper number
  // S: When equal to 2 (10b), use NES 2.0 rules
  // P: Playchoice 10. When set, this is a PC-10 game
  // V: Vs. Unisystem. When set, this is a Vs. game
  std::uint8_t flags_7_{0};

  // 7       0
  // ---------
  // SSSS MMMM
  // 
  // S: Submapper number.  Mappers not using submappers set this to zero.
  // M: Bits 8-11 of mapper number.
  std::uint8_t flags_8_{0};

  // TODO:
  // Remaining NES 2.0 fields from the rest of the flag bytes
  [[maybe_unused]]
  std::uint8_t flags_9_{0};

  [[maybe_unused]]
  std::uint8_t flags_10_{0};
  
  [[maybe_unused]]
  std::uint8_t flags_11_{0};
  
  [[maybe_unused]]
  std::uint8_t flags_12_{0};
  
  [[maybe_unused]]
  std::uint8_t flags_13_{0};
  
  [[maybe_unused]]
  std::uint8_t flags_14_{0};
  
  [[maybe_unused]]
  std::uint8_t reserved_{0};
};

}

#endif // !JVS_RIPPER2A03_INES_HEADER_H_
