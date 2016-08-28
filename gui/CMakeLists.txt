project(CausalAnalysisGUI CXX)

find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Core)
find_package(Qt5Svg QUIET)

if(NOT Qt5Core_FOUND OR NOT Qt5Widgets_FOUND)
	message(STATUS "Could not find a Qt5 installation. The GUI will not be built.")
	return()
endif()

if(Qt5Svg_FOUND)
	set(CT_HAS_QT5_SVG true)
	if(Qt5Svg_VERSION_STRING VERSION_LESS "5.5.0")
		message(STATUS "QtSvg version below 5.5.0 detected. SVG export will still work, but will be of poor quality.")
	endif()
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

configure_file("../cmake/gui_config.h.cmake" "${PROJECT_BINARY_DIR}/gui_config.h")

QT5_WRAP_UI(ui_Headers
    mainwindow.ui
    matrixpopup.ui
    DiscretisationSelection.ui
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
	DiscretisationSelection.h
	DiscretisationSelection.cpp
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
	Qt5::Gui
)

if(Qt5Svg_FOUND)
	target_link_libraries(CausalTrailGui Qt5::Svg)
endif()
