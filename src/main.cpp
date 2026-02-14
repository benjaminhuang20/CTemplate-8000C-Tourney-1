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
  while(!auto_started){
    Controller.Screen.clearScreen();
    Controller.Screen.setCursor(1, 1); 
    Controller.Screen.print("Battery: %d", Brain.Battery.capacity());
    Controller.Screen.setCursor(2, 1); 
    Controller.Screen.print("heading: %f",Chassis.get_heading());

    wait(10, msec); 
  }

  
  // Chassis.set_turn_constants(.07, .15, 0, 5000, 150, 2,12);
}

void auton(){
  auto_started = true; 
  // test();
  right4Ball();
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
  auto_started = true; 
  // drivePIDTuner(0.1, 0.01, 1); 

  
  int storing = 0;

  bool toggleR2 = true, toggleX = true, toggleY = true, toggleA = true;

  while(true){
    Chassis.arcade(12.f, 12.f);
    intakeLeft.spin(fwd, 120 * (controller(primary).ButtonL1.pressing() - controller(primary).ButtonR1.pressing()), volt); 
    intakeRight.spin(fwd, 120 * (controller(primary).ButtonL1.pressing() - controller(primary).ButtonR1.pressing()), volt); 

    if(controller(primary).ButtonR2.pressing()){
      if(toggleR2){
        if (storing == 0){//Long
          storing = 1;
          tripleStateOne = true; 
          tripleStateTwo = true; 
        }
        else if (storing == 1) //store
        {
          storing = 2;
          tripleStateOne = false; 
          tripleStateTwo = true; 
        }
        else if (storing == 2) //middle
        {
          storing = 0;
          tripleStateOne = false; 
          tripleStateTwo = false; 
        }
        toggleR2 = false; 
      }
    } else{
      toggleR2 = true; 
    }

    if(controller(primary).ButtonX.pressing()){
      if(toggleX){
        scraper = !scraper;
        toggleX = false;
      }
    } else{
      toggleX = true; 
    }

    if (controller(primary).ButtonL2.pressing()){
      descoreLeft = false; 
    } else{
      descoreLeft = true; 
    }

    if(controller(primary).ButtonY.pressing()){
        if(toggleY){
          descoreMid = !descoreMid;
          toggleY= false; 
        }
    } else{
      toggleY = true; 
    }

    if(controller(primary).ButtonA.pressing()){
        if(toggleA){
          mid = !mid;
          toggleA= false; 
        }
    } else{
      toggleA = true; 
    }
    wait(10, msec);
  }
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(auton);
  Competition.drivercontrol(usercontrol);
  // Competition.drivercontrol(drivePIDTuner);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}