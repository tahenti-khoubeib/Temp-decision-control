#include "dk_decision_control.hpp"
#include "common_type.hpp"

using namespace Common::units;
using namespace dk::decision;

const bool TempControl::default_enable_{true};
const Common::units::Celsius TempControl::default_min_temp_threshold_{20.0_C};
const Common::units::Celsius TempControl::default_max_temp_threshold_{30.0_C};
const bool TempControl::default_close_{false};

std::shared_ptr<TempControl> TempControl::Create() { return std::shared_ptr<TempControl>(new TempControl()); }

void TempControl::Update(const Common::units::Celsius& intake_temp)
{
  // std::scoped_lock lck(async_mutex_);

  temp_control_state_->Update(shared_from_this(), intake_temp, temp_min_threshold_, temp_max_threshold_);
}

Common::control::ControlType TempControl::GetOutput()
{
  // std::scoped_lock lck(async_mutex_);
  return temp_control_state_->GetOutput(shared_from_this());
}

Common::control::ControlType TempControl::GetMode() { return GetOutput(); }

bool TempControl::IsActive() const { return temp_control_state_ != InactiveTempControlState::GetInstance(); }

const std::shared_ptr<ITempControlState>& TempControl::GetState() const { return temp_control_state_; }

void TempControl::SetState(std::shared_ptr<ITempControlState> state)
{
  if (temp_control_state_ != state)
  {
    temp_control_state_ = state;
  }
}

std::shared_ptr<ITempControlState>& InactiveTempControlState::GetInstance()
{
  static auto instance = std::shared_ptr<ITempControlState>(new InactiveTempControlState());
  return instance;
}

std::shared_ptr<ITempControlState>& ActiveCoolingTempControlState::GetInstance()
{
  static auto instance = std::shared_ptr<ITempControlState>(new ActiveCoolingTempControlState());
  return instance;
}

std::shared_ptr<ITempControlState>& ActiveHeatingTempControlState::GetInstance()
{
  static auto instance = std::shared_ptr<ITempControlState>(new ActiveHeatingTempControlState());
  return instance;
}

void InactiveTempControlState::Update(std::shared_ptr<TempControl> temp_control,
                                      const Common::units::Celsius& intake_temp,
                                      const Common::units::Celsius& threshold_min_temp,
                                      const Common::units::Celsius& threshold_max_temp)
{
  if (intake_temp > threshold_max_temp)
  {
    temp_control->SetState(ActiveCoolingTempControlState::GetInstance());
  }

  if (intake_temp < threshold_min_temp)
  {
    temp_control->SetState(ActiveHeatingTempControlState::GetInstance());
  }
  // else
  // {
  //   temp_control->SetState(InactiveTempControlState::GetInstance());
  // }
}

Common::control::ControlType InactiveTempControlState::GetOutput(std::shared_ptr<TempControl>) { return Common::control::ControlType::Fan; }

void ActiveCoolingTempControlState::Update(std::shared_ptr<TempControl> temp_control,
                                           const Common::units::Celsius& intake_temp,
                                           const Common::units::Celsius& threshold_min_temp,
                                           const Common::units::Celsius& threshold_max_temp)
{
  if (intake_temp < threshold_max_temp && intake_temp > threshold_min_temp)
  {
    temp_control->SetState(InactiveTempControlState::GetInstance());
  }

  if (intake_temp < threshold_min_temp)
  {
    temp_control->SetState(ActiveHeatingTempControlState::GetInstance());
  }
}

Common::control::ControlType ActiveCoolingTempControlState::GetOutput(std::shared_ptr<TempControl>)
{
  return Common::control::ControlType::Cooling;
}

void ActiveHeatingTempControlState::Update(std::shared_ptr<TempControl> temp_control,
                                           const Common::units::Celsius& intake_temp,
                                           const Common::units::Celsius& threshold_min_temp,
                                           const Common::units::Celsius& threshold_max_temp)
{
  if (intake_temp < threshold_max_temp && intake_temp > threshold_min_temp)
  {
    temp_control->SetState(InactiveTempControlState::GetInstance());
  }

  if (intake_temp > threshold_max_temp)
  {
    temp_control->SetState(ActiveCoolingTempControlState::GetInstance());
  }
}

Common::control::ControlType ActiveHeatingTempControlState::GetOutput(std::shared_ptr<TempControl>)
{
  return Common::control::ControlType::Heating;
}

void TempControl::RefreshHandler()
{
  std::scoped_lock lck(async_mutex_);
  Common::control::ControlType previousType = Common::control::ControlType::Fan;
  printf("Thread Started\n");
  auto start_time = std::chrono::steady_clock::now();
  using LoopDurationType = std::chrono::seconds;
  LoopDurationType loop_duration{15};

  while (true)
  {
    std::this_thread::sleep_for(refresh_interval_);
    auto currentType = GetOutput();

    if (currentType != previousType)
    {
      previousType = currentType;
      printf(" Thread Demand Control for the Indoor Unit: %s\n", Common::control::controlTypeToString(currentType).c_str());
    }

    auto elapsed_time = std::chrono::steady_clock::now() - start_time;
    if (elapsed_time >= loop_duration)
    {
      // Exit the loop after 10 seconds
      break;
    }
  }
}
