cmake_minimum_required(VERSION 3.10)
project(Mutex C)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES mutex.c)
set(SYLVANHOME ${PROJECT_SOURCE_DIR}/sylvan-1.2.0/src/)

add_executable(Mutex mutex.c ${SYLVANHOME}/lace.c ${SYLVANHOME}/sha2.c ${SYLVANHOME}/sylvan_bdd.c ${SYLVANHOME}/sylvan_cache.c
        ${SYLVANHOME}/sylvan_common.c ${SYLVANHOME}/sylvan_gmp.c ${SYLVANHOME}/sylvan_ldd.c ${SYLVANHOME}/sylvan_mt.c ${SYLVANHOME}/sylvan_mtbdd.c
        ${SYLVANHOME}/sylvan_refs.c ${SYLVANHOME}/sylvan_sl.c ${SYLVANHOME}/sylvan_stats.c ${SYLVANHOME}/sylvan_table.c)
