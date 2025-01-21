set(core_VERSION ${PROJECT_VERSION})

@PACKAGE_INIT@

set_and_check(core_INCLUDE_DIR "@PACKAGE_INCLUDE_INSTALL_DIR@")
set_and_check(core_SYSCONFIG_DIR "@PACKAGE_SYSCONFIG_INSTALL_DIR@")

check_required_components(core)
