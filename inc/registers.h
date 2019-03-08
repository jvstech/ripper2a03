#if !defined(JVS_RIPPER2A03_REGISTERS_H_)
#define JVS_RIPPER2A03_REGISTERS_H_

namespace jvs
{

class Registers
{
public:
  Registers()
    : a_(0),
    x_(0),
    y_(0),
    pc_(0),
    s_(0xfd),
    p_(0x34)
  {
  }

  std::uint8_t a() const
  {
    return this->a_;
  }

  std::uint8_t& a()
  {
    return this->a_;
  }

  Registers& set_a(std::uint8_t value)
  {
    this->a_ = value;
    return *this;
  }

  std::uint8_t x() const
  {
    return this->x_;
  }

  std::uint8_t& x()
  {
    return this->x_;
  }

  Registers& set_x(std::uint8_t value)
  {
    this->x_ = value;
    return *this;
  }

  std::uint8_t y() const
  {
    return this->y_;
  }

  std::uint8_t& y()
  {
    return this->y_;
  }

  Registers& set_y(std::uint8_t value)
  {
    this->y_ = value;
    return *this;
  }

  std::uint16_t pc() const
  {
    return this->pc_;
  }

  std::uint16_t& pc()
  {
    return this->pc_;
  }

  Registers& set_pc(std::uint16_t value)
  {
    this->pc_ = value;
    return *this;
  }


private:
  std::uint8_t a_ : 8;
  std::uint8_t x_ : 8;
  std::uint8_t y_ : 8;
  std::uint16_t pc_ : 16;
  std::uint8_t s_ : 8;
  std::uint8_t p_ : 6;
};


} // namespace jvs

#endif // !JVS_RIPPER2A03_REGISTERS_H_