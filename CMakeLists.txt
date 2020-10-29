cmake_minimum_required( VERSION 3.8...3.15)
project( mipt_9 VERSION 0.001
    DESCRIPTION "Hi there"
                   LANGUAGES CXX)

set(CMAKE_CXX_STANDART 20)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MT> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
        )
endif()


include_directories(${PROJECT_SOURCE_DIR}/lib)#cache_interface.hpp lru_cache.hpp snlru_cache.hpp)
add_subdirectory(apps)


option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    set( gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
    find_package(GTest REQUIRED)
    find_package(benchmark REQUIRED)
    add_subdirectory(test)
endif()
