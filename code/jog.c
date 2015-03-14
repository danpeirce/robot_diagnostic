#include <stdio.h>
#include "jog.h"
#include "motor_control.h"


enum jog_msg begin_state0(void);
enum jog_msg seq_left_state1(void);
enum jog_msg seq_right_state2(void); 
enum jog_msg seq_done_state3(void);

static enum jog_msg (*jog_state)(void) = begin_state0;
static unsigned int cycle = 0;

enum jog_msg jog_sequence(void)
{
    enum jog_msg msg=null;
    
    if (cycle < 65534u) cycle++;
    else
    {
        cycle = 0;
        printf("cycle overflow -- jog_sequence\r\n");
    }   
    msg = jog_state();
    return (msg);
}   

enum jog_msg begin_state0(void)
{
    enum jog_msg msg=null;
    
    if (cycle >= 2u)
    {
        cycle = 0;
        jog_state = seq_left_state1;
        spin_left();
    }    
    return (null);
}  

enum jog_msg seq_left_state1(void) 
{
    enum jog_msg msg=null;
    
    if (cycle >= 1000u)
    {
        cycle = 0;
        jog_state = seq_right_state2;
        spin_right();
    }    
    return (null);    
} 

enum jog_msg seq_right_state2(void) 
{
    enum jog_msg msg=null;
    
    if (cycle >= 900u)
    {
        cycle = 0;
        jog_state = seq_done_state3;
//        brake_all();
    }    
    return (null);    
}  

 
enum jog_msg seq_done_state3(void) 
{
    jog_state = begin_state0;
    cycle = 0;  
    return (done);    
}  