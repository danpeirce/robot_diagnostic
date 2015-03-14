// code by Daniel Peirce B.Sc. at KPU
// rev. March 13, 2015 -- bug fix for sensor/threshold command
//      to avoid WDT reset

#include <stdio.h>
#include <xc.h>
#include <usart.h>
#include "process_command.h"
#include "sensor.h"
#include "jog.h"

#include "menu.h"

enum commands get_command_char1( unsigned char data );
enum commands get_command_Mchar2( unsigned char data );
enum commands get_command_MLchar3( unsigned char data ) ;
enum commands get_command_MRchar3( unsigned char data );
enum commands get_command_MLchar3_rev( unsigned char data ) ;
enum commands get_command_MRchar3_rev( unsigned char data );
enum commands get_command_Schar2( unsigned char data );
enum commands get_threshold( unsigned char data );
enum commands get_threshold_next( unsigned char data );
enum commands automatic_threshold( unsigned char data );
enum commands automatic_threshold_gap_right( unsigned char data );
enum commands automatic_threshold_gap_cright( unsigned char data );
enum commands automatic_threshold_gap_cleft( unsigned char data );
enum commands automatic_threshold_gap_left( unsigned char data );

void abortcheck(void);

unsigned char data_to_send = 0;

enum speed { fast, medium, slow, zero }; // current speed (without direction)
enum speed LMspeed=zero, RMspeed=zero;  // can be changed in more than one function!

static int min_diff[] = {1000,1000,1000,1000};
static int value[] = {0,0,0,0};
static enum commands (*get_command)(unsigned char data) = get_command_char1; // example of a 
                                                                    // pointer to a function
static char auto_mode =0; // auto mode means finding the threshold automatically while the robot
                          // is manually moved over a line
static int near_peak=700;

enum commands process_command(void)
{
    enum commands command=c_null;
    unsigned char data_rec = '\0' ;
    
    if( auto_mode ) get_command(0);  // in this case is using sensors to find threshold
    else
    {
        if (DataRdyUSART( )) data_rec = ReadUSART ();
        if ( data_rec != '\0' ) command = get_command(data_rec);
        else command = c_null;
    }
    return (command);       
} 

enum commands get_command_char1( unsigned char data )
{
    enum commands command=c_null;
    
    switch (data)
    {
      case 'm':
      case 'M':
        get_command = get_command_Mchar2; // motor
        menu_Mchar2();
        command = c_null;
        break;
      case 's':
      case 'S':
        get_command = get_command_Schar2;   // sensor
        menu_Schar2();
        command = c_null;
        break;
      case '?':
      case 'h':
      case 'H':
        menu_top();
        command = c_null;
        break;
        
      default:
        get_command = get_command_char1;
        command = c_null;
        menu_top();
        break; 
    }
    return (command);     
} 

enum commands get_command_Mchar2( unsigned char data ) 
{
    enum commands command=0;
        
    switch (data)    
    {
      case 'l':
      case 'L':
        get_command = get_command_MLchar3;
        menu_Mchar3();
        printf("\tLeft Motor\n\r>");
        command = 0;
        break;
      case 'r':
      case 'R':
        get_command = get_command_MRchar3;
        menu_Mchar3();
        printf("\tRight Motor\n\r>");
        command = 0;
        break;
      case 'b':
      case 'B':
        command = motor_brake_all;
        printf("brake all\r\n\t>");
        break;
      case 'j':
      case 'J':
        command = jog1;
        printf("running jog sequence: wait;\r\n");  
        break;
      case 'u':
      case 'U':
        get_command = get_command_char1;
        command = 0;
        menu_top();
        break;
      default:
        get_command = get_command_char1;
        command = 0;
        printf(" what? \r\n\n>");
        break;
    }
    return (command);    
}  

enum commands get_command_Schar2( unsigned char data )
{
    enum commands command=c_null;
    char gap;
    
    switch(data)
    {
      case '1':
        printf("read sensor 1\r\n>");
        command = read_sensor_1;   // left most
        break;
      case '2':
        printf("read sensor 2\r\n>");
        command = read_sensor_2;
        break;
      case '3':
        printf("read sensor 3\r\n>");
        command = read_sensor_3;
        break;
      case '4':
        printf("read sensor 4\r\n>");
        command = read_sensor_4;
        break;
      case '5':
        printf("read sensor 5\r\n>");
        command = read_sensor_5;   //right most
        break;
      case 'T':
      case 't':
        printf("update threshold\r\n old value was = %u\r\n", threshold);
        printf("enter threshold value 1 to 1023 >");
        get_command = get_threshold;
        break;
      case 'a':
      case 'A':
        printf("automatic threshold watching for input....\r\n"); 
        for(gap=0;gap<4;gap++)  // initalize arrays for this attempt
        {
            min_diff[gap] = 1000;
            value[gap] = 0;
        }    
        get_command = automatic_threshold;
        auto_mode=1;
        break;
      case 'u':
      case 'U':
        get_command = get_command_char1;
        command = 0;
        menu_top();
        break;
      default:
        printf("what? -- \r\n\n>");
    }
    return (command);    
}    

