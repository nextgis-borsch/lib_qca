ENABLE_TESTING()

set(symmetrickeyunittest_bin_SRCS symmetrickeyunittest.cpp)  

add_executable( symmetrickeyunittest ${symmetrickeyunittest_bin_SRCS} )

target_link_qca_test_libraries(symmetrickeyunittest)

add_qca_test(symmetrickeyunittest "SymmetricKey")
