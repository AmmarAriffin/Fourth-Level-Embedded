cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(FitnessTrackerTests LANGUAGES C)

enable_testing()
add_subdirectory(unity)

set(SRC_DIR ../target/src)
set(TIVAWARE_DIR ../target/libs/tivaware)
set(MOCK_DIR mocks)

add_unity_test(test_example test_example.c)
add_unity_test(test_fff test_fff.c)

add_unity_test(test_circular_buffer test_circular_buffer.c ${SRC_DIR}/circBufT.c)
target_include_directories(test_circular_buffer PRIVATE ${SRC_DIR})


add_unity_test(test_adc_hal test_adc_hal.c ${SRC_DIR}/adc_hal_tiva.c)
target_include_directories(test_adc_hal PRIVATE ${SRC_DIR})
target_include_directories(test_adc_hal PRIVATE ${TIVAWARE_DIR})
target_include_directories(test_adc_hal PRIVATE ${MOCK_DIR})

add_unity_test(test_accelerometer test_accelerometer.c ${SRC_DIR}/accelerometer.c ${SRC_DIR}/average.c ${SRC_DIR}/circBufT.c )
target_include_directories(test_accelerometer PRIVATE ${SRC_DIR})
target_include_directories(test_accelerometer PRIVATE ${MOCK_DIR})

add_unity_test(test_average test_average.c ${SRC_DIR}/average.c ${SRC_DIR}/circBufT.c)
target_include_directories(test_average PRIVATE ${SRC_DIR})

add_unity_test(test_temp_measure test_temp_measure.c ${SRC_DIR}/temp_measure.c)
target_include_directories(test_temp_measure PRIVATE ${SRC_DIR})
target_include_directories(test_temp_measure PRIVATE ${MOCK_DIR})
target_include_directories(test_temp_measure PRIVATE ${TIVAWARE_DIR})

# remove static from the definition of: 
# "static stopWatch_s stopWatch = {.lapNum = 0, .lapReadIndex = -1, .isRunning = false};" 
# in stopwatch.c for stopwatch tests to work
add_unity_test(test_stopwatch test_stopwatch.c ${SRC_DIR}/stopwatch.c)
target_include_directories(test_stopwatch PRIVATE ${SRC_DIR})
target_include_directories(test_stopwatch PRIVATE ${MOCK_DIR})
target_include_directories(test_stopwatch PRIVATE ${TIVAWARE_DIR})

add_unity_test(test_unitConverter test_unitConverter.c ${SRC_DIR}/unitConverter.c)
target_include_directories(test_unitConverter PRIVATE ${SRC_DIR})

add_unity_test(test_stepCounter test_stepCounter.c ${SRC_DIR}/stepCounter.c)
target_include_directories(test_stepCounter PRIVATE ${SRC_DIR})
target_include_directories(test_stepCounter PRIVATE ${MOCK_DIR})

# remove static from the definition of:
# "static timer_s *timerArray[NUM_TIMERS];"
# in timer_s.c for timer_s tests to work
add_unity_test(test_timer_s test_timer_s.c ${SRC_DIR}/timer_s.c)
target_include_directories(test_timer_s PRIVATE ${SRC_DIR})
target_include_directories(test_timer_s PRIVATE ${MOCK_DIR})
target_include_directories(test_timer_s PRIVATE ${TIVAWARE_DIR})