enum commands get_command_MLchar3( unsigned char data ) 
{
    enum commands command=0;

    
    switch (data)
    {
      case 'f':
      case 'F':
        printf("left motor fast speed\r\n>");
        LMspeed = fast;
        command = motor_left_fast;
        break;
      case 'm':
      case 'M':
        printf("left motor medium speed\r\n>");
        LMspeed = medium;
        command = motor_left_medium;
        break;
      case 's':
      case 'S':
        printf("left motor slow speed\r\n>");
        LMspeed = slow;
        command = motor_left_slow;
        break; 
      case 'C':
      case 'c':
        printf("left motor coast\r\n>");
        LMspeed = zero;
        command = motor_left_coast;
        break;
      case '<':
        printf("left motor reverse ");
        switch (LMspeed)
        {
          case fast:
            command = motor_left_rev_fast;
            printf("fast\r\n>");
            break;
          case medium:
            command = motor_left_rev_medium;
            printf("medium\r\n>");
            break;
          case slow:
            command = motor_left_rev_slow;
            printf("slow\r\n>");
            break;
          case zero:
            command = motor_left_rev_coast;
            printf("coast\r\n>");
            break;
        }
        get_command = get_command_MLchar3_rev;
        break; 
      case '>':
        printf("Left motor forward -- no change\r\n");
        break;
      case 'u':
      case 'U':
        get_command = get_command_Mchar2; 
        menu_Mchar2();
        break;
      default:
        command = 0;        // invalid character
        printf(" what? \r\n\n");
        menu_Mchar3();
        break;
    }    
    return (command);
} 

enum commands get_command_MLchar3_rev( unsigned char data ) 
{
    enum commands command=0;

    
    switch (data)
    {
      case 'f':
      case 'F':
        printf("left motor fast speed reverse\r\n>");
        LMspeed = fast;
        command = motor_left_rev_fast;
        break;
      case 'm':
      case 'M':
        printf("left motor medium speed reverse\r\n>");
        LMspeed = medium;
        command = motor_left_rev_medium;
        break;
      case 's':
      case 'S':
        printf("left motor slow speed reverse\r\n>");
        LMspeed = slow;
        command = motor_left_rev_slow;
        break; 
      case 'C':
      case 'c':
        printf("left motor coast reverse\r\n>");
        LMspeed = zero;
        command = motor_left_rev_coast;
        break;
      case '>':
        printf("left motor forward ");
        switch (LMspeed)
        {
          case fast:
            command = motor_left_fast;
            printf("fast\r\n>");
            break;
          case medium:
            command = motor_left_medium;
            printf("medium\r\n>");
            break;
          case slow:
            command = motor_left_slow;
            printf("slow\r\n>");
            break;
          case zero:
            command = motor_left_coast;
            printf("coast\r\n>");
            break;
        }
        get_command = get_command_MLchar3;
        break; 
      case '<':
        printf("Left motor reverse -- no change\r\n");
        break;
      case 'u':
      case 'U':
        get_command = get_command_Mchar2; 
        menu_Mchar2();
        break;
      default:
        command = 0;
        printf(" what? \r\n\n");   // invalid character
        menu_Mchar3();
        break;
    }    
    return (command);
} 
 
enum commands get_command_MRchar3( unsigned char data ) 
{
    enum commands command=0;
    
    switch (data)
    {
      case 'f':
      case 'F':
        printf("Right motor forward fast\r\n>");
        RMspeed = fast;
        command = motor_right_fast;
        break;
      case 'm':
      case 'M':
        printf("Right motor forward medium\r\n>");
        RMspeed = medium;
        command = motor_right_medium;
        break;
      case 's':
      case 'S':
        printf("right motor forward slow\r\n>");
        RMspeed = slow;
        command = motor_right_slow;
        break; 
      case 'C':
      case 'c':
        printf("Right motor coast\r\n>");
        RMspeed = zero;
        command = motor_right_coast;
        break; 
      case '<':
        printf("right motor reverse ");
        switch (RMspeed)
        {
          case fast:
            command = motor_right_rev_fast;
            printf("fast\r\n>");
            break;
          case medium:
            command = motor_right_rev_medium;
            printf("medium\r\n>");
            break;
          case slow:
            command = motor_right_rev_slow;
            printf("slow\r\n>");
            break;
          case zero:
            command = motor_right_rev_coast;
            printf("coast\r\n>");
            break;
          default:
            printf("RMspeed bug\r\n>");
            command = motor_right_rev_fast;
            break;  
        }
        get_command = get_command_MRchar3_rev;
        break; 
      case '>':
        printf("Right motor forward -- no change\r\n");
        break;
      case 'u':
      case 'U':
        get_command = get_command_Mchar2; 
        menu_Mchar2();
        break; 
      default:
        printf(" what? -- \r\n\n");  // invalid character
        menu_Mchar3();
        break;
    }    
    return (command);
}      

