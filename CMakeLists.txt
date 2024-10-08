cmake_minimum_required(VERSION 3.26)

#message(${CMAKE_RC_COMPILER})

#cmake -DCMAKE_BUILD_TYPE=Release
#cmake -DCMAKE_BUILD_TYPE=Debug

project(GeraNES)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")    
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/template.html")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s STACK_SIZE=8MB -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_RUNTIME_METHODS=[ccall,cwrap] -s EXPORTED_FUNCTIONS=[_main,_malloc,_free]")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(CMakeRC)

include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.6
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

set(BUILD_SHARED_LIBS FALSE)

FetchContent_MakeAvailable(SDL2)

INCLUDE_DIRECTORIES(${SDL2_SOURCE_DIR}/include)

# FetchContent_Declare(
#         SDL2_image
#         GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
#         GIT_TAG release-2.6.3
#         GIT_SHALLOW TRUE
#         GIT_PROGRESS TRUE
# )

# set(SDL2IMAGE_INSTALL OFF)
# set(BUILD_SHARED_LIBS FALSE)

# FetchContent_MakeAvailable(SDL2_image)

# FetchContent_Declare(
#         freetype
#         GIT_REPOSITORY https://github.com/freetype/freetype
#         GIT_TAG VER-2-10-3
#         GIT_SHALLOW TRUE
#         GIT_PROGRESS TRUE
# )

# set(BUILD_SHARED_LIBS FALSE)

# FetchContent_MakeAvailable(freetype)

# IF(CMAKE_BUILD_TYPE MATCHES Debug)
#         set(FREETYPE_LIBRARY ${freetype_BINARY_DIR}/libfreetyped.a)
# ELSE()
#         set(FREETYPE_LIBRARY ${freetype_BINARY_DIR}/libfreetype.a)
# ENDIF()

# set(FREETYPE_INCLUDE_DIRS ${freetype_SOURCE_DIR}/include)

# INCLUDE_DIRECTORIES(${freetype_SOURCE_DIR}/include)

# FetchContent_Declare(
#         SDL2_ttf
#         GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
#         GIT_TAG release-2.20.2
#         GIT_SHALLOW TRUE
#         GIT_PROGRESS TRUE
# )

# set(SDL3TTF_SAMPLES_INSTALL OFF)
# set(BUILD_SHARED_LIBS FALSE)

# FetchContent_MakeAvailable(SDL2_ttf)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(imgui)

INCLUDE_DIRECTORIES(PUBLIC ${imgui_SOURCE_DIR})

set(IMGUI_PATH ${imgui_SOURCE_DIR})
file(GLOB IMGUI_GLOB
    ${IMGUI_PATH}/imgui.h
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imconfig.h
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_internal.h
    ${IMGUI_PATH}/imstb_rectpack.h
    ${IMGUI_PATH}/imstb_textedit.h
    ${IMGUI_PATH}/imstb_truetype.h
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp

    # specific bindings...
    ${IMGUI_PATH}/backends/imgui_impl_sdl2.h
    ${IMGUI_PATH}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp)
add_library("imgui" STATIC ${IMGUI_GLOB})

target_include_directories("imgui" PUBLIC ${IMGUI_PATH})
target_include_directories("imgui" PUBLIC ${IMGUI_PATH}/backends)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    message(STATUS "Compiling for Emscripten: Ignoring nfd libraries")
    # Defina uma variável ou faça ajustes conforme necessário para Emscripten
else()

    FetchContent_Declare(
            nfd
            GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
            GIT_TAG v1.0.2
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )

    FetchContent_MakeAvailable(nfd)

    INCLUDE_DIRECTORIES(${nfd_SOURCE_DIR}/src/include)

endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    message(STATUS "Compiling for Emscripten: Ignoring glew libraries")
    # Defina uma variável ou faça ajustes conforme necessário para Emscripten
else()

    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

    FetchContent_Declare(
            GLEW
            GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
            GIT_TAG glew-cmake-2.2.0
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )

    set(glew-cmake_BUILD_SHARED OFF)
    set(glew-cmake_BUILD_STATIC ON)
    set(ONLY_LIBS ON)
    set(BUILD_SHARED_LIBS FALSE)

    FetchContent_MakeAvailable(GLEW)

endif()


FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8

        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

set(BUILD_SHARED_LIBS FALSE)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.11.2

        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)

set(BUILD_SHARED_LIBS FALSE)

FetchContent_MakeAvailable(json)




INCLUDE_DIRECTORIES(src)

# Gets the list of files in the resources folder
file(GLOB_RECURSE RECURSOS_LISTA "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")

# Add the resource files using the list obtained
cmrc_add_resource_library(resources ${RECURSOS_LISTA})

# Copy data folder to build
if(EMSCRIPTEN)
    set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${DATA_DIR}@/")
else()

    file(GLOB source_files_and_dirs ${CMAKE_CURRENT_SOURCE_DIR}/data/*)

    # Copia cada item (diretório ou arquivo) para o destino
    foreach(item IN LISTS source_files_and_dirs)
        file(COPY ${item} DESTINATION ${CMAKE_BINARY_DIR})
    endforeach()

endif()


if(WIN32)
    set(OPENGL_LIBRARY "opengl32")
else()
    set(OPENGL_LIBRARY "GL")
endif()

set(CPP_FILES src/main.cpp src/GeraNES/RomFile.cpp src/sha1/sha1.cpp src/zip/zip.c src/signal/SigSlot.cpp src/GeraNESApp/ConfigFile.cpp src/GeraNESApp/SDLOpenGLWindow.cpp src/GeraNESApp/SDLUtil.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(CPP_FILES ${CPP_FILES} src/GeraNESApp/EmscriptenProcessFile.cpp src/GeraNESApp/EmscriptenFileDialog.cpp)
endif()

#SDL2_image::SDL2_image-static SDL2_ttf::SDL2_ttf-static freetype
set(LIBS -static-libgcc -static-libstdc++ SDL2::SDL2main SDL2::SDL2-static imgui glm nlohmann_json::nlohmann_json resources ${OPENGL_LIBRARY})

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(LIBS ${LIBS} libglew_static nfd stdc++fs)
else()
set(LIBS ${LIBS} -lopenal)
endif()


if(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}/src/resources.rc)
    set(CPP_FILES ${CPP_FILES} ${APP_ICON_RESOURCE_WINDOWS})
    set(LIBS ${LIBS} -ldwmapi)
endif()

add_executable(${CMAKE_PROJECT_NAME} ${CPP_FILES})
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIBS})