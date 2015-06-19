cmake_minimum_required (VERSION 2.8.11)

project (Causal-Analysis-gui)

find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Core)
find_package(Qt5Svg)

if(NOT Qt5Core_FOUND OR NOT Qt5Widgets_FOUND)
	message(STATUS "Could not find a Qt5 installation. The GUI will not be built.")
	return()
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

QT5_WRAP_UI(ui_Headers
    mainwindow.ui
    matrixpopup.ui
    discretisationselection.ui
    DataView.ui
    QueryView.ui
)

add_executable(CausalTrailGui
	Config.cpp
	datastorage.cpp
	DataView.h
	DataView.cpp
	DataMatrixView.h
	DataMatrixView.cpp
	DataMatrixModel.h
	DataMatrixModel.cpp
	discretisationselection.cpp
	edge.cpp
	listwidgetmulticopy.cpp
	main.cpp
	mainwindow.cpp
	matrixpopup.cpp
	methodcombobox.cpp
	NetworkInstance.cpp
	networkvis.cpp
	NodeGui.cpp
	querymanager.cpp
	QueryView.h
	QueryView.cpp
	${ui_Headers}
)

target_link_libraries(CausalTrailGui
	CausalTrailLib
	${Boost_LIBRARIES}
	Qt5::Widgets
	Qt5::Core
	Qt5::Svg
	Qt5::Gui
)
