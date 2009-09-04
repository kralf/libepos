remake_project_option(WITH_UTILS "utility binaries" OFF)

remake_add_directories(lib)
remake_add_directories(bin IF WITH_UTILS)
