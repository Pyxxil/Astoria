CMAKE_MINIMUM_REQUIRED ( VERSION 3.6 )

SET ( PROJECT_NAME MusicPlayer )

PROJECT ( ${PROJECT_NAME} )

SET ( CMAKE_CXX_STANDARD 14 )
SET ( CMAKE_INCLUDE_CURRENT_DIR ON )

SET ( CMAKE_AUTOMOC ON )
SET ( CMAKE_AUTOUIC ON )

SET ( SOURCE_FILES
      source/main.cpp
      source/playercontrols.cpp
      source/playerwindow.cpp
      source/volumecontrols.cpp
      source/menubar.cpp
      )

SET ( INCLUDE_FILES
      includes/menubar.hpp
      includes/volumecontrols.hpp
      includes/playerwindow.hpp
      includes/playercontrols.hpp
      )

SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic" )

IF ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
	SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg -Werror" )
	SET ( CMAKE_VERBOSE_MAKEFILE ON )
ELSE ()
	SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2" )
ENDIF ()

FIND_PACKAGE ( Qt5Widgets )
FIND_PACKAGE ( Qt5Multimedia )

ADD_DEFINITIONS ( ${Qr5Widgets_DEFINITIONS} )
ADD_DEFINITIONS ( ${Qt5Multimedia_DEFINITIONS} )

ADD_EXECUTABLE ( ${PROJECT_NAME} ${INCLUDE_FILES} ${SOURCE_FILES} )
TARGET_LINK_LIBRARIES ( ${PROJECT_NAME} Qt5::Widgets Qt5::Multimedia )
