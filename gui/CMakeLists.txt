cmake_minimum_required (VERSION 2.8.11)

project (Causal-Analysis-gui)

add_definitions(-std=c++11 -Wall -pedantic)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

QT5_WRAP_UI(ui_Headers
    mainwindow.ui
    matrixpopup.ui
    discretisationselection.ui
)

add_executable(CausalTrailGui main.cpp datastorage.cpp mainwindow.cpp mainwindow.ui matrixpopup.cpp matrixpopup.ui methodcombobox.cpp NetworkInstance.cpp networkvis.cpp NodeGui.cpp querymanager.cpp discretisationselection.cpp discretisationselection.ui edge.cpp ${ui_Headers}) 

target_link_libraries(CausalTrailGui CausalTrailLib ${Boost_LIBRARIES} Qt5::Widgets)
