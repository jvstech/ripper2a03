#if !defined(JVS_RIPPER2A03_MIRRORING_KIND_H_)
#define JVS_RIPPER2A03_MIRRORING_KIND_H_

#include <string>

#include "convert_cast.h"

namespace jvs
{

enum class MirroringKind
{
  Horizontal = 0,
  Vertical = 1
};

template <>
struct ConvertCast<MirroringKind, std::string>
{
  std::string operator()(MirroringKind mirroringKind) const
  {
    return (mirroringKind == MirroringKind::Horizontal
      ? "horizontal"
      : "vertical");
  }
};

}

#endif // !JVS_RIPPER2A03_MIRRORING_KIND_H_