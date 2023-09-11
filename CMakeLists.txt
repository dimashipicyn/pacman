cmake_minimum_required(VERSION 3.20)
project(pacman VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON) 
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME}
    main.cpp
    resources.qrc
)

add_subdirectory(source)
add_subdirectory(third_party)

find_package(Qt5 COMPONENTS REQUIRED Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Widgets
)

target_include_directories(${PROJECT_NAME} PUBLIC
    source
)

if(ASAN_ENABLED)
  if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /fsanitize=address)
  else()
    target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=address)
    target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=address)
  endif()
endif()
