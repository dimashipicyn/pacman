cmake_minimum_required(VERSION 3.0.0)
project(cpp_template_proj VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME} main.cpp)

find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

option(ASAN_ENABLED "Build this target with AddressSanitizer" OFF)
if(ASAN_ENABLED)
  if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /fsanitize=address)
  else()
    target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=address)
    target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=address)
  endif()
endif()
