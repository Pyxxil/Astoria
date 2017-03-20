CMAKE_MINIMUM_REQUIRED ( VERSION 3.6 )

PROJECT ( MusicPlayer )

SET ( CMAKE_CXX_STANDARD 14 )
SET ( CMAKE_INCLUDE_CURRENT_DIR ON )

SET ( CMAKE_AUTOMOC ON )
SET ( CMAKE_AUTOUIC ON )
SET ( CMAKE_AUTORCC ON )

SET ( SOURCE_FILES
	  source/controls/durationcontrols.cpp
	  source/controls/sensibleslider.cpp
	  source/controls/playercontrols.cpp
	  source/controls/volumecontrols.cpp
	  source/metadataeditordialog.cpp
	  source/library/librarymodel.cpp
	  source/library/musicscanner.cpp
	  source/menus/rightclickmenu.cpp
	  source/library/libraryview.cpp
	  source/library/playlist.cpp
	  source/trackinformation.cpp
	  source/menus/menubar.cpp
	  source/playerwindow.cpp
	  source/library/song.cpp
	  source/main.cpp
	  source/delegates/hoverdelegate.cpp
	  source/delegates/normaldelegate.cpp
	  source/globals.cpp
	  )

SET ( INCLUDE_FILES
      includes/controls/durationcontrols.hpp
      includes/controls/sensibleslider.hpp
      includes/controls/volumecontrols.hpp
      includes/controls/playercontrols.hpp
      includes/metadataeditordialog.hpp
      includes/library/librarymodel.hpp
      includes/menus/rightclickmenu.hpp
      includes/library/musicscanner.hpp
      includes/library/libraryview.hpp
      includes/library/playlist.hpp
      includes/trackinformation.hpp
      includes/delegates/normaldelegate.hpp
      includes/menus/menubar.hpp
      includes/delegates/hoverdelegate.hpp
      includes/playerwindow.hpp
      includes/library/song.hpp
      includes/globals.hpp
	  )

INCLUDE_DIRECTORIES ( ${CMAKE_SOURCE_DIR}/includes/taglib )

SET ( RCC_TARGETS
	  Icons.qrc
	  Styles.qrc
	  )

SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic" )

IF ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
	SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg -Werror" )
	SET ( CMAKE_VERBOSE_MAKEFILE ON )
ELSE ()
	# This assumes without specifying the build as debug, that it's a
	# release build
	SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2" )
ENDIF ()

ADD_DEFINITIONS ( -DProject="${PROJECT_NAME}" )

FIND_PACKAGE ( Qt5Widgets )
FIND_PACKAGE ( Qt5Multimedia )

FIND_LIBRARY ( TAGLIB tag PATH "${CMAKE_SOURCE_DIR}/libs/taglib/" )

ADD_EXECUTABLE ( ${PROJECT_NAME} ${INCLUDE_FILES} ${SOURCE_FILES} ${RCC_TARGETS} )
TARGET_LINK_LIBRARIES ( ${PROJECT_NAME} Qt5::Widgets Qt5::Multimedia ${TAGLIB} )
