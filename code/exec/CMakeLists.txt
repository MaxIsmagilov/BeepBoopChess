
ADD_EXECUTABLE(BeepBoop0_0_0 BeepBoop0_0_0.cpp)
TARGET_LINK_LIBRARIES(BeepBoop0_0_0 PUBLIC BeepBoop)
target_include_directories(BeepBoop0_0_0 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../BeepBoop)


ADD_EXECUTABLE(_test _test.cpp)
TARGET_LINK_LIBRARIES(_test PUBLIC BeepBoop)
target_include_directories(_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../BeepBoop)