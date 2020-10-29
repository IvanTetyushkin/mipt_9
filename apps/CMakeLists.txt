add_executable( cache_hello cache_hello.cpp)
#target_include_directories(cache_hello PUBLIC ${PROJECT_SOURCE_DIR}/lib)#cache_interface.hpp lru_cache.hpp snlru_cache.hpp)
#target_link_libraries(cache_hello INTERFACE Cache)

add_executable( intersection_hello intersection_hello.cpp)
#target_link_libraries(intersection_hello INTERFACE Intersection)
#target_include_directories(intersection_hello PUBLIC ${PROJECT_SOURCE_DIR}/lib)#cache_interface.hpp lru_cache.hpp snlru_cache.hpp)

add_executable( range_hello range_hello.cpp)
#target_link_libraries(range_hello INTERFACE Range)
#target_include_directories(range_hello PUBLIC ${PROJECT_SOURCE_DIR}/lib)#cache_interface.hpp lru_cache.hpp snlru_cache.hpp)
