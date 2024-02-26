

SET(BeepBoop_Headers
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
    game.hpp
    )

SET(BeepBoop_Impl
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
    game.cpp
    )

ADD_LIBRARY(BeepBoop_objects OBJECT 
    ${BeepBoop_Headers}  
    ${BeepBoop_Impl}
    )


TARGET_COMPILE_OPTIONS(
    BeepBoop_objects PUBLIC "-std=c++20"
    )


target_include_directories(BeepBoop_objects
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

ADD_LIBRARY(BeepBoop $<TARGET_OBJECTS:BeepBoop_objects>)

ADD_LIBRARY(BeepBoop::BeepBoop ALIAS BeepBoop)


if (WIN32)
    SET_TARGET_PROPERTIES( BeepBoop
        PROPERTIES
        CXX_EXTENSIONS NO
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        COMPILE_OPTIONS "-Wall -Wextra -Wpedantic -Werror -std=c++20"
        PUBLIC_HEADER "${BeepBoop_headers}")
else ()
    SET_TARGET_PROPERTIES( BeepBoop
        PROPERTIES
        CXX_EXTENSIONS -lpthread 
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        COMPILE_OPTIONS "-Wall -Wextra -Wpedantic -Werror -std=c++20"
        PUBLIC_HEADER "${BeepBoop_headers}")
endif()

INCLUDE(GNUInstallDirs)

INSTALL(TARGETS BeepBoop BeepBoop_objects
    EXPORT BeepBoopTargets
    RUNTIME DESTINATION ${CMAKE_INSTAL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTAL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTAL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTAL_INCLUDEDIR}/third_party
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTAL_INCLUDEDIR}/BeepBoop
    )

INSTALL( EXPORT BeepBoopTargets
    FILE BeepBoopTargets.cmake
    NAMESPACE BeepBoop::
    DESTINATION ${CMAKE_INSTAL_LIBDIR}/cmake/BeepBoop)
    
enable_testing()

# tests

include(GoogleTest)
include(CTest)



ADD_EXECUTABLE(utils.t utils.t.cpp)
TARGET_LINK_LIBRARIES(utils.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(utils.t)

ADD_EXECUTABLE(transposition.t transposition.t.cpp)
TARGET_LINK_LIBRARIES(transposition.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(transposition.t)

ADD_EXECUTABLE(timekeeping.t timekeeping.t.cpp)
TARGET_LINK_LIBRARIES(timekeeping.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(timekeeping.t)

ADD_EXECUTABLE(thread_pool.t thread_pool.t.cpp)
TARGET_LINK_LIBRARIES(thread_pool.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(thread_pool.t)

ADD_EXECUTABLE(random_generator.t random_generator.t.cpp)
TARGET_LINK_LIBRARIES(random_generator.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(random_generator.t)

ADD_EXECUTABLE(move.t move.t.cpp)
TARGET_LINK_LIBRARIES(move.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(move.t)

ADD_EXECUTABLE(move_list.t move_list.t.cpp)
TARGET_LINK_LIBRARIES(move_list.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(move_list.t)

ADD_EXECUTABLE(move_generator.t move_generator.t.cpp)
TARGET_LINK_LIBRARIES(move_generator.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(move_generator.t)

ADD_EXECUTABLE(move_finder.t move_finder.t.cpp)
TARGET_LINK_LIBRARIES(move_finder.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(move_finder.t)

ADD_EXECUTABLE(magics.t magics.t.cpp)
TARGET_LINK_LIBRARIES(magics.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(magics.t)

ADD_EXECUTABLE(game.t game.t.cpp)
TARGET_LINK_LIBRARIES(game.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(game.t)

ADD_EXECUTABLE(eval.t eval.t.cpp)
TARGET_LINK_LIBRARIES(eval.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(eval.t)

ADD_EXECUTABLE(board.t board.t.cpp)
TARGET_LINK_LIBRARIES(board.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(board.t)

ADD_EXECUTABLE(board_stack.t board_stack.t.cpp)
TARGET_LINK_LIBRARIES(board_stack.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(board_stack.t)

ADD_EXECUTABLE(algorithm_chess.t algorithm_chess.t.cpp)
TARGET_LINK_LIBRARIES(algorithm_chess.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(algorithm_chess.t)