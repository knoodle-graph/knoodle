find_package(Vulkan REQUIRED COMPONENTS SPIRV-Tools dxc)

set(ENABLE_SPIRV_CODEGEN CACHE BOOL "Enable SPIRV codegen" ON)

add_executable(shader_compiler
    "main.cpp"
    "shader_compiler_utils.cpp"
    "shader_compiler_structures.hpp"
    $<$<BOOL:${Vulkan_dxc_exe_FOUND}>:dxc_compiler.cpp>
)

target_link_libraries(shader_compiler
  PUBLIC
    knoodle::core
    Vulkan::SPIRV-Tools
    Vulkan::dxc_lib
    $<$<PLATFORM_ID:Windows>:dxcompiler.lib>)

target_include_directories(shader_compiler
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

add_test(NAME TestShaderCompiler COMMAND $<TARGET_FILE:shader_compiler> --log ${CMAKE_CURRENT_SOURCE_DIR}/test/test_shader_blend.kshader)
set_tests_properties(TestShaderCompiler PROPERTIES DEPENDS "shader_compiler")
