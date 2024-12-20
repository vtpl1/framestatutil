// *****************************************************
//    Copyright 2024 Videonetics Technology Pvt Ltd
// *****************************************************

#include "framestatutil.h"
#include <chrono>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <cstdint>

namespace vtpl::common {
constexpr int MB_10 = 10 * 1024 * 1024;
FrameUtil::FrameUtil(/* args */) {}

FrameUtil::~FrameUtil() {}

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
    auto max_size  = MB_10;
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
    auto max_size  = MB_10;
    auto max_files = 3;
    logger         = spdlog::rotating_logger_st(file_name, file_name, max_size, max_files);
    logger->set_pattern("%v");
  }
  logger->info(ofs.str());
}

} // namespace vtpl::common