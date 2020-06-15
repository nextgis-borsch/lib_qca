project(mozcerts)

set(mozcerts_bin_SRCS main.cpp)

add_executable(${MOZCERTS_NAME} ${mozcerts_bin_SRCS})

if(OSX_FRAMEWORK)
    set_target_properties(${MOZCERTS_NAME} PROPERTIES INSTALL_RPATH "@executable_path/../../Library/Frameworks")
endif()

target_link_qca_libraries(${MOZCERTS_NAME})

if(NOT DEVELOPER_MODE)
  install(TARGETS ${MOZCERTS_NAME} DESTINATION "${QCA_BINARY_INSTALL_DIR}")
  #install_pdb(${MOZCERTS_NAME} ${QCA_BINARY_INSTALL_DIR})
endif()
