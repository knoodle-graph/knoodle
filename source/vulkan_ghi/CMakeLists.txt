find_package(Vulkan REQUIRED COMPONENTS dxc volk)

add_library("vulkan-ghi" SHARED)

target_link_libraries("vulkan-ghi" PRIVATE knoodle-engine Vulkan::Vulkan)

target_sources("vulkan-ghi"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/vulkan_ghi.cpp"
    
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
  FILES
    "vulkan_ghi.hpp")

include (GenerateExportHeader)
generate_export_header("vulkan-ghi" EXPORT_MACRO_NAME KN_VULKAN_API EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/source/vulkan_ghi_api.hpp")

target_include_directories("vulkan-ghi"
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/source>
    
    $<INSTALL_INTERFACE:include>)

target_compile_features("vulkan-ghi" INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
target_compile_options("vulkan-ghi"
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /wd4251 /wd4275>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

option(KN_VULKAN_DEBUG "Enable Vulkan debug" OFF)
