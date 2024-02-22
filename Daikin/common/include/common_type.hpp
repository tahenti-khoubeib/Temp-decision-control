
#ifndef COMMON_TYPES_HPP_
#define COMMON_TYPES_HPP_
#include <unordered_map>

namespace Common
{

  template <class T>
  class CommonType
  {
   public:
    constexpr CommonType() = default;
    constexpr explicit CommonType(T value) : value_(value) {}
    constexpr operator T() const { return value_; }
    CommonType<T>& operator+=(const T& val)
    {
      this->value_ += val;
      return *this;
    }
    CommonType<T>& operator-=(const T& val)
    {
      this->value_ -= val;
      return *this;
    }
    CommonType<T>& operator=(const T& val)
    {
      this->value_ = val;
      return *this;
    }

    auto operator<=>(const CommonType&) const = default;

    constexpr CommonType<T>& operator-()
    {
      this->value_ = -this->value_;
      return *this;
    }

   private:
    T value_{};
  };

  namespace control
  {
    enum class ControlType { Cooling, Heating, Fan };

    inline std::string controlTypeToString(ControlType type)
    {
      static const std::unordered_map<ControlType, std::string> controlTypeStrings{
        {ControlType::Cooling, "Cooling"}, {ControlType::Heating, "Heating"}, {ControlType::Fan, "Fan"}};

      auto it = controlTypeStrings.find(type);
      return (it != controlTypeStrings.end()) ? it->second : "Unknown";
    }

  }  // namespace control

  namespace units
  {
    /**
     * @brief Temperature units
     */
    typedef CommonType<double> Celsius;
    constexpr Celsius operator"" _C(long double degrees_celsius) { return Celsius(degrees_celsius); }

  }  // namespace units

}  // namespace Common

#endif  // COMMON_TYPES_HPP_
