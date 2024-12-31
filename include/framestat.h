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

struct Status {
  float_t fps;
  float_t kbps;
  float_t discont_ps;

  Status()  = default;
  ~Status() = default;

  Status(float_t fps_, float_t kbps_, float_t discont_ps_) : fps(fps_), kbps(kbps_), discont_ps(discont_ps_) {}
};

class FRAMESTATUTIL_EXPORT FrameStat {
private:
  int64_t site_id_;
  int32_t channel_id_;
  int32_t stream_type_;

  int64_t frame_counter_;
  int64_t framebits_counter_;
  int64_t discont_counter_;

  int64_t              last_calc_ts_;
  int64_t              last_received_ts_;
  std::vector<int64_t> arrival_ts_diffs_;
  int64_t              last_gop_median_;

  Status status_;

public:
  FrameStat(int64_t site_id, int32_t channel_id, int32_t stream_type);
  ~FrameStat();

  void   setRawData(int32_t frame_size, int64_t frame_ts, int64_t reference_ts, bool is_nalu_or_iframe);
  Status getStatus();
};

} // namespace vtpl::common
#endif // framestat_h
