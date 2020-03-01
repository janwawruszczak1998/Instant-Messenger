cmake_minimum_required(VERSION 3.13)
project(NokiaMessenger)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# Locate Boost
find_package(Boost 1.65.1 REQUIRED COMPONENTS 
	system
)

# Locate GTest
find_package(GTest REQUIRED)

# Locate Threads
find_package(Threads)


if(Boost_FOUND)

	add_executable(ClientMessenger client.cpp)

	add_executable(ServerMessenger server.cpp)

endif()

if(GTest_FOUND)

	add_executable(Tester tests.cpp)
	
endif()

# target server
target_link_directories(ServerMessenger PUBLIC
	"$(PROJECT_BINARY_DIR)"
	"${Boost_INCLUDE_DIRS}"
)
target_link_libraries(ServerMessenger PUBLIC
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})


# target client
target_link_directories(ClientMessenger PUBLIC
	"$(PROJECT_BINARY_DIR)"
	"${Boost_INCLUDE_DIRS}"
)
target_link_libraries(ClientMessenger PUBLIC
	${Boost_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})


# target tester
target_link_directories(Tester PUBLIC
	"$(PROJECT_BINARY_DIR)"
	"${Boost_INCLUDE_DIRS}"
)
target_link_libraries(Tester PUBLIC
	${Boost_LIBRARIES}
	${GTEST_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})