

SET(BobChess_Headers
    utils.hpp
    move.hpp
    board.hpp
    random_generator.hpp
    magics.hpp
    )

SET(BobChess_Impl
    utils.cpp
    move.cpp
    board.cpp
    random_generator.cpp
    magics.cpp
    )

ADD_LIBRARY(BobChess_objects OBJECT 
    ${BobChess_Headers}  
    ${BobChess_Impl}
    )

TARGET_COMPILE_FEATURES(BobChess_objects
    PUBLIC cxx_std_20
    )

TARGET_INCLUDE_DIRECTORIES(BobChess_objects
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
    )

ADD_LIBRARY(BobChess $<TARGET_OBJECTS:BobChess_objects>)

ADD_LIBRARY(BobChess::BobChess ALIAS BobChess)

SET_TARGET_PROPERTIES( BobChess
    PROPERTIES
    CXX_EXTENSIONS NO
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
    