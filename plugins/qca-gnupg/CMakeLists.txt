# QCA GnuPG

enable_plugin("gnupg")

set(QCA_GNUPG_SOURCES
  qca-gnupg.cpp

  gpgop.cpp
  utils.cpp
  gpgproc/sprocess.cpp
  mypgpkeycontext.cpp
  mykeystoreentry.cpp
  myopenpgpcontext.cpp
  mykeystorelist.cpp
  mymessagecontext.cpp
  ringwatch.cpp
  lineconverter.cpp
  gpgaction.cpp
  gpgproc/gpgproc.cpp
)

set(QCA_GNUPG_HEADERS
  gpgaction.h
  ringwatch.h
  gpgop.h
  gpgop_p.h
  lineconverter.h
  mypgpkeycontext.h
  mykeystoreentry.h
  mykeystorelist.h
  gpgproc/gpgproc_p.h
  gpgproc/sprocess.h
  gpgproc/gpgproc.h
  utils.h
  mymessagecontext.h
  myopenpgpcontext.h
)

add_library(qca-gnupg ${PLUGIN_TYPE} ${QCA_GNUPG_SOURCES} ${EXTRA_GNUPG_SOURCES} ${QCA_GNUPG_HEADERS})

if(OSX_FRAMEWORK)
    set_target_properties(qca-gnupg PROPERTIES
        INSTALL_RPATH ${PLUGIN_RPATH}
        SUFFIX ".dylib")
endif()

include_directories(gpgproc)

if (WIN32)
  # target_link_libraries(qca-gnupg advapi32)
  set(TARGET_LINK_LIB ${TARGET_LINK_LIB} advapi32)
endif (WIN32)

target_link_extlibraries(qca-gnupg)

if(NOT DEVELOPER_MODE)
  install(TARGETS qca-gnupg DESTINATION
          LIBRARY DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
          ARCHIVE DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
          RUNTIME DESTINATION "${QCA_CRYPTO_INSTALL_DIR}")

  #install_pdb(qca-gnupg ${QCA_CRYPTO_INSTALL_DIR})
endif()
