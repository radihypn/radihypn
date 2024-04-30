#include <Player.hpp>
#include <iostream>

void Player::reset() {
  if (pipeline != nullptr) {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    pipeline = nullptr;
  }
}

void Player::play(std::string url) {
  reset();

  pipeline_arg = "playbin uri=" + url;
  pipeline = gst_parse_launch(
    pipeline_arg.c_str(),
    nullptr);
  //std::cout << pipeline_arg << std::endl;
  gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

Player::Player() {
  gst_init(0, nullptr);
  pipeline = nullptr;
}

Player::~Player() {
  // Clean up GStreamer
  if (pipeline) {
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
  }

  gst_deinit();
}
