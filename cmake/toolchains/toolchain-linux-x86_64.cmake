# toolchain-arm64.cmake

# Specify the target system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Specify the compiler
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Specify the location of the sysroot (optional, for libraries and headers)
# Replace <path-to-sysroot> with your actual sysroot directory, if needed.
# set(CMAKE_SYSROOT /path/to/sysroot)

# Other optional settings
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE arm64)
