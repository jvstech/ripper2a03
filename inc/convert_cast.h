#if !defined(JVS_CONVERT_CAST_H_)
#define JVS_CONVERT_CAST_H_

namespace jvs
{

template <typename FromType, typename ToType>
struct ConvertCast 
{
  ToType operator()(const FromType& value) const
  {
    return reinterpret_cast<ToType>(value);
  }
};

template <typename ToType, typename FromType>
static auto ConvertTo(const FromType& value)
{
  return ConvertCast<FromType, ToType>{}(value);
}

} // namespace jvs

#endif // !JVS_CONVERT_CAST_H_
