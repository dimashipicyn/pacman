cmake_minimum_required(VERSION 3.20)
project(pacman VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON) 
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME}
    main.cpp
    resources.qrc
 "source/Sprite.h" "source/Sprite.cpp" "source/SpriteSheet.h" "source/SpriteSheet.cpp" "source/Pacman.h" "source/Pacman.cpp" "source/Scene.h" "source/Scene.cpp" "source/Entity.h" "source/Entity.cpp" "source/Map.h" "source/Map.cpp")

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
