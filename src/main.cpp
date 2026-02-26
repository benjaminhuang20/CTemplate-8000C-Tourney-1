/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       ben                                                       */
/*    Created:      12/29/2025, 2:55:25 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"
int tLine;
float getDrivePower(motor check){
  tLine++;
  int temp = (int)(check.temperature(vex::temperatureUnits::celsius));
  Brain.Screen.setCursor(tLine, 1); 
  Brain.Screen.print(temp);
  if(temp >= 70){
    return 0.f;
  }else if(temp >= 65){
    return 0.125;
  }else if(temp >= 60){
    return 0.25;
  }else if(temp >= 55){
    return 0.5;
  }else{
    return 1.f;
  }
}
int batteryLock = 0;
using namespace vex;
void pre_auton(){ //set things like if pneumatics start actuated or not here and any variables that have to start at a certain position
  // float driveP, float driveI, float driveD, float driveMaxTime, float driveSettleTime, float driveSettleError, float driveMaxOutputVolts
  Chassis.set_drive_constants(2,0,100,5000,100,0.5,10);
  Chassis.set_turn_constants(0.35, 0.025, 30, 2000, 100, 2, 12);
  Chassis.pidUpdateTime = 10;
  
  bool toggleUp = true, toggleDown = true; 
  while (!auto_started)
  {
    // if(Brain.Battery.capacity() < 50 && batteryLock == 0){
    //   batteryLock = 1;
    //   Controller.rumble("...---...");
    // }
    //readouts
    tLine = 0;
    Brain.Screen.clearScreen();
    Controller.Screen.clearScreen();
    
    Controller.Screen.setCursor(1, 1); 
    Controller.Screen.print("Battery: %d", Brain.Battery.capacity());
    Controller.Screen.setCursor(2, 1); 
    Controller.Screen.print("D.Pwr: %d",(int)((
      getDrivePower(leftFront)+
      getDrivePower(leftMiddle)+
      getDrivePower(leftBack)+
      getDrivePower(rightFront)+
      getDrivePower(rightMiddle)+
      getDrivePower(rightBack))*100/6));
    Controller.Screen.setCursor(2, 14); 
    Controller.Screen.print("Int.: %d",
    (int)((
      getDrivePower(intakeRight)+
      getDrivePower(intakeLeft))*100/2)
    );
    Controller.Screen.setCursor(3, 1); 
    Controller.Screen.print("Auto: %d",currentAuto);

    if(controller(primary).ButtonUp.pressing()){
      if(toggleUp){
        currentAuto = (currentAuto + 1) % 8;
        toggleUp = false;
      }
    } else{
      toggleUp = true; 
    }

    if(controller(primary).ButtonDown.pressing()){
      if(toggleDown){
        currentAuto = (currentAuto - 1) % 8;
        toggleDown = false; 
      }
    } else{
      toggleDown = true; 
    }

    wait(10, msec);
  }

  // Chassis.set_turn_constants(.07, .15, 0, 5000, 150, 2,12);
}

void auton(){
  auto_started = true; 
  //rightSplit();
  //right7ball();
  //right9ball();
  //right4ball();
  //soloAWP_PUSH();
  SkillsControlMiddle();

  // switch(currentAuto){
  //   case(1):
  //     skills(); 
  //   case(2):
  //     skills(); 
  //   case(3):
  //     skills(); 
  //   case(4):
  //     skills(); 
  //   case(5):
  //     skills(); 
  //   case(6):
  //     skills(); 
  //   case(7):
  //     skills(); 
  //   case(8):
  //     skills(); 
  // }
  
  // test();
  // right4Ball();
  // rightLowerGoal();

  // Chassis.drive_inches_from_wall(10, 0);
  // Chassis.drive_inches(2);
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

void yousuck(){

}

void usercontrol(){
  
  auto_started = false; 
  // drivePIDTuner(0.1, 0.01, 1); 

  
  int storing = 0;
  float intakeSpeed = 120.f;
  float storingSpeed = 3; 

  bool toggleR2 = true, toggleX = true, toggleY = true, toggleA = true;

  while(true){
    Chassis.arcade(12.f, 12.f);
    intakeLeft.spin(fwd, intakeSpeed * (controller(primary).ButtonL1.pressing() - controller(primary).ButtonR1.pressing()), volt); 
    if(storing == 0){
      intakeRight.spin(fwd, storingSpeed, volt); 
    } else{
      intakeRight.spin(fwd, 120 * (controller(primary).ButtonL1.pressing() - controller(primary).ButtonR1.pressing()) * storing, volt);
    }
    
    
    

    if(controller(primary).ButtonR2.pressing()){
      if(toggleR2){
        if(storing == 0){
          storing = 1; 
        } else {
          storing = 0; 
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

    // if(controller(primary).ButtonY.pressing()){
    //     if(toggleY){
    //       descoreMid = !descoreMid;
    //       toggleY= false; 
    //     }
    // } else{
    //   toggleY = true; 
    // }

    if(controller(primary).ButtonA.pressing()){
      mid = true;
      storing = 0; 
      intakeSpeed = 12.f;
      //it was 10 before if u wanna change it back
      storingSpeed = 6;
      // toggleA = false;
      // if(toggleA){
      //   mid = !mid;
      //   storing = 0;
      //   toggleA = false;
      // }
    } else{
      mid = false;
      intakeSpeed = 120.f;
      storingSpeed = 3; 
      // storing = 1;
      // toggleA = true;
    }

    // if(controller(primary).ButtonA.pressing()){
    //     if(toggleA){
    //       if (storing == 0){ //long 
    //         storing = 2;
    //         intakeSpeed = 120.f;
    //         tripleStateOne = true;
    //         tripleStateTwo = true; 
    //       }
    //       else if (storing == 1) //storing goes to long
    //       {
    //         storing = 2;
    //         intakeSpeed = 120.f;
    //         tripleStateOne = true;
    //         tripleStateTwo = true; 
    //       }
    //       else if (storing == 2)
    //       { // middle
    //         storing = 0;
    //         intakeSpeed = 120.f;
    //         tripleStateOne = false; // middle triple state
    //         tripleStateTwo = false;
    //       }

    //       toggleA= false; 
    //     }
    // } else{
    //   toggleA = true; 
    // }
    wait(10, msec);
  }
}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(auton);
  // Competition.drivercontrol(yousuck);
  Competition.drivercontrol(usercontrol); 
  // Competition.drivercontrol(drivePIDTuner);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}