#include <cassert>
#include <iostream>
#include <thread>

#include "framestat.h"

int main(int argc, char const* argv[]) {

  std::cout << "tests started\n";

  vtpl::common::FrameStat frame_stat(1, 6, 0);

  int     frame_counter = 0;
  int64_t frame_ts      = 0;
  while (true) {
    frame_counter++;

    int64_t reference_ts = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now())
                               .time_since_epoch()
                               .count();

    if (frame_ts == 0) {
      frame_ts = reference_ts;
    }

    frame_stat.setRawData(1024, frame_ts, reference_ts, frame_counter % 10 == 0);

    vtpl::common::Status status = frame_stat.getStatus();
    std::cout << "fps: " << status.fps << ", kbps: " << status.kbps << ", discont_ps: " << status.discont_ps << "\n";

    if (status.discont_ps > 0.001) {
      std::cout << "::::::::::::::::::::::::::::::::::: network poor: " << status.discont_ps << "\n";
    }

    if (frame_counter % 100 == 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5000));
      frame_ts += 5000;
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      frame_ts += 100;
    }
  }

  std::cout << "tests end\n";
  return 0;
}
