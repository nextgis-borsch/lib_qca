if(WITH_botan_PLUGIN STREQUAL "yes")
  find_package(Botan REQUIRED)
else()
  find_package(Botan)
endif()

if(BOTAN_FOUND)
  enable_plugin("botan")

  set(QCA_BOTAN_SOURCES qca-botan.cpp)
  add_definitions(${BOTAN_CFLAGS})
  add_library(qca-botan ${PLUGIN_TYPE} ${QCA_BOTAN_SOURCES})

  if(OSX_FRAMEWORK)
    set_target_properties(qca-botan PROPERTIES
        INSTALL_RPATH ${PLUGIN_RPATH}
        SUFFIX ".dylib")
  endif()

  set(TARGET_LINK_LIB ${TARGET_LINK_LIB} ${BOTAN_LIBRARIES})
  target_link_extlibraries(qca-botan)

  if(NOT DEVELOPER_MODE)
    install(TARGETS qca-botan
            LIBRARY DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
            ARCHIVE DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
            RUNTIME DESTINATION "${QCA_CRYPTO_INSTALL_DIR}")

    install_pdb(qca-botan ${QCA_CRYPTO_INSTALL_DIR})
  endif()

else()
  disable_plugin("botan")
endif()
