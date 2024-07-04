cmake_minimum_required(VERSION 3.28)
project(TextAndFileEncrypting)
set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
add_executable(TextAndFileEncrypting main.cpp)
set(DLL_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(DLL_NAME "CaesarLibrary")
add_custom_command(TARGET TextAndFileEncrypting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_DIRECTORY}/${DLL_NAME}.dll"
        $<TARGET_FILE_DIR:TextAndFileEncrypting>)
target_link_libraries(TextAndFileEncrypting "${DLL_DIRECTORY}/${DLL_NAME}.dll")
