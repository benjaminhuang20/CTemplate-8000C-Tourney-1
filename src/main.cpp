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

char *autoNames[2][8] = {{"4B_Corner", "4b_Matchload", "7B","7b_Split","9B","10B","Solo_AWP","Skills"},{"L1", "L2", "L3","L4","L5","L6","L7","L8"}};

void printAutonSelector(){
  Controller.Screen.clearScreen();
    Controller.Screen.setCursor(1, 1);
    Controller.Screen.print("Press L2 to drive");
    Controller.Screen.setCursor(2, 1);
    if(currentAuto >= 0 && currentAuto <= 7){
    Controller.Screen.print("Auto: %s", autoNames[currentAuto][currentSide]);
    }
}

int runs = 0;
void pre_auton()
{ // set things like if pneumatics start actuated or not here and any variables that have to start at a certain position
  Chassis.set_drive_constants(2, 0, 100, 5000, 100, 0.5, 10); 
  Chassis.set_turn_constants(0.49, 0.02, 32, 1500, 100, 2, 12);
  // Chassis.set_distance_constants(1.2, 0, 45, 5000, 100, 0.5, 4.5);
  // Chassis.set_distance_constants(1.2, 0, 45, 5000, 300, 0.5, 10);
  Chassis.set_distance_constants(2, 0, 100, 5000, 100, 0.5, 10); 
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
    
    if (controller(primary).ButtonRight.pressing())
    {
        currentSide = 1;
        printAutonSelector();
    }
    if (controller(primary).ButtonLeft.pressing())
    {
        currentSide = 0;
        printAutonSelector();
    }
      
    wait(10, msec);
  }

  //once the allow_selector variable is turned to false, it will return these quality of life values onto the screen. (Battery, Motor power)

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
  // switch(currentAuto){
  //   case(0):
  //     right4ballcorner();
  //   case(1):
  //     right4ballmatchload();
  //   case(2):
  //     right7ball();
  //   case(3):
  //     rightSplit();
  //   case(4):
  //     right9ball();
  //   case(5):
  //     right10ball();
  //   case(6):
  //     soloAWP_PUSH();
  //   case(7):
  //     skills();
  //   case(8):
  //     worseSkills();
  //   }
    // clearPark();

  Chassis.set_heading(90);
  Chassis.drive_inches_from_wall(50, 90, 1);
  Chassis.turn_to_angle(180);
  Chassis.drive_inches_from_wall(45, 180, 0);
  Chassis.turn_to_angle(45);
  Chassis.driveMaxOutputVolts = 6;
  Chassis.driveMaxTime = 750;
  Chassis.drive_inches(-20);
  
}

void usercontrol()
{
  auto_started = 2;

  int storing = 0;
  float intakeSpeed = 120.f;
  float storingSpeed = 3;

  bool toggleR2 = true, toggleX = true, toggleY = true, toggleA = true;
  int refreshTimer = 0; 

  while (true)
  {
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

    if (controller(primary).ButtonA.pressing())
    {
      mid = true;
      storing = 0;
      intakeSpeed = 12.f;
      storingSpeed = 6;
    }
    else
    {
      mid = false;
      intakeSpeed = 120.f;
      storingSpeed = 3;
    }

    wait(10, msec);
    refreshTimer += 10; 
  }
}

int main()
{
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(auton);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
