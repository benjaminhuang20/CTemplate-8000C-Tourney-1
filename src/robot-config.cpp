/*
    Place global variables here (variables that can be read from any file)
    This means that any motors, booleans, and brains should be defined here. 
    Explanations of each of these can be found in robot-config.h
*/

#include "vex.h"

brain Brain;
controller Controller;
competition Competition; 

motor leftFront = motor(PORT5, ratio6_1, false);
motor leftMiddle = motor(PORT13, ratio6_1, true);
motor leftBack = motor(PORT14, ratio6_1, false);

motor rightFront = motor(PORT17, ratio6_1, true);
motor rightMiddle = motor(PORT16, ratio6_1, false);
motor rightBack = motor(PORT3, ratio6_1, true);

motor intakeLeft = motor(PORT19, ratio6_1, true); 
motor intakeRight = motor(PORT20, ratio6_1, false);

digital_out scraper = digital_out(Brain.ThreeWirePort.C); 
digital_out descoreLeft = digital_out(Brain.ThreeWirePort.A); 
digital_out mid = digital_out(Brain.ThreeWirePort.B);

//swap if not work
// digital_out descoreMid = digital_out(Brain.ThreeWirePort.C);

chassis Chassis = chassis(
    /*
        DO NOT do something like
        motor_group left = motor_group(leftFront, leftMiddle, leftBack);
        and then put that into the chassis. This will lead to a memory error.
        This holds for everything that you place into this class
        Instead place the motors into the chassis manually like shown below:
        The first motor_group is the left side of your chassis, and the second is the right.
    */
    motor_group(leftFront, leftMiddle, leftBack),
    motor_group(rightFront, rightMiddle, rightBack),

    /*
        This is your gyroscope port
    */
    inertial(PORT12),

    /*
        Vex Gyroscopes usually consistently drift. So this means that for every rotation it will be some number of degrees off, and that number of degrees is consistent.
        The gyro scale shows how much the the true heading changes every time the gyro reads 360 degrees. It is shown below:
    */
    352.8f,

    /*
        Wheel Diameter and RPM are below. RPM is calculated by driver/driven * cartridge
    */
    3.25,
    0.75,

    /*
        Vertical and Horizontal odom wheel
    */
    rotation(PORT1),
  
    0,

    // LEFT distance sensor, right distance sensor, front distance sensor, back distance sensor
    vex::distance(PORT1), //L
    vex::distance(PORT1), //R
    vex::distance(PORT8), //F
    vex::distance(PORT11) //B
);

int auto_started = 0;
bool allow_selector = true;
int currentAuto =7;
int currentSide = 0;
float autoBottomIntakeSpeed;
float autoTopIntakeSpeed; 
// Motor is PORT, CARTRIDGE (blue is ratio6_1), reversed or not