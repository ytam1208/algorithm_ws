#include <chrono>
#include <iostream>

struct UniversalTimeScaleClock {
  using rep = int64_t;
  using period = std::ratio<1, 10000000>;
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<UniversalTimeScaleClock>;
  static constexpr bool is_steady = true;
};

using Duration = UniversalTimeScaleClock::duration;
using Time = UniversalTimeScaleClock::time_point;

Duration FromSeconds(const double seconds) {
  return std::chrono::duration_cast<Duration>(
      std::chrono::duration<double>(seconds));
}

Duration FromMilliseconds(const int64_t milliseconds) {
  return std::chrono::duration_cast<Duration>(
      std::chrono::milliseconds(milliseconds));
}



int main(int argc, char** argv)
{
  double seconds = 1.0;
  Duration duration1 = FromSeconds(seconds);

  double milliseconds = 1000.0;
  Duration duration2 = FromMilliseconds(milliseconds);

  std::cout << "Duration1: " << duration1.count() << std::endl;
  std::cout << "Duration2: " << duration2.count() << std::endl;
  
  Time max_time = Time::max();
  Time min_time = Time::min();
  std::cout << "Max time: " << max_time.time_since_epoch().count() << std::endl;
  std::cout << "Min time: " << min_time.time_since_epoch().count() << std::endl;

  using namespace std::chrono_literals;
  using fps_30 = std::chrono::duration<double, std::ratio<1, 30>>;
  std::cout << "10 second is: " << fps_30(10s).count() << " frames at 30fps\n";

  return 0;
}