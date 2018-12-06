#if !defined(JVS_RIPPER2A03_INTERRUPT_VECTORS_H_)
#define JVS_RIPPER2A03_INTERRUPT_VECTORS_H_

#include <cstdint>

namespace jvs
{

//!
//! @class InterruptVectors
//!
//! @brief
//!   Contains the addresses of the reset, NMI, and IRQ/BRK vectors
//!
struct InterruptVectors final
{
  std::uint16_t reset() const
  {
    return this->reset_;
  }

  InterruptVectors& reset(std::uint16_t& value)
  {
    value = this->reset_;
    return *this;
  }

  InterruptVectors& set_reset(std::uint16_t value)
  {
    this->reset_ = value;
    return *this;
  }

  std::uint16_t nmi() const
  {
    return this->nmi_;
  }

  InterruptVectors& nmi(std::uint16_t& value)
  {
    value = this->nmi_;
    return *this;
  }

  InterruptVectors& set_nmi(std::uint16_t value)
  {
    this->nmi_ = value;
    return *this;
  }

  std::uint16_t irq() const
  {
    return this->irq_;
  }

  InterruptVectors& irq(std::uint16_t& value)
  {
    value = this->irq_;
    return *this;
  }

  InterruptVectors& set_irq(std::uint16_t value)
  {
    this->irq_ = value;
    return *this;
  }


private:
  std::uint16_t nmi_{0};
  std::uint16_t reset_{0};
  std::uint16_t irq_{0};
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_INTERRUPT_VECTORS_H_
