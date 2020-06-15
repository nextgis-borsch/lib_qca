include(CheckIncludeFiles)
CHECK_INCLUDE_FILES(sys/filio.h HAVE_SYS_FILIO_H)
IF(HAVE_SYS_FILIO_H)
  ADD_DEFINITIONS(-DHAVE_SYS_FILIO_H)
ENDIF(HAVE_SYS_FILIO_H)

INCLUDE(CheckCXXSourceCompiles)
CHECK_CXX_SOURCE_COMPILES("
# include <stdlib.h>
# include <sys/mman.h>
int main() { void *f = 0; return mlock(f,8); }
" MLOCK_TAKES_VOID)
if(NOT MLOCK_TAKES_VOID)
  MESSAGE(STATUS "mlock(2) does not take a void *")
  ADD_DEFINITIONS(-DMLOCK_NOT_VOID_PTR)
endif(NOT MLOCK_TAKES_VOID)

if(DEVELOPER_MODE)
  add_definitions(-DQCA_PLUGIN_PATH="${CMAKE_BINARY_DIR}/lib/${QCA_LIB_NAME}")
else()
  if(USE_RELATIVE_PATHS)
    add_definitions(-DQCA_PLUGIN_PATH="${QCA_PREFIX_INSTALL_DIR}/${QCA_PLUGINS_INSTALL_DIR}")
  else()
    add_definitions(-DQCA_PLUGIN_PATH="${QCA_PLUGINS_INSTALL_DIR}")
  endif()
endif()

# base source files


SET( SOURCES
	qca_tools.cpp
	qca_plugin.cpp
	qca_textfilter.cpp
	qca_basic.cpp
	support/logger.cpp

	qca_cert.cpp
	qca_core.cpp
	qca_default.cpp
	qca_keystore.cpp
	qca_publickey.cpp
	qca_safeobj.cpp
	qca_safetimer.cpp
	qca_securelayer.cpp
	qca_securemessage.cpp
	support/qpipe.cpp
	support/console.cpp
	support/synchronizer.cpp
	support/dirwatch.cpp
	support/syncthread.cpp
)

IF (WIN32)
  SET( SOURCES ${SOURCES} qca_systemstore_win.cpp )
elseif(APPLE)
  set( SOURCES ${SOURCES} qca_systemstore_mac.cpp)
else()
  SET( SOURCES ${SOURCES} qca_systemstore_flatfile.cpp )
endif()

# Support files
#SET( qca_HEADERS ${qca_HEADERS} support/dirwatch/dirwatch_p.h )

# Botan tools
SET( botan_BASE botantools/botan )


INCLUDE_DIRECTORIES(support ${botan_BASE} )

ADD_DEFINITIONS(
	-DBOTAN_TYPES_QT
	-DBOTAN_NO_INIT_H
	-DBOTAN_NO_CONF_H
	-DBOTAN_TOOLS_ONLY
	-DBOTAN_MINIMAL_BIGINT
)

ADD_DEFINITIONS(
  -DBOTAN_MP_WORD_BITS=32
  -DBOTAN_KARAT_MUL_THRESHOLD=12
  -DBOTAN_KARAT_SQR_THRESHOLD=12
  -DBOTAN_EXT_MUTEX_QT
)

if(UNIX)
   ADD_DEFINITIONS( -DBOTAN_EXT_ALLOC_MMAP)
endif(UNIX)

SET( botan_SOURCES
	${botan_BASE}/util.cpp
	${botan_BASE}/exceptn.cpp
	${botan_BASE}/mutex.cpp
	${botan_BASE}/mux_qt/mux_qt.cpp
	${botan_BASE}/charset.cpp
	${botan_BASE}/defalloc.cpp
	${botan_BASE}/mp_comba.cpp
	${botan_BASE}/mp_mul.cpp
	${botan_BASE}/mp_shift.cpp
	${botan_BASE}/mp_misc.cpp
	${botan_BASE}/divide.cpp
	${botan_BASE}/big_base.cpp
	${botan_BASE}/big_code.cpp
	${botan_BASE}/big_io.cpp
	${botan_BASE}/big_ops2.cpp
	${botan_BASE}/big_ops3.cpp
	${botan_BASE}/bit_ops.cpp
	${botan_BASE}/libstate.cpp
	${botan_BASE}/mem_pool.cpp
	${botan_BASE}/modules.cpp
	${botan_BASE}/mp_asm.cpp
	${botan_BASE}/mp_mulop.cpp
	${botan_BASE}/parsing.cpp
)

IF (UNIX)
  SET( botan_SOURCES ${botan_SOURCES} ${botan_BASE}/ml_unix/mlock.cpp)
  SET( botan_SOURCES ${botan_SOURCES} ${botan_BASE}/alloc_mmap/mmap_mem.cpp)
ENDIF (UNIX)

IF(WIN32)
  SET( botan_SOURCES ${botan_SOURCES} ${botan_BASE}/ml_win32/mlock.cpp)
  # Hack to properly generate moc files
  add_definitions(-DQ_OS_WIN)
ENDIF(WIN32)

SET( SOURCES ${SOURCES} ${botan_SOURCES})

if(OSX_FRAMEWORK)
    SET( SOURCES ${SOURCES}
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/${QCA_PC_NAME}
        ${qca_CERTSTORE}
        ${CMAKE_BINARY_DIR}/mkspecs/features/crypto.prf
        # ${CMAKE_CONFIG_FILES}
        ${CMAKE_BINARY_DIR}/share/man/man1/${QCA_TOOL_NAME}.1
    )
endif()

add_library(${QCA_LIB_NAME} ${SOURCES}  ${public_HEADERS} )

if(WIN32)
    set(TARGET_LINK_LIB ${TARGET_LINK_LIB} crypt32 ws2_32)
	# target_link_libraries(${QCA_LIB_NAME} crypt32 ws2_32)
endif(WIN32)

if(APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(COREFOUNDATION_LIBRARY_SECURITY Security)

    set(TARGET_LINK_LIB ${TARGET_LINK_LIB} ${COREFOUNDATION_LIBRARY} ${COREFOUNDATION_LIBRARY_SECURITY})

   # set(COREFOUNDATION_LIBRARY "-framework CoreFoundation")
   # set(COREFOUNDATION_LIBRARY_SECURITY "-framework Security")
   # target_link_libraries(${QCA_LIB_NAME} ${COREFOUNDATION_LIBRARY} ${COREFOUNDATION_LIBRARY_SECURITY})
endif(APPLE)


target_link_libraries(${QCA_LIB_NAME} PUBLIC Qt5::Core PRIVATE ${TARGET_LINK_LIB})

set(TARGET_LINK_LIB ${TARGET_LINK_LIB} ${QCA_LIB_NAME} PARENT_SCOPE)

if(OSX_FRAMEWORK)
    set_property(SOURCE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/${QCA_PC_NAME} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/pkgconfig")
    set_property(SOURCE ${qca_CERTSTORE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/certs")
    set_property(SOURCE ${CMAKE_BINARY_DIR}/mkspecs/features/crypto.prf PROPERTY MACOSX_PACKAGE_LOCATION "Resources/features")
    # set_property(SOURCE ${CMAKE_CONFIG_FILES} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/CMake")
    set_property(SOURCE ${CMAKE_BINARY_DIR}/share/man/man1/${QCA_TOOL_NAME}.1 PROPERTY MACOSX_PACKAGE_LOCATION "Resources/man")

    set_target_properties(${QCA_LIB_NAME} PROPERTIES
        DEFINE_SYMBOL QCA_MAKEDLL
        FRAMEWORK TRUE
        FRAMEWORK_VERSION ${FRAMEWORK_VERSION}
        MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${VERSION}
        MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION}
        MACOSX_FRAMEWORK_IDENTIFIER org.qca.qca
        XCODE_ATTRIBUTE_INSTALL_PATH "@rpath"
        PUBLIC_HEADER "${public_HEADERS}"
    )
endif()

if(NOT ANDROID)
  set_target_properties(${QCA_LIB_NAME} PROPERTIES
                        VERSION ${VERSION}
                        SOVERSION ${QCA_LIB_MAJOR_VERSION}
                        )
endif()

if (WIN32)
    set_target_properties(${QCA_LIB_NAME} PROPERTIES
                      DEFINE_SYMBOL QCA_MAKEDLL
                      PUBLIC_HEADER "${public_HEADERS}"
                      FRAMEWORK FALSE
                      EXPORT_NAME ${QCA_LIB_NAME}
                      )
endif()

if(NOT DEVELOPER_MODE)
  # Do not split 'PUBLIC_HEADER ...' line. It means install headers to folder
  # and set this folder as -I flag for imported target.
  # Also EXPORT doesn't actually install any files. It only created a new target.
  install(TARGETS ${QCA_LIB_NAME} EXPORT ${PACKAGE_UPPER_NAME}Targets
          LIBRARY DESTINATION "${QCA_LIBRARY_INSTALL_DIR}"
          RUNTIME DESTINATION "${QCA_BINARY_INSTALL_DIR}"
          ARCHIVE DESTINATION "${QCA_LIBRARY_INSTALL_DIR}"
          FRAMEWORK DESTINATION "${QCA_LIBRARY_INSTALL_DIR}"
          PUBLIC_HEADER DESTINATION "${QCA_FULL_INCLUDE_INSTALL_DIR}" INCLUDES DESTINATION "${QCA_FULL_INCLUDE_INSTALL_DIR}"
  )
  #install_pdb(${QCA_LIB_NAME} ${QCA_BINARY_INSTALL_DIR})
endif()

if(NOT DEVELOPER_MODE)
    # Install the <Package>Config.cmake
    install(FILES
      ${PROJECT_BINARY_DIR}/${PACKAGE_UPPER_NAME}Config.cmake
      DESTINATION ${INSTALL_CMAKECONF_DIR} COMPONENT dev)

    # Install the export set for use with the install-tree
    install(EXPORT ${PACKAGE_UPPER_NAME}Targets DESTINATION ${INSTALL_CMAKECONF_DIR} COMPONENT dev)
endif()
