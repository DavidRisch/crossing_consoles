#ifndef CROSSING_CONSOLES_TIMING_HELPER_H
#define CROSSING_CONSOLES_TIMING_HELPER_H

#include <chrono>
#include <memory>

/**
 * \brief Used in tests to help with timing dependent code.
 */
class TimingHelper {
 public:
  TimingHelper();

  /**
   * \brief Return an instance of `TimingHelper`, may or may not create a new instance.
   */
  static const TimingHelper& GetInstance();

  /**
   * \brief Get a time interval which is longer if the hardware is slower.
   */
  template <class T>
  static std::chrono::duration<int64_t, T> HardwareDependentTime(double amount) {
    return (std::chrono::duration_cast<std::chrono::duration<int64_t, T>>(
        amount * TimingHelper::GetInstance().reference_duration));
  }

 private:
  void CalculateReferenceTime();

  static void DummyActions();

  std::chrono::duration<int64_t, std::nano> reference_duration{};

  static std::shared_ptr<TimingHelper> instance;
};

#endif  // CROSSING_CONSOLES_TIMING_HELPER_H
