/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       ben                                                       */
/*    Created:      12/29/2025, 2:55:25 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;
void pre_auton(){ //set things like if pneumatics start actuated or not here and any variables that have to start at a certain position
  // float driveP, float driveI, float driveD, float driveMaxTime, float driveSettleTime, float driveSettleError, float driveMaxOutputVolts
  Chassis.set_drive_constants(2,0,100,5000,100,0.5,10);
  Chassis.set_turn_constants(0.35, 0.025, 30, 5000, 100, 2, 12);
  Chassis.pidUpdateTime = 10;
  // Chassis.set_turn_constants(.07, .15, 0, 5000, 150, 2,12);

}

void auton(){
  test();
  // Chassis.turn_to_angle(90); 
  // Chassis.turn_to_angle(180); 
  // Chassis.turn_to_angle(270); 
  // Chassis.turn_to_angle(0);
  // Chassis.drive_inches(24,90);
  // Chassis.turnMaxOutputVolts = 6;
  // Chassis.drive_inches(24, 90);
  // Chassis.drive_inches(12); 
  // Chassis.drive_inches(-36);

  // Chassis.turn_to_angle(90);
  // Chassis.driveMaxOutputVolts = 6; 
  // Chassis.drive_inches(10); 


  // Chassis.arcade(12.f, 12.f);
}

void usercontrol(){

  int storing = 0; 

  bool toggleR2 = true; 
  // drivePIDTuner(0.05, 0.001, 1);
  // headingPIDTuner(0.01, 0.001, 1);
  while(true){
    Chassis.arcade(12.f, 12.f);
    intakeLeft.spin(fwd, 12 * (controller(primary).ButtonL1.pressing() - controller(primary).ButtonR1.pressing()), volt); 
    intakeRight.spin(fwd, 12 * (controller(primary).ButtonL1.pressing() - controller(primary).ButtonR1.pressing()) * storing, volt); 

    if(controller(primary).ButtonR2.pressing()){
      if(toggleR2 == true){
        if (storing == 0){
          storing = 1; 
        } else if (storing == 1){
          storing = 0; 
        }
        toggleR2 = false; 
      }
    } else{
      toggleR2 = true; 
    }

    // intakeLeft.spin(fwd, 12, volt); 
    // intakeRight.spin(fwd, 12, volt); 
  }
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(auton);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}