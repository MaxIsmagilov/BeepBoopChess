

SET(BobChess_Headers
    thread_pool.hpp
    utils.hpp
    move.hpp
    board.hpp
    random_generator.hpp
    magics.hpp
    move_list.hpp
    move_generator.hpp
    eval.hpp
    board_stack.hpp
    transposition.hpp
    timekeeping.hpp
    algorithm_chess.hpp
    move_finder.hpp
    )

SET(BobChess_Impl
    thread_pool.cpp
    utils.cpp
    move.cpp
    board.cpp
    random_generator.cpp
    magics.cpp
    move_list.cpp
    move_generator.cpp
    eval.cpp
    board_stack.cpp
    transposition.cpp
    timekeeping.cpp
    algorithm_chess.cpp
    move_finder.cpp
    )

ADD_LIBRARY(BobChess_objects OBJECT 
    ${BobChess_Headers}  
    ${BobChess_Impl}
    )


TARGET_COMPILE_FEATURES(BobChess_objects
    PRIVATE cxx_std_20
    )


target_include_directories(BobChess_objects
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

ADD_LIBRARY(BobChess $<TARGET_OBJECTS:BobChess_objects>)

ADD_LIBRARY(BobChess::BobChess ALIAS BobChess)


if (WIN32)
    SET_TARGET_PROPERTIES( BobChess
        PROPERTIES
        CXX_EXTENSIONS NO
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        COMPILE_OPTIONS "-Wall -Wextra -Werror"
        PUBLIC_HEADER "${BobChess_headers}")
else ()
    SET_TARGET_PROPERTIES( BobChess
        PROPERTIES
        CXX_EXTENSIONS -lpthread 
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        COMPILE_OPTIONS "-Wall -Wextra -Werror"
        PUBLIC_HEADER "${BobChess_headers}")
endif()

INCLUDE(GNUInstallDirs)

INSTALL(TARGETS BobChess BobChess_objects
    EXPORT BobChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTAL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTAL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTAL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTAL_INCLUDEDIR}/third_party
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
target_compile_options(test.t PRIVATE -g)

# tests

include(GoogleTest)
include(CTest)

ADD_EXECUTABLE(move_list.t move_list.t.cpp)
TARGET_LINK_LIBRARIES(move_list.t BobChess::BobChess GTest::gtest_main)
gtest_discover_tests(move_list.t)

ADD_EXECUTABLE(move_generator.t move_generator.t.cpp)
TARGET_LINK_LIBRARIES(move_generator.t BobChess::BobChess GTest::gtest_main)
gtest_discover_tests(move_generator.t)
