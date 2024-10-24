cmake_minimum_required(VERSION 3.25)
project(wstest)

set(CMAKE_CXX_STANDARD 23)

# include_directories(include)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/websocket.cpp.o
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)
