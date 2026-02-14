#include "vex.h"

float reduce_heading(float heading){
    float mod = 360; 
    return heading - floor(heading/mod) * mod; 
}

float clamp(float lower, float toClamp, float higher){
    if(toClamp < lower){
        return lower; 
    }
    if(toClamp > higher){
        return higher; 
    }
    return toClamp; 
}

int clamp(int lower, int toClamp, int higher){
    if(toClamp < lower){
        return lower; 
    }
    if(toClamp > higher){
        return higher; 
    }
    return toClamp; 
}

float deadband(int num, float deadRange){
    if(num > 0 && num < deadRange){
        return 0.f; 
    }

    if(num < 0 && num > deadRange){
        return 0.f;
    }

    return num; 
}

int drivePIDTuner(float Pincrement, float Iincrement, int Dincrement){
    bool toggleA, toggleLeft, toggleRight, toggleUp, toggleDown;
    int arrowPosition = 2;
    while (true)
    {
        Controller.Screen.clearScreen();
        Controller.Screen.setCursor(1, 1); 
        Controller.Screen.print("P : %f", Chassis.driveP);
        Controller.Screen.setCursor(2, 1); 
        Controller.Screen.print("I : %f", Chassis.driveI);
        Controller.Screen.setCursor(3, 1); 
        Controller.Screen.print("D : %f", Chassis.driveD);
        Controller.Screen.setCursor(arrowPosition, 15); 
        Controller.Screen.print("<");

        if(controller(primary).ButtonUp.pressing()){
            if(toggleUp){
                toggleUp = false;
                arrowPosition = clamp(1, arrowPosition - 1, 3); 
            }
        } else {
            toggleUp = true; 
        }

        if(controller(primary).ButtonDown.pressing()){
            if(toggleDown){
                toggleDown = false;
                arrowPosition = clamp(1, arrowPosition + 1, 3); 
            }
        } else {
            toggleDown = true; 
        }

        if(controller(primary).ButtonLeft.pressing()){
            if(toggleLeft){
                toggleLeft = false;
                if(arrowPosition == 2){
                    Chassis.driveP -= Pincrement; 
                } else if(arrowPosition == 3){
                    Chassis.driveI -= Iincrement; 
                } else if (arrowPosition == 4){
                    Chassis.driveD -= Dincrement;
                }
            }
        } else {
            toggleLeft = true; 
        }

        if(controller(primary).ButtonRight.pressing()){
            if(toggleRight){
                toggleRight = false;
                if(arrowPosition == 1){
                    Chassis.driveP += Pincrement; 
                } else if(arrowPosition == 2){
                    Chassis.driveI += Iincrement; 
                } else if (arrowPosition == 3){
                    Chassis.driveD+= Dincrement;
                }
            }
        } else {
            toggleRight = true; 
        }

        if(controller(primary).ButtonA.pressing()){
            if(toggleA){
                toggleA = false;
                Chassis.drive_inches(36); 
                Chassis.drive_inches(-24); 
                Chassis.drive_inches(-6); 
                Chassis.drive_inches(-6);

                Chassis.drive_inches(-36); 
                Chassis.drive_inches(30); 
                Chassis.drive_inches(6); 
            }
        } else {
            toggleA = true; 
        }
        wait(10, msec); 
    }

    return 0; 
}

int headingPIDTuner(float Pincrement, float Iincrement, int Dincrement){
    bool toggleA, toggleLeft, toggleRight, toggleUp, toggleDown;
    int arrowPosition = 2;
    while (true)
    {
        Controller.Screen.clearScreen();
        Controller.Screen.setCursor(1, 1); 
        Controller.Screen.print("P : %f", Chassis.turnP);
        Controller.Screen.setCursor(2, 1); 
        Controller.Screen.print("I : %f", Chassis.turnI);
        Controller.Screen.setCursor(3, 1); 
        Controller.Screen.print("D : %f", Chassis.turnD);
        Controller.Screen.setCursor(arrowPosition, 15); 
        Controller.Screen.print("<");

        if(controller(primary).ButtonUp.pressing()){
            if(toggleUp){
                toggleUp = false;
                arrowPosition = clamp(1, arrowPosition - 1, 3); 
            }
        } else {
            toggleUp = true; 
        }

        if(controller(primary).ButtonDown.pressing()){
            if(toggleDown){
                toggleDown = false;
                arrowPosition = clamp(1, arrowPosition + 1, 3); 
            }
        } else {
            toggleDown = true; 
        }

        if(controller(primary).ButtonLeft.pressing()){
            if(toggleLeft){
                toggleLeft = false;
                if(arrowPosition == 1){
                    Chassis.turnP -= Pincrement; 
                } else if(arrowPosition == 2){
                    Chassis.turnI -= Iincrement; 
                } else if (arrowPosition == 3){
                    Chassis.turnD -= Dincrement;
                }
            }
        } else {
            toggleLeft = true; 
        }

        if(controller(primary).ButtonRight.pressing()){
            if(toggleRight){
                toggleRight = false;
                if(arrowPosition == 1){
                    Chassis.turnP += Pincrement; 
                } else if(arrowPosition == 2){
                    Chassis.turnI += Iincrement; 
                } else if (arrowPosition == 3){
                    Chassis.turnD+= Dincrement;
                }
            }
        } else {
            toggleRight = true; 
        }

        if(controller(primary).ButtonA.pressing()){
            if(toggleA){
                toggleA = false;
                Chassis.turn_to_angle(90); 
                Chassis.turn_to_angle(180); 
                Chassis.turn_to_angle(270); 
                Chassis.turn_to_angle(0); 

                Chassis.turn_to_angle(45); 
                Chassis.turn_to_angle(90); 
                Chassis.turn_to_angle(180); 
                Chassis.turn_to_angle(90); 
                Chassis.turn_to_angle(45); 
                Chassis.turn_to_angle(0); 
            }
        } else {
            toggleA = true; 
        }
        wait(10, msec); 
    }

    return 0; 
}