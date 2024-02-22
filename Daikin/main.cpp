#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include "dk_decision_control/include/dk_decision_control.hpp"
using namespace Common::units;
using namespace dk::decision;

int main(int argc, char* argv[])
{
  try
  {
    auto temp_control = TempControl::Create();

    temp_control->Update(25.0_C);

    printf(" MAIN::Demand Control for the Indoor Unit With 25.0_C: %s\n",
           Common::control::controlTypeToString(temp_control->GetOutput()).c_str());

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::thread myThread(&TempControl::RefreshHandler, temp_control.get());

    std::this_thread::sleep_for(std::chrono::seconds(3));
    temp_control->Update(35.0_C);
    printf("MAIN:: Demand Control for the Indoor Unit With 35.0_C: %s\n",
           Common::control::controlTypeToString(temp_control->GetOutput()).c_str());

    std::this_thread::sleep_for(std::chrono::seconds(3));
    temp_control->Update(15.0_C);
    printf("MAIN:: Demand Control for the Indoor Unit With 15.0_C: %s\n",
           Common::control::controlTypeToString(temp_control->GetOutput()).c_str());

    std::this_thread::sleep_for(std::chrono::seconds(3));
    temp_control->Update(25.0_C);
    printf("MAIN:: Demand Control for the Indoor Unit With 25.0_C: %s\n",
           Common::control::controlTypeToString(temp_control->GetOutput()).c_str());

    myThread.join();

    printf("Hello, world! Second Try %d \n", temp_control->IsActive());

    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
}