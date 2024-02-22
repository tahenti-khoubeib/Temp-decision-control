#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "dk_decision_control_state.hpp"

namespace dk::decision
{

  class MockTempControlState : public ITempControlState
  {
   public:
    MOCK_METHOD(void,
                Update,
                (std::shared_ptr<TempControl> temp_control,
                 const Common::units::Celsius& intake_temp,
                 const Common::units::Celsius& threshold_min_temp,
                 const Common::units::Celsius& threshold_max_temp),
                (override));

    MOCK_METHOD(Common::control::ControlType, GetOutput, (std::shared_ptr<TempControl> temp_control), (override));
  };

}  // namespace dk::decision
