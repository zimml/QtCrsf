cmake_minimum_required(VERSION 3.19)

project(QtCrsf LANGUAGES CXX)

# Define C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)	 # Needed for Linux only !!
endif()

# Define flags needed for all
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Include CTest on top level to be able to run CTest from root
include(CTest)

# Setup Qt related stuffs
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_subdirectory(QtCrsf		${CMAKE_CURRENT_BINARY_DIR}/QtCrsf)
