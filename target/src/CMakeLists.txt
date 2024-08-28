add_tiva_executable(fitness-tracker
    accelerometer.c
    adc_hal_tiva.c
    accl_hal_tiva.c
    average.c
    buttons4.c
    circBufT.c
    core.c
    displayInterface.c
    Fitness_Tracker.c
    i2c_driver.c
    main.c
    pot_measure.c
    serial_sender.c
    state_distance.c
    state_setGoal.c
    state_setTimer.c
    state_stopwatch.c
    state_temperature.c
    state_timer.c
    state.c
    stepCounter.c
    stopwatch.c
    switches.c
    temp_measure.c
    timer_s.c
    unitConverter.c
    state_calorie_counter.c
    calorie_counter.c
)
target_compile_options(fitness-tracker PRIVATE -Wall)
target_link_libraries(fitness-tracker PRIVATE tivaware OrbitOLED freertos::heap-4 m)
add_program_target(fitness-tracker PROGRAM_TARGET program DEBUG_TARGET debug)
