#include <stdio.h>
#include <usart.h>
#include "jog.h"
#include "menu.h"
// #include "motor_control.h"

static enum jog_msg jog_sequence(void);
static enum jog_msg begin_state0(void);
static enum jog_msg seq_left_state1(void);
static enum jog_msg seq_right_state2(void); 
static enum jog_msg seq_done_state3(void);

static enum jog_msg (*jog_state)(void) = begin_state0;
static unsigned int cycle = 0;

enum commands jog1_state( void )
{
    enum commands command=c_null;
    enum jog_msg msg;
    unsigned char data_rec;
    static int jog_count=0;
    
    msg = jog_sequence(); // makes calls to sub states! in file jog.c
    switch(msg)
    {
      case j_null:
        break;
      case jog_spin_left_med:
        command = spin_left_med;
        break;
      case jog_spin_right_med:
        command = spin_right_med;
        break;
      case done:
        jog_count++;
        printf("\t...Jog1 sequence completed %i\n\r", jog_count);
		if (DataRdyUSART( )) 
        {
            data_rec = ReadUSART ();
            if ( (data_rec == 'u')||(data_rec == 'U') ) 
            {
                command = stop_command;
                menu_Mchar2();
            }
            else if (data_rec == '2') command = jog2;
        }    
        break;
      default:
		command = menu_system;
        printf("\t...bug: from jog1()\r\n");
        break;        
    }
    return (command);    
}


static enum jog_msg jog_sequence(void)
{
    enum jog_msg msg=j_null;
    
    if (cycle < 65534u) cycle++;
    else
    {
        cycle = 0;
        printf("cycle overflow -- jog_sequence\r\n");
    }   
    msg = jog_state();
    return (msg);
}   

static enum jog_msg begin_state0(void)
{
    enum jog_msg msg=j_null;
    
    if (cycle >= 2u)
    {
        cycle = 0;
        jog_state = seq_left_state1;
        msg = jog_spin_left_med;  // spin_left();
    }    
    return (msg);
}  

static enum jog_msg seq_left_state1(void) 
{
    enum jog_msg msg=j_null;
    
    if (cycle >= 1000u)
    {
        cycle = 0;
        jog_state = seq_right_state2;
        msg = jog_spin_right_med;
    }    
    return (msg);    
} 

static enum jog_msg seq_right_state2(void) 
{
    enum jog_msg msg=j_null;
    
    if (cycle >= 900u)
    {
        cycle = 0;
        jog_state = seq_done_state3;
//        brake_all();
    }    
    return (j_null);    
}  

 
static enum jog_msg seq_done_state3(void) 
{
    jog_state = begin_state0;
    cycle = 0;  
    return (done);    
}  