project(qcatool)

set(qcatool_bin_SRCS main.cpp)

add_executable(${QCA_TOOL_NAME} ${qcatool_bin_SRCS})

if(OSX_FRAMEWORK)
    set_target_properties(${QCA_TOOL_NAME} PROPERTIES INSTALL_RPATH "@executable_path/../../Library/Frameworks")
endif()

target_link_qca_libraries(${QCA_TOOL_NAME})

if(NOT DEVELOPER_MODE)
  install(TARGETS ${QCA_TOOL_NAME} DESTINATION "${QCA_BINARY_INSTALL_DIR}")
  #install_pdb(${QCA_TOOL_NAME} ${QCA_BINARY_INSTALL_DIR})
endif()
