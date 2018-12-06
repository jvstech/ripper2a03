#include "ines_header.h"


jvs::INesHeader& jvs::INesHeader::set_mapper_number(int value)
{
  // clear the lower 4 bits of the mapper number from flags_6_ (which is
  // actually the upper 4 bits of flags_6_)
  this->flags_6_ &= 0x0f;
  // set the lower 4 bits of the mapper number
  this->flags_6_ |= ((static_cast<std::uint8_t>(value) & 0xf) << 4);
  // clear the upper 4 bits of the mapper number from flags_7_ (again, which is
  // actually the upper 4 bits of flags_7_)
  this->flags_7_ &= 0x0f;
  // set the upper 4 bits of the mapper number
  this->flags_7_ |= (static_cast<std::uint8_t>(value) << 4);
  // clear the lower 4 bits of the mapper number from flags_8_ (which is 
  // actually bits 8-11 of the NES 2.0 mapper number)
  this->flags_8_ &= 0xf0;
  // set bits 8-11 of the NES 2.0 mapper number
  this->flags_8_ |= ((value >> 8) & 0x0f);
  return *this;
}

jvs::INesHeader& jvs::INesHeader::set_is_four_screen_mode(bool value)
{
  if (value)
  {
    this->flags_6_ |= (1 << 3);
  }
  else
  {
    this->flags_6_ &= ~(1 << 3);
  }

  return *this;
}

jvs::INesHeader& jvs::INesHeader::set_has_trainer(bool value)
{
  if (value)
  {
    this->flags_6_ |= (1 << 2);
  }
  else
  {
    this->flags_6_ &= ~(1 << 2);
  }

  return *this;
}

jvs::INesHeader& jvs::INesHeader::set_has_battery(bool value)
{
  if (value)
  {
    this->flags_6_ |= 0x2;
  }
  else
  {
    this->flags_6_ &= ~0x2;
  }

  return *this;
}

jvs::INesHeader& jvs::INesHeader::set_mirroring(MirroringKind value)
{
  if (value == MirroringKind::Vertical)
  {
    this->flags_6_ |= 0x1;
  }
  else
  {
    // horizontal mirroring
    this->flags_6_ &= ~0x1;
  }

  return *this;
}

jvs::INesHeader& jvs::INesHeader::set_version(int value)
{
  // clamp to version 1 or 2
  value = (value < 1 ? 1 : (value > 2 ? 2 : value));
  if (value == 2)
  {
    this->flags_7_ |= 0x8;
  }
  else
  {
     // clear both version bits to set iNES version 1.0
    this->flags_7_ &= ~0xC;
  }

  return *this;
}

jvs::INesHeader& jvs::INesHeader::set_is_playchoice_10(bool value)
{
  if (value)
  {
    this->flags_7_ |= 0x2;
  }
  else
  {
    this->flags_7_ &= ~0x2;
  }

  return *this;
}

jvs::INesHeader& jvs::INesHeader::set_is_vs_unisystem(bool value)
{
  if (value)
  {
    this->flags_7_ |= 0x1;
  }
  else
  {
    this->flags_7_ &= ~0x1;
  }

  return *this;
}

jvs::INesHeader& jvs::INesHeader::set_submapper_number(int value)
{
  // clear the upper 4 bits
  this->flags_8_ &= 0x0f;
  // set the upper 4 bits
  this->flags_8_ |= ((static_cast<std::uint8_t>(value) & 0xf) << 4);
  return *this;
}

bool jvs::INesHeader::IsValid()
{
  return (
    // I don't feel like using strcmp or memcmp right now
    signature_1_[0] == 'N' &&
    signature_1_[1] == 'E' &&
    signature_1_[2] == 'S' &&
    signature_2_ == 0x1a
    );
}

unsigned int jvs::INesHeader::GetPrgRomOffset() const
{
  return (sizeof(*this) +
    (this->has_trainer()
      ? 512
      : 0));
}

unsigned int jvs::INesHeader::GetPrgRomPageOffset(unsigned int pageNumber) const
{
  pageNumber = (pageNumber >= this->prg_rom_page_count()
    ? this->prg_rom_page_count() - 1
    : pageNumber);
  return (this->GetPrgRomOffset() + (PrgRomPageSize * pageNumber));
}

unsigned int jvs::INesHeader::GetChrRomOffset() const
{
  return (this->GetPrgRomOffset() + this->prg_rom_byte_count());
}

unsigned int jvs::INesHeader::GetChrRomPageOffset(unsigned int pageNumber) const
{
  pageNumber = (pageNumber > this->chr_rom_page_count() 
    ? this->chr_rom_page_count() 
    : pageNumber);
  return (this->GetChrRomOffset() + (ChrRomPageSize * pageNumber));
}

