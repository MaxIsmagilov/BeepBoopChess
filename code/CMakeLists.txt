
cmake_minimum_required(VERSION 3.25)

PROJECT( BobChess
    VERSION 0.0.0
    LANGUAGES C CXX
    )

ADD_SUBDIRECTORY(BobChess)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-fconcepts" COMPILER_SUPPORTS_CONCEPTS)

if (COMPILER_SUPPORTS_CONCEPTS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts")
else ()
    message(FATAL_ERROR "Your compiler does not support C++20 concepts.")
endif ()


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()
include(GoogleTest)
include(CTest)