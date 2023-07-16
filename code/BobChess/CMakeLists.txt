

SET(BobChess_Headers
    utils.hpp
    move.hpp
    board.hpp
    random_generator.hpp
    magics.hpp
    move_list.hpp
    move_generator.hpp
    pesto_eval.hpp
    board_stack.hpp
    transposition.hpp
    algorithm_chess.hpp
    )

SET(BobChess_Impl
    utils.cpp
    move.cpp
    board.cpp
    random_generator.cpp
    magics.cpp
    move_list.cpp
    move_generator.cpp
    pesto_eval.cpp
    board_stack.cpp
    transposition.cpp
    algorithm_chess.cpp
    )

ADD_LIBRARY(BobChess_objects OBJECT 
    ${BobChess_Headers}  
    ${BobChess_Impl}
    )


TARGET_COMPILE_FEATURES(BobChess_objects
    PRIVATE cxx_std_20
    )

TARGET_LINK_LIBRARIES(BobChess_objects -fconcepts)

TARGET_INCLUDE_DIRECTORIES(BobChess_objects
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
    )

ADD_LIBRARY(BobChess $<TARGET_OBJECTS:BobChess_objects>)

ADD_LIBRARY(BobChess::BobChess ALIAS BobChess)

TARGET_LINK_LIBRARIES(BobChess -fconcepts)

SET_TARGET_PROPERTIES( BobChess
    PROPERTIES
    CXX_EXTENSIONS -fconcepts
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    PUBLIC_HEADER "${BobChess_headers}")


INCLUDE(GNUInstallDirs)

INSTALL(TARGETS BobChess BobChess_objects
    EXPORT BobChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTAL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTAL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTAL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTAL_INCLUDEDIR}/BobChess
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTAL_INCLUDEDIR}/BobChess
    )

INSTALL( EXPORT BobChessTargets
    FILE BobChessTargets.cmake
    NAMESPACE BobChess::
    DESTINATION ${CMAKE_INSTAL_LIBDIR}/cmake/BobChess)
    
enable_testing()

# test file

ADD_EXECUTABLE(test.t test.t.cpp)
TARGET_LINK_LIBRARIES(test.t BobChess::BobChess)

# tests

include(GoogleTest)
include(CTest)

ADD_EXECUTABLE(move_list.t move_list.t.cpp)
TARGET_LINK_LIBRARIES(move_list.t BobChess::BobChess GTest::gtest_main)
gtest_discover_tests(move_list.t)

ADD_EXECUTABLE(move_generator.t move_generator.t.cpp)
TARGET_LINK_LIBRARIES(move_generator.t BobChess::BobChess GTest::gtest_main)
gtest_discover_tests(move_generator.t)
