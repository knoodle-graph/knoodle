knoodle_create_module(
  NAME core
  NAMESPACE knoodle
  ENABLE_PCH On)

target_sources(
  core
  PRIVATE
  private/core.cpp
)
