
#pragma once
#include <chrono>
#include <functional>
#include <memory>
#include "common_type.hpp"

namespace dk::decision
{
  class TempControlState;

  class ITempControl
  {
   public:
    virtual ~ITempControl() = default;

    virtual void Update(const Common::units::Celsius& intake_temp) = 0;

    virtual Common::control::ControlType GetOutput() = 0;
    virtual Common::control::ControlType GetMode() = 0;

    virtual bool IsActive() const = 0;
  };
}  // namespace dk::decision