// *****************************************************
//    Copyright 2024 Videonetics Technology Pvt Ltd
// *****************************************************

#pragma once
#ifndef framestat_h
#define framestat_h

#include <cmath>
#include <cstdint>
#include <framestatutil_export.h>
#include <string>
#include <vector>

namespace vtpl::common {

class FRAMESTATUTIL_EXPORT FrameStat {
private:
  int64_t site_id_;
  int64_t channel_id_;
  int64_t thread_id_;

  int64_t frame_counter_;
  int64_t framebits_counter_;
  int64_t discont_counter_;

  int64_t              last_received_ts_;
  std::vector<int64_t> arrival_ts_diffs_;
  int64_t              last_gop_median_;

  float_t fps_;
  float_t kbps_;
  float_t discont_ps_;

  int64_t last_calc_ts_;

public:
  FrameStat(int64_t site_id, int64_t channel_id, int64_t thread_id);
  ~FrameStat();

  void setStatus(int32_t media_type, int32_t frame_type, int32_t frame_size, int64_t timestamp, bool is_nalu);
  std::tuple<float_t, float_t, float_t> getStatus();
};

} // namespace vtpl::common
#endif // framestat_h
