#include "ping-time.h"

#include <chrono>
#include <unistd.h>

int PingTime::timeMs() {
  auto now = std::chrono::steady_clock::now();

  auto duration = now.time_since_epoch();
  auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

  return durationMs.count();
}

void PingTime::microsecondDelay(unsigned int microseconds) { usleep(microseconds); }