enum commands get_command_MRchar3_rev( unsigned char data ) 
{
    enum commands command=0;
    
    switch (data)
    {
      case 'f':
      case 'F':
        printf("Right motor reverse fast\r\n>");
        RMspeed = fast;
        command = motor_right_rev_fast;
        break;
      case 'm':
      case 'M':
        printf("Right motor reverse medium\r\n>");
        RMspeed = medium;
        command = motor_right_rev_medium;
        break;
      case 's':
      case 'S':
        printf("right motor reverse slow\r\n>");
        RMspeed = slow;
        command = motor_right_slow;
        break; 
      case 'C':
      case 'c':
        printf("Right motor coast\r\n>");
        RMspeed = zero;
        command = motor_right_rev_coast;
        break; 
      case '>':
        printf("right motor forward ");
        switch (RMspeed)
        {
          case fast:
            command = motor_right_fast;
            printf("fast\r\n>");
            break;
          case medium:
            command = motor_right_medium;
            printf("medium\r\n>");
            break;
          case slow:
            command = motor_right_slow;
            printf("slow\r\n>");
            break;
          case zero:
            command = motor_right_coast;
            printf("coast\r\n>");
            break;
          default:
            printf("RMspeed bug\r\n>");  // should never get here if no bugs
            command = motor_right_fast;
            break;
        }
        get_command = get_command_MRchar3;
        break; 
      case '<':
        printf("Right motor reverse -- no change\r\n");
        break;
      case 'u':
      case 'U':
        get_command = get_command_Mchar2; 
        menu_Mchar2();
        break; 
      default:
        printf(" what? -- \r\n\n");
        menu_Mchar3();
        break;
    }    
    return (command);
}      


enum commands get_threshold( unsigned char data )
{

    if ( (data >= '0') && (data <= '9') )
    {
        printf("%c",data);
        threshold = data-'0';
        get_command = get_threshold_next;
    }
    else 
    {
        printf("\r\n\n\t** entry error **\r\n\n"); 
        ReadUSART ();           // flush read buffer
        ReadUSART (); 
        printf("\n\renter threshold value 1 to 1023 >");
    }    
       
    return (c_null);
}

enum commands get_threshold_next( unsigned char data )
{
    if (threshold < 1024u)
    {
        if ( (data >= '0') && (data <= '9') )   // is the new character a digit?
        {
            printf("%c",data);
            threshold = threshold*10+data-'0';
        }
        else   // not a digit (take as a delimiter like enter)
        {
            if (threshold > 0u )  // new threshold is in valid range
            {
                printf("\r\nThe new threshold value is %u\r\n>",threshold);
                ReadUSART ();           // flush read buffer
                ReadUSART (); 
                get_command = get_command_Schar2;  // done back to Sensor command menu!
                ClrWdt();
                menu_Schar2();  // display menu
            }
            else   // threshold is zero, try again
            {
                printf("\r\n\n\t** entry error ** threshold must be > 0\r\n\n");
                ReadUSART ();           // flush read buffer
                ReadUSART (); 
                printf("update threshold\r\n old value was = %u\r\n", threshold);
                printf("enter threshold value 1 to 1023 >"); 
                threshold = 510u;
                get_command = get_threshold;
            }        
        }    
    }
    else    // threshold is >= 1024 and not valid, try again
    {
        printf("\r\n\n\t** entry error ** threshold must be < 1024\r\n\n");
        ReadUSART ();           // flush read buffer
        ReadUSART ();  
        threshold = 510u;
        printf("update threshold\r\n old value was = %u\r\n", threshold);
        printf("enter threshold value 1 to 1023 >");
        get_command = get_threshold; 
    }    
    
  
    return (c_null);
}

enum commands automatic_threshold( unsigned char data )
{
    if (quiet_sensor_reading(5)>near_peak) get_command = automatic_threshold_gap_right;

    abortcheck();

    return c_null;
}    


enum commands automatic_threshold_gap_right( unsigned char data )
{
    
    int sensor_reading[]={0,0};
    int difference;
    
    unsigned char position;
    
