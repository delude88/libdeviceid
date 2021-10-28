cmake_minimum_required(VERSION 3.8)

project(DeviceId VERSION 0.0.1
        LANGUAGES CXX
        DESCRIPTION "A header only c++ library that provides a cross-platform unique device ID"
        HOMEPAGE_URL "https://github.com/delude88/libdeviceid")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# Configure artifact
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_TOPLEVEL_PROJECT TRUE)
else ()
    set(IS_TOPLEVEL_PROJECT FALSE)
endif ()

target_include_directories(
        ${PROJECT_NAME}
        INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
if (WIN32)
    target_compile_definitions(${PROJECT_NAME} WIN32)
endif ()

if (IS_TOPLEVEL_PROJECT)
    # locations are provided by GNUInstallDirs
    install(
            TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}_Targets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
            "${PROJECT_NAME}ConfigVersion.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion)

    configure_package_config_file(
            "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    install(
            EXPORT ${PROJECT_NAME}_Targets
            FILE ${PROJECT_NAME}Targets.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/deviceid
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif (IS_TOPLEVEL_PROJECT)

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

if (IS_TOPLEVEL_PROJECT)
    # Test
    add_executable(${PROJECT_NAME}-test)
    target_sources(${PROJECT_NAME}-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    target_link_libraries(${PROJECT_NAME}-test
            PRIVATE
            ${PROJECT_NAME}
            )
endif (IS_TOPLEVEL_PROJECT)
