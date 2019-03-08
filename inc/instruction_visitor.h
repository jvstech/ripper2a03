#if !defined(JVS_RIPPER2A03_INSTRUCTION_VISITOR_H_)
#define JVS_RIPPER2A03_INSTRUCTION_VISITOR_H_

#include <cstdint>
#include <functional>

#include "instruction_data.h"
#include "instruction_info.h"

namespace jvs
{

template <typename InstructionVisitorT>
class InstructionVisitor
{
  const InstructionVisitorT& self() const
  {
    return static_cast<const InstructionVisitorT&>(*this);
  }

  InstructionVisitorT& self()
  {
    return static_cast<InstructionVisitorT&>(*this);
  }

public:
  unsigned int address() const
  {
    return this->address_;
  }

  unsigned int& address()
  {
    return this->address_;
  }

  InstructionVisitorT& set_address(unsigned int value)
  {
    this->address_ = value;
    return self();
  }
    
  bool should_continue() const
  {
    return this->should_continue_;
  }

  bool& should_continue()
  {
    return this->should_continue_;
  }

  InstructionVisitorT& set_should_continue(bool value)
  {
    this->should_continue_ = value;
    return self();
  }
    
  // X-macro for visitor functions
#define INSTRUCTION_MNEMONIC(m) \
  void Visit##m(const InstructionData& instData) { }
#include "mnemonics.def"

  void Visit(const InstructionData& instData)
  {
#define INSTRUCTION_MNEMONIC(m) \
    case jvs::MOSM:: m : \
      this->Visit##m(instData); \
      break;

    switch (instData.info().mnemonic())
    {
#include "mnemonics.def"
    }
  }
  
private:
  unsigned int address_{0};
  bool should_continue_{true};
};

class CallbackInstructionVisitor final : public InstructionVisitor<CallbackInstructionVisitor>
{
public:
  using CallbackVisitorFunction = 
    std::function<void(const InstructionData&, CallbackInstructionVisitor&)>;

  // X-macro for visitor function overrides, and callback accessors and mutators
#define INSTRUCTION_MNEMONIC(m) \
  void Visit##m(const InstructionData& instData); \
  const CallbackVisitorFunction& visit_##m() const; \
  CallbackVisitorFunction& visit_##m(); \
  CallbackInstructionVisitor& set_visit_##m(CallbackVisitorFunction cb);
#include "mnemonics.def"

private:

#define INSTRUCTION_MNEMONIC(m) \
  CallbackVisitorFunction visit_##m##_{[](const InstructionData&, CallbackInstructionVisitor&){}};
#include "mnemonics.def"
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_INSTRUCTION_VISITOR_H_
