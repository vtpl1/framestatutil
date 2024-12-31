// *****************************************************
//    Copyright 2024 Videonetics Technology Pvt Ltd
// *****************************************************

#include "framestat.h"

#include <chrono>
#include <cstdint>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <iostream>
#include <math.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <tuple>

namespace vtpl::common {
constexpr int TEN_MB_IN_BYTES = 10 * 1024 * 1024;

double median(std::vector<int64_t>& v) {
  // Sort the vector
  sort(v.begin(), v.end());
  auto n = v.size();
  // Check if the number of elements is odd
  if (n % 2 != 0)
    return (double)v[n / 2];

  // If the number of elements is even, return the average
  // of the two middle elements
  return (double)(v[(n - 1) / 2] + v[n / 2]) / 2.0;
}

FrameStat::FrameStat(int64_t site_id, int32_t channel_id, int32_t stream_type)
    : site_id_(site_id), channel_id_(channel_id), stream_type_(stream_type), status_(0.0, 0.0, 0.0),
      last_received_ts_(0), last_calc_ts_(0), frame_counter_(0), framebits_counter_(0), discont_counter_(0),
      last_gop_median_(0) {}

FrameStat::~FrameStat() {}

void FrameStat::setRawData(int32_t frame_size, int64_t frame_ts, int64_t reference_ts, bool is_nalu_or_iframe) {
  frame_counter_++;
  framebits_counter_ += frame_size * 8;

  if (last_calc_ts_ == 0) {
    last_calc_ts_ = reference_ts;
  }
  if (last_received_ts_ == 0) {
    last_received_ts_ = reference_ts;
  }

  if (last_gop_median_ > 0 && (reference_ts - last_received_ts_) >= (5 * last_gop_median_)) {
    discont_counter_++;
  }

  if (is_nalu_or_iframe) {
    last_gop_median_ = !arrival_ts_diffs_.empty() ? median(arrival_ts_diffs_) : 0;
    arrival_ts_diffs_.clear();
    arrival_ts_diffs_.push_back(reference_ts - last_received_ts_);
  }

  const int64_t time_diff = reference_ts - last_calc_ts_;
  if (time_diff >= 1000) {
    status_.fps        = static_cast<float>(frame_counter_ * 1000) / static_cast<float>(time_diff);
    status_.kbps       = static_cast<float>(framebits_counter_ * 1000 / 1024) / static_cast<float>(time_diff);
    status_.discont_ps = static_cast<float>(discont_counter_ * 1000) / static_cast<float>(time_diff);

    frame_counter_     = 0;
    framebits_counter_ = 0;
    discont_counter_   = 0;
    last_calc_ts_      = reference_ts;
  }
  last_received_ts_ = reference_ts;
}

Status FrameStat::getStatus() { return status_; }

void writeStartEnd(const std::chrono::time_point<std::chrono::system_clock>& start,
                   const std::chrono::time_point<std::chrono::system_clock>& end, const std::string& file_name) {
  // std::ofstream ofs(file_name, std::ios_base::app);
  std::stringstream ofs;
  ofs << "{";
  ofs << R"("systemTime":")" << fmt::format("{:%y-%m-%d %H:%M:%S}", std::chrono::system_clock::now()) << "\",";
  ofs << R"("start":")" << fmt::format("{:%y-%m-%d %H:%M:%S}", start) << "\",";
  ofs << R"("end":")" << fmt::format("{:%y-%m-%d %H:%M:%S}", end) << "\"";
  ofs << "}";
  auto logger = spdlog::get(file_name);
  if (logger == nullptr) {
    auto max_size  = TEN_MB_IN_BYTES;
    auto max_files = 3;
    logger         = spdlog::rotating_logger_st(file_name, file_name, max_size, max_files);
    logger->set_pattern("%v");
  }
  logger->info(ofs.str());
  // ofs.close();
}

void writeFrameMeta(int frame_count, int media_type, int frame_type, int frame_size, int64_t time_stamp,
                    std::string nalu_str, bool is_nal_detected, int64_t frame_time_gap, int64_t sys_time_gap,
                    const std::string& file_name) {
  // std::ofstream ofs(file_name, std::ios_base::app);
  std::stringstream ofs;
  ofs << "{";
  ofs << R"("systemTime":")" << fmt::format("{:%y-%m-%d %H:%M:%S}", std::chrono::system_clock::now()) << "\",";
  ofs << "\"mediaType\":" << media_type << ",";
  ofs << "\"frameType\":" << frame_type << ",";
  ofs << "\"nalu\":" << nalu_str << ",";
  ofs << "\"isNalDetected\":" << is_nal_detected << ",";
  ofs << "\"frameTimeGap\":" << frame_time_gap << ",";
  ofs << "\"sysTimeGap\":" << sys_time_gap << ",";
  ofs << "\"frameTimeGapMinusSysTimeGap\":" << (frame_time_gap - sys_time_gap) << ",";
  ofs << "\"frameSize\":" << frame_size << ",";
  ofs << "\"timeStamp\":" << time_stamp << ",";
  ofs << "\"frameCount\":" << frame_count;
  ofs << "\"}";
  // ofs.close();
  auto logger = spdlog::get(file_name);
  if (logger == nullptr) {
    auto max_size  = TEN_MB_IN_BYTES;
    auto max_files = 3;
    logger         = spdlog::rotating_logger_st(file_name, file_name, max_size, max_files);
    logger->set_pattern("%v");
  }
  logger->info(ofs.str());
}

} // namespace vtpl::common