
cmake_minimum_required(VERSION 3.16)
project(NokiaMessenger)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CSS_EXTENSION OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
find_package(Boost 1.65.1 REQUIRED COMPONENTS 
	date_time
	system
)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	add_executable(ClientMessenger client.cpp)
	target_link_libraries(ClientMessenger ${Boost_LIBRARIES})

	include_directories(${Boost_INCLUDE_DIRS})
	add_executable(ServerMessenger server.cpp)
	target_link_libraries(ServerMessenger ${Boost_LIBRARIES})
endif()
