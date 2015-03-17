#include <stdio.h>
#include "menu.h"

void menu_top(void)
{
    printf("\r\n   Kwantlen Polytechnic University\r\n");
    printf(  "             APSC1299\r\n");
    printf("   Robot Diagnostic\r\n");
    printf("   program by Dan Peirce B.Sc.\r\n");
    printf("   version 5.1 March 16, 2015\r\n");
    printf("\n\tTop Level Menu\n\r\n");
    
    printf("\t m => motor\r\n");
	printf("\t S => sensor\r\n");
    printf("\t ? or h => help print this menu\n\r\n>");
} 

void menu_Mchar2(void)  // motor menu 2nd character
{
    printf("\r\n\tL => left motor\n\r");
    printf("\tR => right motor\n\r");
    printf("\tB => brake all\n\r");
    printf("\tJ => Jog Sequence (check for reliable code execution)\n\r");
    printf("\tU => up to top menu\n\r\n>");
} 

void menu_Mchar3(void)  // motor menu 3rd character
{
    printf("\r\n\tF => fast speed\r\n");
    printf("\tM => medium speed\r\n");
    printf("\tS => slow speed\r\n");
    printf("\t< => reverse (at same speed)\r\n");
    printf("\t> => forward (at same speed)\r\n");
    printf("\tC => coast\r\n");
    printf("\tU => up one menu\n\r\n>");
    
}  

void menu_Schar2(void)      // sensor menu second character
{
    printf("\r\n\t1 => read sensor 1 (left)\r\n");
    printf("\t2 => read sensor 2 (left)\r\n");
    printf("\t3 => read sensor 3 (left)\r\n");
    printf("\t4 => read sensor 4 (left)\r\n");
    printf("\t5 => read sensor 5 (left)\r\n");
    printf("\tT => set the threshold value\r\n");
    printf("\tA => Automatic threshold set-up\r\n");
    printf("\t      slide robot perpendicular to line!\r\n");
    printf("\tU => up one menu\n\r\n>");
}
            
