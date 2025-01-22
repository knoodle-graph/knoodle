find_package(Vulkan REQUIRED)

add_library(vulkan-ghi SHARED)

knoodle_setup_module(
  TARGET vulkan-ghi
  API KN_VULKAN_API
  PRIVATE_DEPENDS core ghi Vulkan::Vulkan)

option(KN_VULKAN_DEBUG "Enable Vulkan debug" OFF)

target_sources(vulkan-ghi
  PRIVATE
    "vulkan_ghi.cpp"
    
  PUBLIC
  FILE_SET HEADERS
  FILES
    "vulkan_ghi.hpp")

target_link_libraries(vulkan-ghi
  PRIVATE
    $<$<PLATFORM_ID:Linux>:dl>)
