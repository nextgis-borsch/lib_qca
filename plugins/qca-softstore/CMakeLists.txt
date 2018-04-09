# qca-softstore

enable_plugin("softstore")
set(QCA_SOFTSTORE_SOURCES qca-softstore.cpp)
my_automoc( QCA_SOFTSTORE_SOURCES )

add_library(qca-softstore ${PLUGIN_TYPE} ${QCA_SOFTSTORE_SOURCES})

if(OSX_FRAMEWORK)
    set_target_properties(qca-softstore PROPERTIES
        INSTALL_RPATH ${PLUGIN_RPATH}
        SUFFIX ".dylib")
endif()

set_property(TARGET qca-softstore APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)
set_property(TARGET qca-softstore APPEND PROPERTY COMPILE_DEFINITIONS_RELWITHDEBINFO QT_NO_DEBUG)
set_property(TARGET qca-softstore APPEND PROPERTY COMPILE_DEFINITIONS_MINSIZEREL QT_NO_DEBUG)

add_definitions(${SOFTSTOREH_DEFINITIONS})
include_directories(${SOFTSTOREH_INCLUDE_DIR})
target_link_extlibraries(qca-softstore)

if(NOT DEVELOPER_MODE)
  install(TARGETS qca-softstore
          LIBRARY DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
          ARCHIVE DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
          RUNTIME DESTINATION "${QCA_CRYPTO_INSTALL_DIR}")

  install_pdb(qca-softstore ${QCA_CRYPTO_INSTALL_DIR})
endif()
