#include "vex.h"



int bottomAntiJam(){
    while(auto_started){
        double voltage = intakeLeft.voltage(vex::voltageUnits::volt);
        double efficiency = intakeLeft.efficiency(); 

        if(voltage > 0 && efficiency == 0){
            intakeLeft.spin(vex::directionType::fwd, 120, vex::voltageUnits::volt); 
        } else{
            intakeLeft.spin(vex::directionType::rev, 120, vex::voltageUnits::volt);
        }

        wait(0.2, sec); 
    }

    return 0; 
}

void test(){
    scraper = true; 
    vex::task antiJam = vex::task(bottomAntiJam); 
    Chassis.drive_inches_from_wall(12, FRONT_WALL);
    // Chassis.drive_inches(-10);
    // Chassis.drive_inches(10, 50);
}

void right4Ball(){
    scraper = true;
    vex::task antiJam = vex::task(bottomAntiJam); 
    Chassis.drive_inches_from_wall(20, FRONT_WALL);
    Chassis.turn_to_angle(-90);
    Chassis.driveSettleError = -1;
    Chassis.driveMaxTime = 1500;
    Chassis.drive_inches_from_wall(11.5, FRONT_WALL);
    Chassis.driveSettleError = 0.5;
    Chassis.driveMaxTime = 1500;
    Chassis.driveMaxOutputVolts = 8; 
    Chassis.drive_inches(-40, -90);
    intakeRight.spin(vex::directionType::rev, 120, vex::voltageUnits::volt); 
}
