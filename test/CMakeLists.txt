
macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} GTest::GTest GTest::Main)
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
endmacro()

if(PACKAGE_TESTS)
	package_add_test(lru_test lru_cache_test.cpp)
	package_add_test(snlru_test snlru_cache_test.cpp)
    add_executable(hit_rate hit_rate.cpp)

	package_add_test(intersection_test intersection_test.cpp)


	package_add_test(range_basic_test range_basic_test.cpp)

    add_executable(range_speed range_speed.cpp)
    target_link_libraries(range_speed benchmark::benchmark)
endif()

