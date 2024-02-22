
#pragma once

#include <chrono>
#include <memory>
#include <mutex>
#include <system_error>
#include <thread>
#include "dk_decision_control_state.hpp"
#include "idk_decision_control.hpp"
namespace dk::decision
{

  class TempControl : public ITempControl, public std::enable_shared_from_this<TempControl>
  {
   public:
    static std::shared_ptr<TempControl> Create();

    ~TempControl() override = default;

    TempControl(const TempControl&) = delete;
    TempControl(TempControl&&) = delete;
    TempControl& operator=(const TempControl&) = delete;
    TempControl& operator=(TempControl&&) = delete;

    void Update(const Common::units::Celsius& intake_temp) override;

    Common::control::ControlType GetOutput() override;
    Common::control::ControlType GetMode() override;
    bool IsActive() const override;
    const std::shared_ptr<ITempControlState>& GetState() const;
    void SetState(std::shared_ptr<ITempControlState> state);

    void RefreshHandler();
    // std::shared_ptr<ITempControlState> temp_control_state_{InactiveTempControlState::GetInstance()};

   private:
    explicit TempControl() = default;

    std::shared_ptr<ITempControlState> temp_control_state_{InactiveTempControlState::GetInstance()};

    static const bool default_enable_;
    static const Common::units::Celsius default_min_temp_threshold_;
    static const Common::units::Celsius default_max_temp_threshold_;
    static const bool default_close_;

    bool enable_ = default_enable_;

    Common::units::Celsius temp_min_threshold_ = default_min_temp_threshold_;

    Common::units::Celsius temp_max_threshold_ = default_max_temp_threshold_;

    bool close_ = default_close_;

    std::chrono::seconds refresh_interval_{1};

    mutable std::mutex async_mutex_;
  };
}  // namespace dk::decision