qt_add_library(core STATIC
  Database.cpp
)

target_link_libraries(core PRIVATE Qt6::Widgets Qt6::Sql)
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
