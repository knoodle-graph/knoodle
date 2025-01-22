# Adding modules

add_subdirectory(core)
add_subdirectory(ghi)
add_subdirectory(engine)

if(KNOODLE_WITH_VULKAN)
  add_subdirectory(vulkan_ghi)
endif()
