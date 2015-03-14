#ifndef COMMANDS
#define COMMANDS

enum commands
{
    c_null,
    motor_left_fast,
    motor_left_medium,
    motor_left_slow,
    motor_left_coast,
    motor_right_fast,
    motor_right_medium,
    motor_right_slow,
    motor_right_coast,
    motor_left_rev_fast,
    motor_left_rev_medium,
    motor_left_rev_slow,
    motor_left_rev_coast,
    motor_right_rev_fast,
    motor_right_rev_medium,
    motor_right_rev_slow,
    motor_right_rev_coast,
    motor_brake_all,
    spin_left_med,
    spin_right_med,
    spin_left_fast,
    spin_right_fast,
    boundary_top_motor,
    read_sensor_1,
    read_sensor_2,
    read_sensor_3,
    read_sensor_4,
    read_sensor_5,
    boundary_top_sensor,
    menu_system,
    jog1,
    jog2,
    stop_command,
    boundary_top_states
};
    
#endif
