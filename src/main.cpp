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
    Controller.Screen.setCursor(3, 1);
    Controller.Screen.print("Heading: %f", Chassis.get_heading());
}

int runs = 0;
void pre_auton()
{ // set things like if pneumatics start actuated or not here and any variables that have to start at a certain position
  Chassis.set_drive_constants(2, 0, 100, 5000, 200, 0.5, 10, 0.5f); 
  // Chassis.set_turn_constants(0.45, 0.02, 60, 1000, 100, 2, 10);
  Chassis.set_turn_constants(0.40, 0.02, 38, 1000, 200, 2, 10, 1.f);
  // Chassis.set_turn_constants(0.47, 0.02, 40, 1500, 100, 2, 12);
  // Chassis.set_distance_constants(1.2, 0, 45, 5000, 100, 0.5, 4.5);
  // Chassis.set_distance_constants(1.2, 0, 45, 5000, 300, 0.5, 10);
  Chassis.set_distance_constants(2, 0, 100, 6000, 300, 0.3, 6, 0.5f); 
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
  // void rightSplit
  auto_started = 1;
  switch(currentAuto){
    case(0):
      right4ballcorner();
      break;
    case (1):
      right4ballmatchload();
      break;
    case (2):
      right7ball();
      // skills();
      break;
    case (3):
      // rightSplit();
      right7ball();
      break;
    case (4):
      leftSplit();
      break;
    case (5):
      right10ball();
      break;
    case (6):
      soloAWP_PUSH();
      break;
    case (7):
      // turn_test();
      worseSkills();
      //Chassis.drive_inches(2);
      // Chassis.set_heading(180);
      // RBCrossover();
      // Chassis.drive_inches_from_wall(18.25, 0, 0);
      // Chassis.turn_to_angle(-90);
      // Chassis.drive_inches(10);
      // Chassis.turn_to_angle(-90);
      // Chassis.drive_inches_from_wall(17.75, -90, 0);
      break;
    case (8):
      test();
      break;
    }
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
      intakeSpeed = 10.f;
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
