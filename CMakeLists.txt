cmake_minimum_required(VERSION 3.21)

project(3dPang
    VERSION 2.0.0
    DESCRIPTION "A 3D game inspired by Pang (Mitchell, 1989), with its level editor"
    HOMEPAGE_URL "https://github.com/andrealorenzani/3dPang"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(PANG3D_BUILD_PLAYER "Build the game (needs SDL3)" ON)
option(PANG3D_BUILD_EDITOR "Build the level editor" ON)
option(PANG3D_FETCH_DEPENDENCIES "Download SDL3/vcglib when they are not found" ON)
set(PANG3D_VCGLIB_DIR "" CACHE PATH "Existing vcglib checkout (empty: download it)")
set(PANG3D_VCGLIB_TAG "2025.07" CACHE STRING "vcglib release to download")
set(PANG3D_SDL3_TAG "release-3.4.20" CACHE STRING "SDL3 release to download")

include(FetchContent)
include(GNUInstallDirs)

# Installed programs find the libraries installed next to them (SDL3)
if(APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../${CMAKE_INSTALL_LIBDIR}")
elseif(UNIX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
endif()

# ---------------------------------------------------------------- dependencies

# Qt 6: images, XML, and the widgets/OpenGL of the editor
set(PANG3D_QT_COMPONENTS Core Gui Xml)
if(PANG3D_BUILD_EDITOR)
    list(APPEND PANG3D_QT_COMPONENTS Widgets OpenGL OpenGLWidgets)
endif()
find_package(Qt6 6.4 REQUIRED COMPONENTS ${PANG3D_QT_COMPONENTS})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Legacy (compatibility profile) OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# vcglib: header-only, with its own copy of Eigen
if(PANG3D_VCGLIB_DIR)
    set(VCGLIB_ROOT "${PANG3D_VCGLIB_DIR}")
else()
    FetchContent_Declare(vcglib
        GIT_REPOSITORY https://github.com/cnr-isti-vclab/vcglib.git
        GIT_TAG        ${PANG3D_VCGLIB_TAG}
        GIT_SHALLOW    TRUE)
    # Only the headers are needed: do not run vcglib's own CMakeLists
    FetchContent_GetProperties(vcglib)
    if(NOT vcglib_POPULATED)
        if(POLICY CMP0169)
            cmake_policy(SET CMP0169 OLD)
        endif()
        FetchContent_Populate(vcglib)
    endif()
    set(VCGLIB_ROOT "${vcglib_SOURCE_DIR}")
endif()
if(NOT EXISTS "${VCGLIB_ROOT}/vcg/math/matrix44.h")
    message(FATAL_ERROR "vcglib not found in '${VCGLIB_ROOT}'")
endif()
add_library(pang3d_vcglib INTERFACE)
target_include_directories(pang3d_vcglib SYSTEM INTERFACE "${VCGLIB_ROOT}" "${VCGLIB_ROOT}/eigenlib")

# SDL3 for the game window
if(PANG3D_BUILD_PLAYER)
    find_package(SDL3 CONFIG QUIET)
    if(NOT SDL3_FOUND)
        if(NOT PANG3D_FETCH_DEPENDENCIES)
            message(FATAL_ERROR "SDL3 not found: install it or enable PANG3D_FETCH_DEPENDENCIES")
        endif()
        message(STATUS "SDL3 not found: building ${PANG3D_SDL3_TAG} from source")
        set(SDL_SHARED ON CACHE BOOL "" FORCE)
        set(SDL_STATIC OFF CACHE BOOL "" FORCE)
        set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
        set(SDL_TESTS OFF CACHE BOOL "" FORCE)
        set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
        FetchContent_Declare(SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG        ${PANG3D_SDL3_TAG}
            GIT_SHALLOW    TRUE)
        FetchContent_MakeAvailable(SDL3)
        set(PANG3D_BUNDLE_SDL3 ON)
    endif()
endif()

# ---------------------------------------------------------------- engine

# The scene graph is shared by the two programs but compiled differently
# for each (PANG3D_PLAYER / PANG3D_EDITOR), as in 2005.
set(PANG3D_ENGINE_SOURCES
    src/common/BMF_font.c
    src/common/DataPaths.cpp
    src/common/Geometry.cpp
    src/common/GlUtil.cpp
    src/common/Model3ds.cpp
    src/common/TextWriter.cpp
    src/course/CICubeMap.cpp
    src/course/CIMd2.cpp
    src/course/CIMd2Normals.cpp
    src/course/CITrackBall.cpp
    src/scenegraph/Actors.cpp
    src/scenegraph/Bonus.cpp
    src/scenegraph/Environment.cpp
    src/scenegraph/Groups.cpp
    src/scenegraph/Nodes.cpp
    src/scenegraph/Projectiles.cpp
    src/scenegraph/SceneGraph.cpp)

function(pang3d_add_engine target define)
    add_library(${target} STATIC ${PANG3D_ENGINE_SOURCES})
    target_compile_definitions(${target} PUBLIC ${define}
        PANG3D_SOURCE_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
        PANG3D_VERSION="${PROJECT_VERSION}")
    target_include_directories(${target} PUBLIC src/common src/course src/scenegraph)
    target_link_libraries(${target} PUBLIC pang3d_vcglib Qt6::Core Qt6::Gui Qt6::Xml OpenGL::GL)
    set_target_properties(${target} PROPERTIES AUTOMOC OFF)
    if(MSVC)
        target_compile_options(${target} PUBLIC /W3 /utf-8 /bigobj)
        target_compile_definitions(${target} PUBLIC _CRT_SECURE_NO_WARNINGS)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    endif()
endfunction()

# Copies the data folder next to the executable, so that the build tree runs
# from anywhere (and from IDEs) without setting PANG3D_DATA_DIR
function(pang3d_copy_data target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/data" "$<TARGET_FILE_DIR:${target}>/data"
        COMMENT "Copying game data next to ${target}")
endfunction()

if(PANG3D_BUILD_PLAYER)
    pang3d_add_engine(pang3d_engine_player PANG3D_PLAYER)
    add_executable(3dpang WIN32
        src/player/main.cpp
        src/player/Game.cpp
        src/player/Menu.cpp)
    target_link_libraries(3dpang PRIVATE pang3d_engine_player SDL3::SDL3)
    set_target_properties(3dpang PROPERTIES AUTOMOC OFF)
    pang3d_copy_data(3dpang)
    if(WIN32)
        # Put SDL3.dll next to the executable
        add_custom_command(TARGET 3dpang POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_RUNTIME_DLLS:3dpang> $<TARGET_FILE_DIR:3dpang>
            COMMAND_EXPAND_LISTS)
    endif()
    install(TARGETS 3dpang RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    if(PANG3D_BUNDLE_SDL3)
        # Only the runtime library of the SDL3 built here (no headers)
        install(TARGETS SDL3-shared
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} NAMELINK_SKIP)
    endif()
endif()

if(PANG3D_BUILD_EDITOR)
    pang3d_add_engine(pang3d_engine_editor PANG3D_EDITOR)
    add_executable(3dpang-editor WIN32
        src/editor/main.cpp
        src/editor/MainWindow.cpp src/editor/MainWindow.h
        src/editor/EditorView.cpp src/editor/EditorView.h
        src/editor/ViewPreview.cpp src/editor/ViewPreview.h
        src/editor/Dialogs.h
        src/editor/AboutDialog.cpp
        src/editor/BallsDialog.cpp
        src/editor/BaseFloorDialog.cpp
        src/editor/FloorDialog.cpp
        src/editor/WallDialog.cpp
        src/editor/editor.qrc)
    target_link_libraries(3dpang-editor PRIVATE
        pang3d_engine_editor Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets)
    pang3d_copy_data(3dpang-editor)
    install(TARGETS 3dpang-editor RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

# Data and documentation
if(WIN32 OR APPLE)
    set(PANG3D_INSTALL_DATADIR "${CMAKE_INSTALL_BINDIR}/data")
else()
    set(PANG3D_INSTALL_DATADIR "${CMAKE_INSTALL_DATADIR}/3dpang/data")
endif()
install(DIRECTORY data/ DESTINATION ${PANG3D_INSTALL_DATADIR})
install(FILES README.md LICENSE CHANGELOG.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(DIRECTORY docs/ DESTINATION ${CMAKE_INSTALL_DOCDIR})

# Smoke tests: play every level for 20 seconds (shooting), off screen.
# SDL's "offscreen" driver needs EGL; set PANG3D_TEST_VIDEO_DRIVER to x11,
# wayland or windows to use a real (hidden) window instead.
enable_testing()
if(PANG3D_BUILD_PLAYER)
    set(PANG3D_TEST_VIDEO_DRIVER "offscreen" CACHE STRING "SDL video driver used by the tests")
    file(GLOB PANG3D_LEVELS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/data/levels" data/levels/*.xml)
    foreach(level IN LISTS PANG3D_LEVELS)
        add_test(NAME demo_${level} COMMAND 3dpang --demo ${level} --frames 1200 --autofire)
        set_tests_properties(demo_${level} PROPERTIES
            TIMEOUT 300
            ENVIRONMENT "SDL_VIDEO_DRIVER=${PANG3D_TEST_VIDEO_DRIVER}")
    endforeach()
endif()

set(CPACK_PACKAGE_NAME "3dPang")
set(CPACK_PACKAGE_VENDOR "Andrea Lorenzani")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_GENERATOR "TGZ;ZIP")
include(CPack)
