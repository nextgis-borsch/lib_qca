if(WITH_nss_PLUGIN STREQUAL "yes")
  find_package(Nss REQUIRED)
else()
  find_package(Nss)
endif()

if(NSS_FOUND)
  enable_plugin("nss")

  set(QCA_NSS_SOURCES qca-nss.cpp)
  add_library(qca-nss ${PLUGIN_TYPE} ${QCA_NSS_SOURCES})

  set(TARGET_LINK_LIB ${TARGET_LINK_LIB} ${NSS_LDFLAGS})
  target_link_extlibraries(qca-nss)

  if(OSX_FRAMEWORK)
      set_target_properties(qca-nss PROPERTIES
          INSTALL_RPATH ${PLUGIN_RPATH}
          SUFFIX ".dylib")
  endif()

  if(NOT DEVELOPER_MODE)
    install(TARGETS qca-nss
            LIBRARY DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
            ARCHIVE DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
            RUNTIME DESTINATION "${QCA_CRYPTO_INSTALL_DIR}")

    #install_pdb(qca-nss ${QCA_CRYPTO_INSTALL_DIR})
  endif()
else()
  disable_plugin("nss")
  set(WITH_nss_PLUGIN "no" PARENT_SCOPE)
endif()
