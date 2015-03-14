#include <stdio.h>
#include <xc.h>
#include "sumovore.h"
#include "motor_control.h"
#include "sensor.h"
#include "process_command.h"
#include "menu.h"
#include "interrupts.h"
#include "jog.h"

void new_state(enum commands command);
enum commands stop_state( void );

static enum commands (*diagnostic_state)(void);

// main acts as a cyclical task sequencer
void main(void)
{
    enum commands command = 0;
    
    initialization(); // function from sumovore.c
                      // it sets up pwm (using timer2),
                      // IO pins, the ADC, the 
                      // USART and the default
                      // threshold 

    
    printf("\a\f"); // audible bell on the terminal
                    // and form feed
    menu_top();
    diagnostic_state = process_command;

    while(1)
    {
        check_sensors();   // from sumovore.c
        set_leds();
        ClrWdt();  // defined in <p18f4525.h>
        if(lvd_flag_set())  LVtrap();

	    command = diagnostic_state();  // from process_command.c

	    if (command == c_null); // if null skip the rest
	    else if(command < boundary_top_motor) motor_control(command); // function from motor_control.c
	    else if (command < boundary_top_sensor) sensor_reading(command);  //function from sensor.c
        else if ( command < boundary_top_states ) new_state( command );
	    else printf("\r\c command not valid -- bug main.c -- msg from function main\r\c");
    }
}

enum commands stop_state( void )
{
    motors_brake_all();
    return (menu_system);
}    

void new_state(enum commands command)
{
    if( command == jog1) diagnostic_state = jog1_state;
    else if( command == jog2) diagnostic_state = jog2_state;
    else if( command == menu_system) diagnostic_state = process_command;
    else if( command == stop_command ) diagnostic_state = stop_state;
    else printf("\r\c command not valid -- bug main.c -- msg from function new_state\r\c");
}
 