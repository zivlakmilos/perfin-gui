qt_add_library(ui STATIC
  DWRibbon.cpp
  DWRibbon.ui

  WAccounts.cpp
  WAccounts.ui

  WBase.cpp
  Database.cpp

  MainWindow.cpp
)
target_link_libraries(ui PRIVATE core Qt6::Widgets Qt6::Sql)
target_include_directories(ui INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
