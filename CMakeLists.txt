cmake_minimum_required(VERSION 3.10)
project(lista VERSION 1.1.0 LANGUAGES C) 

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(Tlist STATIC src/Tlist.c src/Titerator.c)

target_compile_options(Tlist PRIVATE 
    $<$<NOT:$<BOOL:${VCPKG_BUILD}>>:-Wall -Wextra -Wpedantic -Werror>  
    $<$<BOOL:${VCPKG_BUILD}>:-Wall -Wextra -Wpedantic>                
)

target_include_directories(Tlist PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/tlist-deprecated>
)

include(GNUInstallDirs)

install(TARGETS Tlist
    EXPORT tlist-deprecated-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tlist-deprecated
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT tlist-deprecated-targets
    FILE tlist-deprecated-targets.cmake
    NAMESPACE tlist-deprecated::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tlist-deprecated
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tlist-deprecated-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/tlist-deprecated-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tlist-deprecated
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/tlist-deprecated-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/tlist-deprecated-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/tlist-deprecated-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tlist-deprecated
)

export(EXPORT tlist-deprecated-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/cmake/tlist-deprecated-targets.cmake
    NAMESPACE tlist-deprecated::
)