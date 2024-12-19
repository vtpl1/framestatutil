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

class framestatutil_EXPORT FrameUtil {
private:
  int64_t total_frames_;
  int64_t last_write_ts_{0};
  int64_t time_stamp_{0};
  int64_t last_time_stamp_{0};
  float   last_fps_{0.0};

public:
  FrameUtil(/* args */);
  ~FrameUtil();
};

} // namespace vtpl::common
#endif // framestatutil_h
