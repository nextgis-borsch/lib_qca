if(WITH_cyrus-sasl_PLUGIN STREQUAL "yes")
  find_package(Sasl2 REQUIRED)
else(WITH_cyrus-sasl_PLUGIN STREQUAL "yes")
  find_package(Sasl2)
endif(WITH_cyrus-sasl_PLUGIN STREQUAL "yes")

if(SASL2_FOUND)
  enable_plugin("cyrus-sasl")

  set(QCA_SASL_SOURCES qca-cyrus-sasl.cpp)
  include_directories( ${SASL2_INCLUDE_DIR} )
  my_automoc( QCA_SASL_SOURCES )
  add_library(qca-cyrus-sasl ${PLUGIN_TYPE} ${QCA_SASL_SOURCES})

  if(APPLE AND ${PLUGIN_TYPE} STREQUAL "MODULE")
    set_property(TARGET qca-cyrus-sasl PROPERTY SUFFIX ".dylib")
    if(OSX_FRAMEWORK)
        set_target_properties(qca-cyrus-sasl PROPERTIES INSTALL_RPATH "@loader_path/../../Frameworks/") # Library/plugins/crypto/
    endif()
  endif()

  set(TARGET_LINK_LIB ${TARGET_LINK_LIB} ${SASL2_LIBRARIES})
  target_link_extlibraries(qca-cyrus-sasl)

  if(NOT DEVELOPER_MODE)
    install(TARGETS qca-cyrus-sasl
            LIBRARY DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
            ARCHIVE DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
            RUNTIME DESTINATION "${QCA_CRYPTO_INSTALL_DIR}")

    install_pdb(qca-cyrus-sasl ${QCA_CRYPTO_INSTALL_DIR})
  endif()
else(SASL2_FOUND)
  disable_plugin("cyrus-sasl")
endif(SASL2_FOUND)
