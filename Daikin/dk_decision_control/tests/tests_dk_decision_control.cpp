#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <functional>
#include "common_type.hpp"
#include "dk_decision_control.hpp"
#include "mock_dk_decision_control_state.hpp"
#include "mocks_dk_decision_control.hpp"

using namespace dk::decision;
using namespace ::testing;
using namespace Common::units;
using namespace Common::control;
using namespace ::testing;
constexpr auto MAX_THRESHOLD_TEMP = Celsius{30.0};
constexpr auto MIN_THRESHOLD_TEMP = Celsius{20.0};

class TempControlTest : public Test
{
 public:
  TempControlTest() {}

  ~TempControlTest() {}

  void SetUp()
  {
    TempControl_ = TempControl::Create(MIN_THRESHOLD_TEMP, MAX_THRESHOLD_TEMP);
    // TempControl_->temp_control_state_ = std::move(mockState);
  }

 protected:
  //  MockFTempControlFactory mockState {};
  std::shared_ptr<TempControl> TempControl_;
  Common::control::ControlType control{Common::control::ControlType::Fan};
};

TEST_F(TempControlTest, Active)
{
  TempControl_->Update(35.0_C);
  EXPECT_TRUE(TempControl_->IsActive());
  ASSERT_TRUE(TempControl_->GetOutput() == Common::control::ControlType::Cooling);
}

TEST_F(TempControlTest, NotActive)
{
  TempControl_->Update(25.0_C);
  EXPECT_FALSE(TempControl_->IsActive());
  ASSERT_TRUE(TempControl_->GetOutput() == Common::control::ControlType::Fan);
}

// TEST_F(TempControlTest, GetCurrentMode)
// {
//   EXPECT_CALL(TempControl, GetMode());
//   TempControl_->GetOutput();
//   EXPECT_FALSE(TempControl_->IsActive());
//   ASSERT_TRUE(TempControl_->GetOutput() == Common::control::ControlType::Fan);
// }

TEST_F(TempControlTest, GoingToActiveStateTransitions)
{
  TempControl_->SetState(InactiveTempControlState::GetInstance());
  ASSERT_TRUE(TempControl_->GetState() == InactiveTempControlState::GetInstance());

  InactiveTempControlState::GetInstance()->Update(TempControl_, 19.0_C, MIN_THRESHOLD_TEMP, MAX_THRESHOLD_TEMP);
  ASSERT_FALSE(TempControl_->GetState() == ActiveCoolingTempControlState::GetInstance());
  ASSERT_TRUE(TempControl_->GetState() == ActiveHeatingTempControlState::GetInstance());
  ASSERT_EQ(TempControl_->GetOutput(), ControlType::Heating);

  ActiveHeatingTempControlState::GetInstance()->Update(TempControl_, 35.0_C, MIN_THRESHOLD_TEMP, MAX_THRESHOLD_TEMP);
  ASSERT_FALSE(TempControl_->GetState() == ActiveHeatingTempControlState::GetInstance());
  ASSERT_TRUE(TempControl_->GetState() == ActiveCoolingTempControlState::GetInstance());
  ASSERT_EQ(TempControl_->GetOutput(), ControlType::Cooling);

  ActiveHeatingTempControlState::GetInstance()->Update(TempControl_, 25.0_C, MIN_THRESHOLD_TEMP, MAX_THRESHOLD_TEMP);
  ASSERT_FALSE(TempControl_->GetState() == ActiveCoolingTempControlState::GetInstance());
  ASSERT_FALSE(TempControl_->GetState() == ActiveHeatingTempControlState::GetInstance());

  ASSERT_TRUE(TempControl_->GetState() == InactiveTempControlState::GetInstance());
  ASSERT_EQ(TempControl_->GetOutput(), ControlType::Fan);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}