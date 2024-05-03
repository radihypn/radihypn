#pragma once
#include <string>
#include <gst/gst.h>

class Player {
  GstElement *pipeline = nullptr;
  std::string pipeline_arg;
public:
  void play(std::string url);
  void reset();
  Player();
  ~Player();
};
