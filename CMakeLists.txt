cmake_minimum_required(VERSION 3.22)
Project(cplat_project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE LIB_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/lib-src/*.c"
)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows -static")
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wstrict-overflow=3 -Wimplicit -Wconversion -Wformat -O2 -std=c99")

# copying header files to include dir
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib-src")
set(DEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE HEADER_FILES RELATIVE "${SOURCE_DIR}" "${SOURCE_DIR}/*.h")

foreach(header IN LISTS HEADER_FILES)
    set(src "${SOURCE_DIR}/${header}")
    set(dst "${DEST_DIR}/${header}")
    get_filename_component(dst_dir "${dst}" DIRECTORY)
    file(MAKE_DIRECTORY "${dst_dir}")
    file(COPY "${src}" DESTINATION "${dst_dir}")
endforeach()

# CPLAT Libray
add_library(cplat STATIC ${LIB_SOURCES})

# logging macros
target_compile_definitions(cplat PRIVATE NO_LOGGING)
#target_compile_definitions(cplat PRIVATE CP_DEBUG)

if(WIN32)
    target_link_libraries(cplat PUBLIC opengl32 gdi32)
else()
    target_link_libraries(cplat PUBLIC xcb xcb-util xcb-icccm xcb-keysyms X11-xcb X11 GL EGL)
endif()

target_link_libraries(cplat PUBLIC)

# CPLAT test executable
add_executable(cplat_test ${SOURCES})
target_include_directories(cplat_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(cplat_test PUBLIC cplat)

