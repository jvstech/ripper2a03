#if !defined(JVS_CONVERT_CAST_H_)
#define JVS_CONVERT_CAST_H_

namespace jvs
{

template <typename T, typename U>
struct ConvertCast 
{
  U operator()(const T& value) const
  {
    return reinterpret_cast<U>(value);
  }
};

template <typename ToType, typename FromType>
static auto ConvertTo(const FromType& value)
{
  return ConvertCast<FromType, ToType>{}(value);
}

} // namespace jvs

#endif // !JVS_CONVERT_CAST_H_