    sensor_reading[1] = quiet_sensor_reading(5);
    sensor_reading[0] = quiet_sensor_reading(4);

    if( sensor_reading[1]>sensor_reading[0])
    {
        difference = sensor_reading[1] - sensor_reading[0];
        if (difference < min_diff[3])
        {
            min_diff[3] = difference;
            value[3] = sensor_reading[0];
        }    
    } 
    else
    {
        difference = sensor_reading[0] - sensor_reading[1];
        if (difference < min_diff[3])
        {
            min_diff[3] = difference;
            value[3] = sensor_reading[1];
        }    
    }   
            

    if (quiet_sensor_reading(4)>near_peak) 
    {
        get_command = automatic_threshold_gap_cright;
        
            /*
            for( position=2;position<=4u;position++)
            {
                if(value[position-1]>value[0]) value[0] = value[position-1];
            }
             */  
    }
    abortcheck();    
    return c_null;    
     
}
  
enum commands automatic_threshold_gap_cright( unsigned char data )
{
   
    int sensor_reading[]={0,0};
    int difference;
    
    unsigned char position;
    
    sensor_reading[1] = quiet_sensor_reading(4);
    sensor_reading[0] = quiet_sensor_reading(3);

    if( sensor_reading[1]>sensor_reading[0])
    {
        difference = sensor_reading[1] - sensor_reading[0];
        if (difference < min_diff[2])
        {
            min_diff[2] = difference;
            value[2] = sensor_reading[0];
        }    
    } 
    else
    {
        difference = sensor_reading[0] - sensor_reading[1];
        if (difference < min_diff[2])
        {
            min_diff[2] = difference;
            value[2] = sensor_reading[1];
        }    
    }   
            

    if (quiet_sensor_reading(3)>near_peak) 
    {
        get_command = automatic_threshold_gap_cleft;  
    }
    
    abortcheck();    
    return c_null;    
     
}    

enum commands automatic_threshold_gap_cleft( unsigned char data )
{
    
    int sensor_reading[]={0,0};
    int difference;
    
    unsigned char position;
    
    sensor_reading[1] = quiet_sensor_reading(3);
    sensor_reading[0] = quiet_sensor_reading(2);

    if( sensor_reading[1]>sensor_reading[0])
    {
        difference = sensor_reading[1] - sensor_reading[0];
        if (difference < min_diff[1])
        {
            min_diff[1] = difference;
            value[1] = sensor_reading[0];
        }    
    } 
    else
    {
        difference = sensor_reading[0] - sensor_reading[1];
        if (difference < min_diff[1])
        {
            min_diff[1] = difference;
            value[1] = sensor_reading[1];
        }    
    }   
            

    if (quiet_sensor_reading(2)>near_peak) 
    {
        get_command = automatic_threshold_gap_left;  
    }
    
    abortcheck();    
    return c_null;    
     
}    

enum commands automatic_threshold_gap_left( unsigned char data )
{

    
    int sensor_reading[]={0,0};
    int difference;
    char gap;
    
    unsigned char position;
    
    sensor_reading[1] = quiet_sensor_reading(2);
    sensor_reading[0] = quiet_sensor_reading(1);

    if( sensor_reading[1]>sensor_reading[0])
    {
        difference = sensor_reading[1] - sensor_reading[0];
        if (difference < min_diff[0])
        {
            min_diff[0] = difference;
            value[0] = sensor_reading[0];
        }    
    } 
    else
    {
        difference = sensor_reading[0] - sensor_reading[1];
        if (difference < min_diff[0])
        {
            min_diff[0] = difference;
            value[0] = sensor_reading[1];
        }    
    }   
            

    if (quiet_sensor_reading(1)>near_peak) 
    {
        for (gap=1;gap<4;gap++) 
        {
            if(value[0] > value[gap]) value[0]= value[gap]; 
        }      
        printf("update threshold\r\n old value was = %u\r\n", threshold);
        threshold = value[0] -20;
        printf("\r\nThe new threshold value is %u\r\n>",threshold);
        ReadUSART ();           // flush read buffer
        ReadUSART (); 
        get_command = get_command_Schar2;  // done back to Sensor command menu!
        ClrWdt();
        menu_Schar2();  // display menu 
        auto_mode = 0; 
    }
    else abortcheck();
    return c_null;    
     
} 

void abortcheck(void)
{
    char data_rec;
    
    if (DataRdyUSART( )) 
    {  
        data_rec = ReadUSART ();
        ReadUSART ();           // flush read buffer
        ReadUSART ();
        printf("%c\r\n", data_rec);
        printf("\r\n abborted automatic thesehold detection\r\n"); 
        ClrWdt();
        menu_Schar2();  // display menu 
        auto_mode=0;
    }
}

    
