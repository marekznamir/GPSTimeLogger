add_library(App OBJECT)

set_target_properties(App PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/*.c
)

target_sources(App PRIVATE
    ${APP_SOURCES}
)

target_include_directories(App PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

set(STM32H7A3xx TRUE)
add_subdirectory(Inc/Display)
add_subdirectory(Inc/Delta)

target_link_libraries(App PUBLIC
    stm32cubemx
    Display
    Delta
)