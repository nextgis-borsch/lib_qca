# QCA WinCrypto

IF (WIN32)
   SET (QCA_WINCRYPTO_SOURCES qca-wincrypto.cpp)

   ADD_LIBRARY (qca-wincrypto MODULE ${QCA_WINCRYPTO_SOURCES})
   # use win32 includes
   TARGET_LINK_LIBRARIES (qca-wincrypto advapi32)
   target_link_extlibraries(qca-wincrypto)

   INSTALL (TARGETS qca-wincrypto
        LIBRARY DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
        ARCHIVE DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
        RUNTIME DESTINATION "${QCA_CRYPTO_INSTALL_DIR}"
   )
ENDIF (WIN32)
