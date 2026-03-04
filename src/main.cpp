/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       ben                                                       */
/*    Created:      12/29/2025, 2:55:25 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

float getDrivePower(motor check)
{
  int temp = (int)(check.temperature(vex::temperatureUnits::celsius));

  if (temp >= 70)
  {
    return 0.f;
  }
  else if (temp >= 65)
  {
    return 0.125;
  }
  else if (temp >= 60)
  {
    return 0.25;
  }
  else if (temp >= 55)
  {
    return 0.5;
  }
  else
  {
    return 1.f;
  }
}
// int batteryLock = 0;
int uiMode = 0;
// 0 = generic

using namespace vex;


char *autoNames[] = {"4B_Corner", "4b_Matchload", "7B","7b_Split","9B","10B","Solo_AWP","Skills"};

void printAutonSelector(){
  Controller.Screen.clearScreen();
    Controller.Screen.setCursor(1, 1);
    Controller.Screen.print("Press L2 to drive");
    Controller.Screen.setCursor(2, 1);
    if(currentAuto >= 0 && currentAuto <= 7){
    Controller.Screen.print("Auto: %s", autoNames[currentAuto]);
    }
}

int runs = 0;
void pre_auton()
{ // set things like if pneumatics start actuated or not here and any variables that have to start at a certain position
  // float driveP, float driveI, float driveD, float driveMaxTime, float driveSettleTime, float driveSettleError, float driveMaxOutputVolts
  // auto_started = false;
  
  Chassis.set_drive_constants(2, 0, 100, 5000, 100, 0.5, 10);
  // Chassis.set_turn_constants(0.225, 0.00, 16.5, 2000, 100, 2, 12);
  Chassis.set_turn_constants(0.45, 0.02, 32, 2000, 100, 2, 12);
  Chassis.set_distance_constants(1.2, 0, 45, 5000, 100, 0.5, 4.5);
  Chassis.pidUpdateTime = 10;

  bool toggleUp = true, toggleDown = true;
  descoreLeft = false;
  drawLogo();
 

  
  printAutonSelector();
  while (allow_selector == true)
  {
   

    if (controller(primary).ButtonUp.pressing())
    {
      if (toggleUp)
      {
        currentAuto = (currentAuto + 1) % 8;
        toggleUp = false;
        printAutonSelector();
      }
    }
    else
    {
      toggleUp = true;
    }

    if (controller(primary).ButtonDown.pressing())
    {
      if (toggleDown)
      {
        currentAuto--;
        if (currentAuto < 0){
          currentAuto = 7;
        }
        printAutonSelector();
          toggleDown = false;
      }
    }
    else
    {
      toggleDown = true;
    }
    if (controller(primary).ButtonL2.pressing())
    {
      allow_selector = false;
      break;
        }

    wait(10, msec);
  }

  while(true){
    Controller.Screen.clearScreen();

      Controller.Screen.setCursor(1, 1);
      Controller.Screen.print("Battery: %d", Brain.Battery.capacity());
      Controller.Screen.setCursor(2, 1);
      Controller.Screen.print("D.Pwr: %d", (int)((
                                                     getDrivePower(leftFront) +
                                                     getDrivePower(leftMiddle) +
                                                     getDrivePower(leftBack) +
                                                     getDrivePower(rightFront) +
                                                     getDrivePower(rightMiddle) +
                                                     getDrivePower(rightBack)) *
                                                 100 / 6));
      Controller.Screen.setCursor(2, 14);
      Controller.Screen.print("Int.: %d",
                              (int)((
                                        getDrivePower(intakeRight) +
                                        getDrivePower(intakeLeft)) *
                                    100 / 2));
      wait(200, msec);
  }

  // Chassis.set_turn_constants(.07, .15, 0, 5000, 150, 2,12);
}

void auton()
{
  auto_started = 1;
  switch(currentAuto){
    case(0):
      right4ballcorner();
    case(1):
      right4ballmatchload();
    case(2):
      right7ball();
    case(3):
      rightSplit();
    case(4):
      right9ball();
    case(5):
      right10ball();
    case(6):
      soloAWP_PUSH();
    case(7):
      skills();
  }
}

void usercontrol()
{
  // drawLogo();
  auto_started = 2;
  // drivePIDTuner(0.1, 0.01, 1);
  // headingPIDTuner(0.1, 0.01, 1);

  int storing = 0;
  float intakeSpeed = 120.f;
  float storingSpeed = 3;

  bool toggleR2 = true, toggleX = true, toggleY = true, toggleA = true;
  int refreshTimer = 0; 

  while (true)
  {
    if (!allow_selector && refreshTimer % 200 == 0)
    {
    
    }

    Chassis.arcade(12.f, 12.f);
    intakeLeft.spin(fwd, intakeSpeed * (controller(primary).ButtonL1.pressing() - controller(primary).ButtonR1.pressing()), volt);
    if (storing == 0)
    {
      intakeRight.spin(fwd, storingSpeed, volt);
    }
    else
    {
      intakeRight.spin(fwd, 120 * (controller(primary).ButtonL1.pressing() - controller(primary).ButtonR1.pressing()) * storing, volt);
    }

    if (controller(primary).ButtonR2.pressing())
    {
      if (toggleR2)
      {
        if (storing == 0)
        {
          storing = 1;
        }
        else
        {
          storing = 0;
        }

        toggleR2 = false;
      }
    }
    else
    {
      toggleR2 = true;
    }

    if (controller(primary).ButtonX.pressing())
    {
      if (toggleX)
      {
        scraper = !scraper;
        toggleX = false;
      }
    }
    else
    {
      toggleX = true;
    }

    if (controller(primary).ButtonL2.pressing())
    {
      descoreLeft = false;
    }
    else
    {
      if(auto_started == 2 && !allow_selector){
        descoreLeft = true;
      } else {
        descoreLeft = false; 
      }
      
    }

    // if(controller(primary).ButtonY.pressing()){
    //     if(toggleY){
    //       descoreMid = !descoreMid;
    //       toggleY= false;
    //     }
    // } else{
    //   toggleY = true;
    // }

    if (controller(primary).ButtonA.pressing())
    {
      mid = true;
      storing = 0;
      intakeSpeed = 12.f;
      // it was 10 before if u wanna change it back
      storingSpeed = 6;
      // toggleA = false;
      // if(toggleA){
      //   mid = !mid;
      //   storing = 0;
      //   toggleA = false;
      // }
    }
    else
    {
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
    refreshTimer += 10; 
  }
}

int main()
{
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(auton);
  // Competition.drivercontrol(yousuck);

  Competition.drivercontrol(usercontrol);
  // Competition.drivercontrol(drivePIDTuner);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
