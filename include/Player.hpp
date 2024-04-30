#pragma once
#include <string>
#include <gst/gst.h>

class Player {
  GstElement *pipeline = nullptr;
  void reset();
  std::string pipeline_arg;
public:
  void play(std::string url);
  Player();
  ~Player();
};
