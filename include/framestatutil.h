// *****************************************************
//    Copyright 2024 Videonetics Technology Pvt Ltd
// *****************************************************

#pragma once
#ifndef framestatutil_h
#define framestatutil_h

#include <cstdint>
#include <framestatutil_export.h>
#include <string>

namespace vtpl::common {

class FRAMESTATUTIL_EXPORT StreamStat {

private:
  float_t fps;
  float_t kbps;
  int64_t frame_gap;
  int64_t discontinuity;
};

class FRAMESTATUTIL_EXPORT FrameStatUtil {
private:
  int64_t total_frames_;
  int64_t last_write_ts_{0};
  int64_t time_stamp_{0};
  int64_t last_time_stamp_{0};
  float   last_fps_{0.0};

public:
  FrameStatUtil(/* args */);
  ~FrameStatUtil();
};

} // namespace vtpl::common
#endif // framestatutil_h
