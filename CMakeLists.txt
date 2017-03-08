CMAKE_MINIMUM_REQUIRED ( VERSION 3.6 )

PROJECT ( MusicPlayer )

IF ( APPLE )
    # This isn't exactly perfect, but it would make it so the user can define
    # their own qt5 installation directory.
    SET ( CMAKE_PREFIX_PATH $ENV{QT5_INSTALL_DIR} )
ENDIF ()

SET ( CMAKE_CXX_STANDARD 14 )
SET ( CMAKE_INCLUDE_CURRENT_DIR ON )

SET ( CMAKE_AUTOMOC ON )
SET ( CMAKE_AUTOUIC ON )

SET ( SOURCE_FILES
	  source/main.cpp
	  source/controls/playercontrols.cpp
	  source/playerwindow.cpp
	  source/controls/volumecontrols.cpp
	  source/menus/menubar.cpp
	  source/controls/durationcontrols.cpp
	  source/library/librarymodel.cpp
	  source/library/musicscanner.cpp
	  source/menus/rightclickmenu.cpp
	  source/controls/durationslider.cpp
	  )

SET ( INCLUDE_FILES
	  includes/menus/menubar.hpp
	  includes/controls/volumecontrols.hpp
	  includes/playerwindow.hpp
	  includes/controls/playercontrols.hpp
	  includes/controls/durationcontrols.hpp
	  includes/library/librarymodel.hpp
	  includes/menus/rightclickmenu.hpp
	  includes/library/musicscanner.hpp
	  includes/controls/durationslider.hpp
	  )

MESSAGE ("SOURCE DIRECTORY: ${CMAKE_SOURCE_DIR}")

SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic" )

IF ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
	SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg -Werror" )
	SET ( CMAKE_VERBOSE_MAKEFILE ON )
ELSE ()
	SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2" )
ENDIF ()

ADD_DEFINITIONS ( -DProject="${PROJECT_NAME}" )

FIND_PACKAGE ( Qt5Widgets )
FIND_PACKAGE ( Qt5Multimedia )

ADD_EXECUTABLE ( ${PROJECT_NAME} ${INCLUDE_FILES} ${SOURCE_FILES} )
TARGET_LINK_LIBRARIES ( ${PROJECT_NAME} Qt5::Widgets Qt5::Multimedia )
