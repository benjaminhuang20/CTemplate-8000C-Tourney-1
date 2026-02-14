#pragma once
#include "vex.h"
// #include "CTemplate/chassis.h"
using namespace vex; 

extern brain Brain;            //Main brain
extern controller Controller;  //Main controller
extern competition Competition; //What the comp uses to run functions like autonomous and usercontrol

//Left Chassis
extern motor leftFront; 
extern motor leftMiddle; 
extern motor leftBack;

//Right Chassis
extern motor rightFront;
extern motor rightMiddle; 
extern motor rightBack;

extern motor intakeLeft;
extern motor intakeRight; 

extern digital_out scraper; 
extern digital_out descoreLeft;
extern digital_out mid; 
extern digital_out descoreMid;

extern digital_out tripleStateOne;
extern digital_out tripleStateTwo;

class chassis; 
extern chassis Chassis;

extern bool auto_started; 
