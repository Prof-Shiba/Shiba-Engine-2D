#pragma once

struct GodModeComponent {
  bool god_mode_enabled;
  GodModeComponent(bool god_mode_enabled = false) : god_mode_enabled{god_mode_enabled} {}
};
