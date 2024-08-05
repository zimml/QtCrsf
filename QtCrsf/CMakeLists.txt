cmake_minimum_required(VERSION 3.19)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Positioning Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Positioning Network REQUIRED)

set(_sources
		QtCrsf/sources/Crsf.cpp
		QtCrsf/sources/Crc.cpp
	)

set(_headers
		QtCrsf/sources/Crsf.h
		QtCrsf/sources/Crc.h
	)

qt_add_library(QtCrsf ${ADD_LIB_TYPE}
		${_sources}
		${_headers}
)

add_library(QtCrsf::QtCrsf ALIAS QtCrsf)

include(GenerateExportHeader)
generate_export_header(QtCrsf
				EXPORT_FILE_NAME QtCrsf_export.h)

target_link_libraries(QtCrsf
			Qt${QT_VERSION_MAJOR}::Core)

target_include_directories(QtCrsf
				PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(QtCrsf
				PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (BUILD_TESTING)
	add_subdirectory(tests)
endif()

