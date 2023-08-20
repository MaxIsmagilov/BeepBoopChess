

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


TARGET_COMPILE_FEATURES(BeepBoop_objects
    PRIVATE cxx_std_20
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
        COMPILE_OPTIONS "-Wall -Wextra -Werror"
        PUBLIC_HEADER "${BeepBoop_headers}")
else ()
    SET_TARGET_PROPERTIES( BeepBoop
        PROPERTIES
        CXX_EXTENSIONS -lpthread 
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        COMPILE_OPTIONS "-Wall -Wextra -Werror"
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

ADD_EXECUTABLE(move_list.t move_list.t.cpp)
TARGET_LINK_LIBRARIES(move_list.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(move_list.t)

ADD_EXECUTABLE(move_generator.t move_generator.t.cpp)
TARGET_LINK_LIBRARIES(move_generator.t BeepBoop::BeepBoop GTest::gtest_main)
gtest_discover_tests(move_generator.t)
