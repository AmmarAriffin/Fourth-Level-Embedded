add_tiva_executable(fitness-tracker
    adc_hal_tiva.c
    pot_measure.c
    buttons4.c
    circBufT.c
    i2c_driver.c
    serial_sender.c
    step_counter_main.c
    stopwatch.c
    switches.c
    timer_s.c
    temp_measure.c
    average.c
    accelerometer.c
    accl_hal_tiva.c
    state_distance.c
    state_setGoal.c
    state_temperature.c
    displayInterface.c
    pot_measure.c
    state.c
    stepCounter.c
    unitConverter.c
    Fitness_Tracker.c
)
target_compile_options(fitness-tracker PRIVATE -Wall)
target_link_libraries(fitness-tracker PRIVATE tivaware OrbitOLED freertos::heap-4 m)
add_program_target(fitness-tracker PROGRAM_TARGET program DEBUG_TARGET debug)
