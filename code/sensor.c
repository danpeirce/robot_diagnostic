#include <stdio.h>
#include <adc.h>
#include "sumovore.h"
#include "sensor.h"

void read_sensor(unsigned char position);

void sensor_reading(enum commands command)
{
   
    switch(command)
    {
        case read_sensor_1:
          printf(" sensor 1 (left, RLS_LeftCH0),");
          read_sensor(RLS_LeftCH0);   // needs sumovore.h and adc.h
          break;
        case read_sensor_2:
          printf(" sensor 2 (center left, CntLeftCH1),");
          read_sensor(RLS_CntLeftCH1);
          break;
        case read_sensor_3:
          printf(" sensor 3 (center, RLS_CenterCH2),");
          read_sensor(RLS_CenterCH2);
          break;
        case read_sensor_4:
          printf(" sensor 4 (center right, RLS_CntRightCH3),");
          read_sensor(RLS_CntRightCH3);
          break;
        case read_sensor_5:
          printf(" sensor 5 (right, RLS_RightCH4),");
          read_sensor(RLS_RightCH4);
          break;
        default:
          printf("\r\ninvalid sensor position -- bug\r\n>");
          break;
    }    
}    

void read_sensor(unsigned char position)
{
   
    printf(" raw reading %u\r\n>", adc(position));
    
}  

int quiet_sensor_reading(unsigned char position)
{
    int value;
    
    switch(position)
    {
        case 1:
          value = adc(RLS_LeftCH0);   // needs sumovore.h and adc.h
          break;
        case 2:
          value = adc(RLS_CntLeftCH1);
          break;
        case 3:
          value = adc(RLS_CenterCH2);
          break;
        case 4:
          value = adc(RLS_CntRightCH3);
          break;
        case 5:
          value = adc(RLS_RightCH4);
          break;
        default:
          printf("\r\nquit sensor reading -- bug\r\n>");
          break; 
    }    
    return (value);
}  