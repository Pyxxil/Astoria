cmake_minimum_required ( VERSION 3.6 )

project ( Astoria )

set ( CMAKE_CXX_STANDARD 14 )
set ( CMAKE_INCLUDE_CURRENT_DIR ON )

set ( CMAKE_AUTOMOC ON )
set ( CMAKE_AUTOUIC ON )
set ( CMAKE_AUTORCC ON )

set ( SOURCE_FILES
      source/controls/durationcontrols.cpp
      source/delegates/normaldelegate.cpp
      source/controls/sensibleslider.cpp
      source/controls/playercontrols.cpp
      source/controls/volumecontrols.cpp
      source/delegates/hoverdelegate.cpp
      source/metadataeditordialog.cpp
      source/library/librarymodel.cpp
      source/library/musicscanner.cpp
      source/menus/rightclickmenu.cpp
      source/library/libraryview.cpp
      source/astoria/playlist.cpp
      source/library/playlist.cpp
      source/trackinformation.cpp
      source/menus/menubar.cpp
      source/astoria/audio.cpp
      source/playerwindow.cpp
      source/library/song.cpp
      source/astoria/ui.cpp
      source/astoria.cpp
      source/main.cpp
      )

set ( INCLUDE_FILES
      includes/controls/durationcontrols.hpp
      includes/delegates/normaldelegate.hpp
      includes/controls/sensibleslider.hpp
      includes/controls/volumecontrols.hpp
      includes/controls/playercontrols.hpp
      includes/delegates/hoverdelegate.hpp
      includes/metadataeditordialog.hpp
      includes/library/librarymodel.hpp
      includes/menus/rightclickmenu.hpp
      includes/library/musicscanner.hpp
      includes/library/libraryview.hpp
      includes/library/playlist.hpp
      includes/trackinformation.hpp
      includes/menus/menubar.hpp
      includes/playerwindow.hpp
      includes/library/song.hpp
      includes/astoria.hpp
      )

include_directories ( ${CMAKE_SOURCE_DIR}/includes/taglib )

set ( RCC_TARGETS
      Icons.qrc
      Styles.qrc
      )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic" )

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg -Werror -pedantic -Wshadow -Wnon-virtual-dtor" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wsign-conversion" )
    set ( CMAKE_VERBOSE_MAKEFILE ON )
else ()
    # This assumes that without specifying the build as debug, that it's a
    # release build.
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2" )
endif ()

add_definitions ( -DProject="${PROJECT_NAME}" )

find_package ( Qt5Widgets )
find_package ( Qt5Multimedia )

find_library ( TAGLIB tag PATHS "${CMAKE_SOURCE_DIR}/libs/taglib" NO_DEFAULT_PATH )

add_executable ( ${PROJECT_NAME} ${INCLUDE_FILES} ${SOURCE_FILES} ${RCC_TARGETS} )
target_link_libraries ( ${PROJECT_NAME} Qt5::Widgets Qt5::Multimedia ${TAGLIB} )
