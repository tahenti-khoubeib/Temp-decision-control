
#pragma once

#include <memory>
#include "common_type.hpp"

namespace dk::decision
{
  class TempControl;

  class ITempControlState
  {
   public:
    virtual ~ITempControlState() = default;

    virtual void Update(std::shared_ptr<TempControl> temp_control,
                        const Common::units::Celsius& intake_temp,
                        const Common::units::Celsius& threshold_min_temp,
                        const Common::units::Celsius& threshold_max_temp) = 0;

    virtual Common::control::ControlType GetOutput(std::shared_ptr<TempControl> temp_control) = 0;
  };

  class ActiveCoolingTempControlState : public ITempControlState
  {
   public:
    ActiveCoolingTempControlState(const ActiveCoolingTempControlState& other) = delete;
    ActiveCoolingTempControlState& operator=(const ActiveCoolingTempControlState& other) = delete;

    static std::shared_ptr<ITempControlState>& GetInstance();

    void Update(std::shared_ptr<TempControl> temp_control,
                const Common::units::Celsius& intake_temp,
                const Common::units::Celsius& threshold_min_temp,
                const Common::units::Celsius& threshold_max_temp) override;
    Common::control::ControlType GetOutput(std::shared_ptr<TempControl> temp_control) override;

   private:
    ActiveCoolingTempControlState() = default;
  };

  class ActiveHeatingTempControlState : public ITempControlState
  {
   public:
    ActiveHeatingTempControlState(const ActiveHeatingTempControlState& other) = delete;
    ActiveHeatingTempControlState& operator=(const ActiveHeatingTempControlState& other) = delete;

    static std::shared_ptr<ITempControlState>& GetInstance();
    void Update(std::shared_ptr<TempControl> temp_control,
                const Common::units::Celsius& intake_temp,
                const Common::units::Celsius& threshold_min_temp,
                const Common::units::Celsius& threshold_max_temp) override;

    Common::control::ControlType GetOutput(std::shared_ptr<TempControl> temp_control) override;

   private:
    ActiveHeatingTempControlState() = default;
  };

  class InactiveTempControlState : public ITempControlState
  {
   public:
    InactiveTempControlState(const InactiveTempControlState& other) = delete;
    InactiveTempControlState& operator=(const InactiveTempControlState& other) = delete;

    static std::shared_ptr<ITempControlState>& GetInstance();

    void Update(std::shared_ptr<TempControl> temp_control,
                const Common::units::Celsius& intake_temp,
                const Common::units::Celsius& threshold_min_temp,
                const Common::units::Celsius& threshold_max_temp) override;
                
    Common::control::ControlType GetOutput(std::shared_ptr<TempControl> temp_control) override;

   private:
    InactiveTempControlState() = default;
  };
}  // namespace dk::decision