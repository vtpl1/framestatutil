#include <cassert>
#include <iostream>
#include <thread>

#include "framestat.h"

int main(int argc, char const* argv[]) {

  std::cout << "tests started\n";

  vtpl::common::FrameStat frame_stat(1, 6, 0);

  int frame_counter = 0;
  while (true) {
    frame_counter++;

    frame_stat.setStatus(0, 0, 1024, 0, frame_counter % 10 == 0);

    auto [fps, kbps, discont_ps] = frame_stat.getStatus();
    // std::cout << "fps: " << fps << ", kbps: " << kbps << ", discont_ps: " << discont_ps << "\n";

    if (frame_counter == 1000) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  std::cout << "tests end\n";
  return 0;
}
