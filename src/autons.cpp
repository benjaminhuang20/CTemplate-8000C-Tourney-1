#include "vex.h"

double matchLoaderDistance = 11.5; 

int bottomAntiJam(){
    while(auto_started){
        double voltage = intakeLeft.voltage(vex::voltageUnits::volt);
        double efficiency = intakeLeft.efficiency(); 

        if(fabs(voltage) > 0 && efficiency < 5){
            intakeLeft.spin(vex::directionType::fwd, autoBottomIntakeSpeed, vex::voltageUnits::volt); 
        } else{
            intakeLeft.spin(vex::directionType::rev, autoBottomIntakeSpeed, vex::voltageUnits::volt);
        }

        wait(0.2, sec); 
    }

    return 0; 
}

int topAntiJam(){
    while(auto_started){
        double voltage = intakeRight.voltage(vex::voltageUnits::volt);
        double efficiency = intakeRight.efficiency(); 

        if(fabs(voltage) > 0 && efficiency < 5){
            intakeRight.spin(vex::directionType::fwd, autoTopIntakeSpeed, vex::voltageUnits::volt); 
        } else{
            intakeRight.spin(vex::directionType::rev, autoTopIntakeSpeed, vex::voltageUnits::volt); 
        }

        wait(0.2, sec); 
    }

    return 0; 
}

void test(){
    scraper = true; 
    vex::task antiJam = vex::task(bottomAntiJam); 
    Chassis.drive_inches_from_wall(12, 1);
    // Chassis.drive_inches(-10);
    // Chassis.drive_inches(10, 50);
}

void right4Ball(){
    scraper = true;

    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);

    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = 0.f; 

    Chassis.driveSettleTime = 300; 
    Chassis.drive_inches_from_wall(19, 1); //1 is front wall 0 is back wall 
    Chassis.driveSettleTime = 150; 
    // Chassis.drive_inches(30); 
    Chassis.turn_to_angle(90);
    Chassis.driveSettleError = -1;
    Chassis.driveMaxTime = 3000;
    Chassis.driveMaxOutputVolts = 6; 
    Chassis.drive_inches_from_wall(11.5, 1);
    // Chassis.drive_inches(15); 
    Chassis.driveSettleError = 0.5;
    Chassis.driveMaxTime = 1500;
    Chassis.drive_inches(-40, 90);
    Chassis.drive_inches(1.5);
    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = 120.f; 
}

void rightLowerGoal(){
    scraper = false;
    // tripleStateOne = true; //store
    // tripleStateTwo = false;
    autoTopIntakeSpeed = 120.f; 

    vex::task antiJam = vex::task(bottomAntiJam);
    Chassis.drive_inches(24.5);
    Chassis.turnMaxOutputVolts = 12;
    Chassis.turn_to_angle(30);
    Chassis.driveMaxOutputVolts = 6; 
    Chassis.drive_inches(21);
    wait(1, sec); 
    Chassis.driveMaxOutputVolts = 10; 
    Chassis.turn_to_angle(-45);
    Chassis.drive_inches(19.5);

    autoTopIntakeSpeed = -120.f; 
    // scraper = true;
}

void RBCrossover(){
    // Chassis.turn_to_angle(-135);
    Chassis.drive_inches(-15, -135); 
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-87, -90);
    Chassis.turnMaxOutputVolts = 12;
    Chassis.turn_to_angle(180); 

    Chassis.driveSettleTime = 300; 
    Chassis.drive_inches_from_wall(17, 0); //back wall is 0
    Chassis.driveSettleTime = 150; 
    Chassis.turn_to_angle(90);
}

void score(){
    Chassis.driveMaxTime = 1500; 
    Chassis.drive_inches(-20);
    Chassis.driveMaxTime = 3000;

    autoTopIntakeSpeed = 120.f;
    wait(2.5, sec);
    Chassis.turn_to_angle(90); 
    autoTopIntakeSpeed = 0.f;

    Chassis.matchload(11.5, 90, 3000);
    Chassis.turn_to_angle(90); 

    Chassis.driveMaxTime = 1000; 
    Chassis.drive_inches(-50);
    Chassis.driveMaxTime = 3000;
    autoTopIntakeSpeed = 120.f;
    wait(2.5, sec);
    Chassis.turn_to_angle(90); 
}

void skills(){
    // scraper = true; 
    
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);

    autoBottomIntakeSpeed = 120.f; //storing
    autoTopIntakeSpeed = 0.f; 

    Chassis.driveSettleTime = 300; 
    Chassis.drive_inches_from_wall(19, 1);

    Chassis.driveSettleTime = 150;
    Chassis.turn_to_angle(-90);
    Chassis.matchload(11.5, -90, 3000); //matchloads at 11.5 away from matchload adjusting to -90 for 3 seconds

    RBCrossover();
    score();

    // Chassis.set_heading(90); 
    scraper = false;
    Chassis.turnMaxOutputVolts = 4.5;
    Chassis.driveMaxTime = 5000;
    // Chassis.driveMaxOutputVolts = 12; 
    Chassis.drive_inches(100, 180);
    // Chassis.turnMaxOutputVolts = 12;

    // Chassis.drive_inches(-15); 
    // Chassis.driveMaxTime = 5000;
    // Chassis.driveMaxOutputVolts = 10;
    // Chassis.drive_inches_from_wall(50, 1);

    //NOTE FOR TATE: PLEASE USE FRONT DISTANCE SENSOR AFTER CLEAR
}
