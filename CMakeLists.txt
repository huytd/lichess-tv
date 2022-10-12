cmake_minimum_required(VERSION 3.10)

project(litv)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

file(GLOB MAIN_SOURCES CONFIGURE_DEPENDS
	"src/*.c"
)

add_compile_options(-Wall -Wextra -Werror -Wno-unused-parameter -fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(litv ${MAIN_SOURCES})

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(litv ncursesw curl pthread)
else()
	target_link_libraries(litv ncurses curl pthread)
endif()
