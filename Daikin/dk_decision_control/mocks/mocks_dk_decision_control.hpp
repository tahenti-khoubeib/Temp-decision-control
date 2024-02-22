#pragma once

#include <gmock/gmock.h>
#include "common_type.hpp"
#include "idk_decision_control.hpp"
using namespace Common::units;

namespace dk::decision
{

  class MockTempControl : public ITempControl
  {
   public:
    MOCK_METHOD(void, Update, (const Celsius& intake_temp), (override));
    MOCK_METHOD(Common::control::ControlType, GetOutput, (), (override));
    MOCK_METHOD(Common::control::ControlType, GetMode, (), (override));
    MOCK_METHOD(bool, IsActive, (), (const override));
  };

}  // namespace dk::decision