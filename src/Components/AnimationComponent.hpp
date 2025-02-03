#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

struct AnimationComponent {
  uint16_t num_of_frames;
  uint16_t current_frame;
  uint16_t frame_speed;
  bool is_loop;
  uint32_t start_time;

  AnimationComponent(uint8_t num_of_frames = 1, uint16_t frame_speed = 1, bool is_loop = true) 
  : num_of_frames{num_of_frames}, current_frame{1}, frame_speed{frame_speed}, is_loop{is_loop}, start_time{SDL_GetTicks()}
  {}
};
