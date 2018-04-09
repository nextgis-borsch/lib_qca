# qca-logger

enable_plugin("logger")
set(QCA_LOGGER_SOURCES qca-logger.cpp)
my_automoc( QCA_LOGGER_SOURCES )

add_library(qca-logger ${PLUGIN_TYPE} ${QCA_LOGGER_SOURCES})

if(OSX_FRAMEWORK)
    set_target_properties(qca-logger PROPERTIES
        INSTALL_RPATH ${PLUGIN_RPATH}
        SUFFIX ".dylib")
endif()

set_property(TARGET qca-logger APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)
set_property(TARGET qca-logger APPEND PROPERTY COMPILE_DEFINITIONS_RELWITHDEBINFO QT_NO_DEBUG)
set_property(TARGET qca-logger APPEND PROPERTY COMPILE_DEFINITIONS_MINSIZEREL QT_NO_DEBUG)

add_definitions(${LOGGERH_DEFINITIONS})
include_directories(${LOGGERH_INCLUDE_DIR})
target_link_extlibraries(qca-logger)

if(NOT DEVELOPER_MODE)
  install(TARGETS qca-logger
          LIBRARY DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
          ARCHIVE DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
          RUNTIME DESTINATION "${QCA_CRYPTO_INSTALL_DIR}")

  install_pdb(qca-logger ${QCA_CRYPTO_INSTALL_DIR})
endif()
