find_package(Vulkan REQUIRED OPTIONAL_COMPONENTS dxc MoltenVK)

add_executable(shader_compiler
    "main.cpp"
    $<$<BOOL:${Vulkan_dxc_lib_FOUND}>:dxc_compiler.cpp>
    $<$<BOOL:${Vulkan_MoltenVK_FOUND}>:molten_compiler.cpp>
)

target_link_libraries(shader_compiler
  PRIVATE
    knoodle::core
    $<$<PLATFORM_ID:Windows>:Vulkan::dxc_lib>
    $<$<PLATFORM_ID:Windows>:dxcompiler.lib>
    $<$<PLATFORM_ID:Darwin>:Vulkan::MoltenVK>)

target_include_directories(shader_compiler
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
