# Robot Diagnostic

## Overview

The purpose of the Robot Diagnostic program is primarily to test the 
correct functioning of the different parts of a modified Solarbotics 
Sumovore robot. The students often add new code to their programs and 
find the robot stops working in ways they don't think are related to 
their new code. It is necessary to have a program that can test all 
aspects of the robot to ensure it works. The diagnostic program was 
designed to be easy to use by students, lab instructors and technicians.

The program was written to be used with a terminal program such as PuTTY,
minicom or hyperterminal. The robot is connected to a PC with a simple 
passive cable. The program uses a menu system that is easy to navigate.

## Compiler XC8 from Microchip

This project has been modified to use the Microchip XC8 compiler. It 
is currently being used with the MPlab IDE 8.xx but there are plans to
move to the MPlabX IDE. [Previous versions of the project](https://danpeirce.github.io/2010/robot_diagnostic.html) 
were used with the Microchip C18 compiler. Version control was not 
used for previous versions.

## Modified Solarbotics Sumovore Robot

The changes made to the sumovore are listed at: 
[https://danpeirce.github.io/sumovore/ChangesToSumovoreBB2.html](https://danpeirce.github.io/sumovore/ChangesToSumovoreBB2.html)

### Using PIC18F4525 in place of PIC16F877A

We use the PIC18F4525 as a drop in replacement for the stock PIC16F877A.
It has more program space and a deeper hardware stack making it more 
suitable for C programs.

### The Purpose of the Modifications

The purpose of the modifications are each for one of the following 
reasons:

1.  To make the **robot hardware more reliable**. Reliability is
    essential. It is virtually impossible for the students to have
    reasonable success modifying their programs in a resonable
    amount of time if the robot hardware does not behave
    consistently. The PIC has now being configured to watch for out
    for low voltage conditions.  
2.  To make in circuit programming with the PICkit2 possible.
3.  To make the robots as interchangeable as reasonably possible. A
    program should work equally well on any of the robots (to the
    extent reasonably possible).
4.  To simplify the task of building the robots (i.e. leaving off
    unnecessary components the robot was designed for the
    mini-sumo event and doubles for line following as a secondary
    purpose. We only do line following because it is a better
    programming task. 
5.  To use long lasting Lithium batteries. 
    1. The voltage from the lithium batteries is more consistent 
       than from alkaline batteries. 
    2. The robot speed is also more consistent. 
    3. This means the time of travel is more consistent.
    4. The motors are actually rated for six volts not nine volts. Using 
       four lithium batteries should extent the life of the motors.
6.  The use of a smaller main battery voltage requires a regulator for 
    the digital electronics that can maintain a regulated output even 
    when the battery voltage drops below six volts. This is why the 
    DC-DC converter is used in place of the linear regulator. 
7.  To avoid damaging the track electricians tape covers the bottom of 
    the scoop.
    
## Menu System

The top level menu has not changed from early versions of the program 
except for the version number and date displayed.

![](https://danpeirce.github.io/2010/top_menu1_1.gif)

The menu system was [programmed as a state machine](https://danpeirce.github.io/2010/MenuStateMachineDiagnosticProgram.html#State%20transition%20diagram). The overall 
state machine has not been modified from the last version of 2010.

