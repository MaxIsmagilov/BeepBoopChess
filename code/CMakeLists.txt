
cmake_minimum_required(VERSION 3.25)

PROJECT( BeepBoop
    VERSION 0.0.0
    LANGUAGES C CXX
    )

ADD_SUBDIRECTORY(BeepBoop)

ADD_SUBDIRECTORY(exec)

ADD_SUBDIRECTORY(third_party)

if(NOT WIN32)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-lpthread" COMPILER_SUPPORTS_THREADS)

    if (COMPILER_SUPPORTS_THREADS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
    else ()
        message(FATAL_ERROR "Your compiler does not support C++ threads")
    endif ()
endif ()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
target_compile_options(gtest PRIVATE "-Wno-error=restrict")

enable_testing()
include(GoogleTest)
include(CTest)