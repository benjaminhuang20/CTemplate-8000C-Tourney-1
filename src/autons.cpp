#include "vex.h"
#include <chrono>

double matchLoaderDistance = 11.5; 

int bottomAntiJam(){
    while(auto_started==1){
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
    while(auto_started==1){
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

void soloAWP_PUSH(){
    scraper = false;
    descoreLeft = true;
    Chassis.driveMaxOutputVolts = 12;

    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);

    autoTopIntakeSpeed = -6.f;
    autoBottomIntakeSpeed = 120.f;

    Chassis.driveMaxTime = 500;
    Chassis.drive_inches(8);
    Chassis.turn_to_angle(0.0);
    Chassis.driveMaxTime = 1000;
    Chassis.driveMaxOutputVolts = 8;
    Chassis.left.setStopping(vex::brakeType::coast);
    Chassis.right.setStopping(vex::brakeType::coast);
    Chassis.drive_inches(-50.0);

    Chassis.driveMaxTime = 800;
    Chassis.turnMaxTime = 600;
    Chassis.turnSettleTime = 600;
    scraper = true;
    Chassis.driveMaxTime = 1000;
    Chassis.driveSettleTime = 800;
    Chassis.drive_inches_from_wall(16, 0);
    Chassis.driveMaxTime = 800;
    Chassis.driveSettleTime = 500;
    Chassis.turnSettleTime = 400;
    Chassis.turn_to_angle(-90);
    autoBottomIntakeSpeed = 120.f;
    Chassis.driveMaxOutputVolts = 5;
    Chassis.driveSettleTime = 1500;
    Chassis.driveMaxOutputVolts = 12;
    Chassis.matchload(10.6, -90, 750);
    Chassis.turn_to_angle(-90);


    Chassis.driveMaxOutputVolts = 12;
    Chassis.driveMaxTime = 650;
    Chassis.drive_inches(-30.0);
    Chassis.driveMaxTime = 800;
    autoTopIntakeSpeed = 120.f;
    scraper = false;
    

    autoTopIntakeSpeed = 120.f; 
    wait(1, sec); 
    
    autoTopIntakeSpeed = 0.f;

    Chassis.turn_to_angle(10);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.driveMaxTime = 1100;
    Chassis.drive_inches(54.5,0);
    scraper = true;
    Chassis.turn_to_angle(-45);
    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(-17,-45);
    autoBottomIntakeSpeed = -120.f;
    wait(0.1,sec);
    autoBottomIntakeSpeed = 120.f;

    autoTopIntakeSpeed = -12.f;
    autoBottomIntakeSpeed = -12.f;
    mid = true;
    wait(0.2, sec);
    autoBottomIntakeSpeed = 120.f;
    wait(0.8, sec);
    autoBottomIntakeSpeed = 0.f;
    mid = false;
    autoBottomIntakeSpeed = 120.f;

    Chassis.driveMaxTime = 1300;
    Chassis.driveMaxOutputVolts = 12;
    Chassis.drive_inches(54.5,-45);
    Chassis.turn_to_angle(-90);
    Chassis.driveSettleTime = 1500;
    Chassis.driveMaxOutputVolts = 12;
    Chassis.matchload(10.6, -90, 750);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.driveMaxTime = 1000;
    Chassis.drive_inches(-35,-90);

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
    Chassis.turn_to_angle(135);
    Chassis.driveMaxTime = 5000; 
    Chassis.turnMaxOutputVolts = 4;
    Chassis.drive_inches(-100,180);
    Chassis.turnMaxOutputVolts = 12;
    Chassis.turn_to_angle(90);
    Chassis.distanceMaxTime = 2000; 
    Chassis.drive_inches_from_wall(17, 90, 0);
    Chassis.turn_to_angle(0);

    Chassis.driveMaxTime = 350; 
    Chassis.drive_inches(-20);
    Chassis.driveMaxTime = 750; 
    autoTopIntakeSpeed = 120.f;
    wait(2, sec);
    Chassis.turn_to_angle(0); 

    autoTopIntakeSpeed = 0.f; 
    Chassis.matchload(11.1, 0, 2900);

    Chassis.drive_inches(-50);
    autoTopIntakeSpeed = 120.f;
}

void score(){
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    autoTopIntakeSpeed = -120.f;
    autoBottomIntakeSpeed = 120.f;
    scraper = true;
    Chassis.matchload(11.1, 0, 5000);
}

void clearPark(){
    vex::task antiJamBottom = vex::task(bottomAntiJam);

    Chassis.turnMaxOutputVolts = 4; 
    Chassis.drive_inches(55, 90);

    Chassis.driveMaxOutputVolts = 8.f;
    Chassis.driveMaxTime = 5000;

    autoBottomIntakeSpeed = 120.f;
    
    Chassis.drive_inches(50, 80,
                         {18}, {[](void)
                                { scraper = true; }});
}

void skills(){
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    mid = false; 
    autoTopIntakeSpeed = -120.f;
    autoBottomIntakeSpeed = 120.f;

    Chassis.driveMaxOutputVolts = 6.f; 

    // Chassis.drive_inches_from_wall(24.f,0);
    Chassis.turn_to_angle(-20);
    Chassis.drive_inches(28.f, Chassis.get_heading(), {10, 24}, {[](void)
                                                                    { Chassis.driveMaxOutputVolts = 3.f;}, [](void)
                                                                    { scraper = true; }});
    // Chassis.drive_inches(28.f); 
    // scraper = true;
    Chassis.driveMaxOutputVolts = 5.f; 
    Chassis.turn_to_angle(-135);
    Chassis.driveMaxTime = 1500; 
    Chassis.drive_inches(-19.2);
    Chassis.driveMaxTime = 3000;
    autoBottomIntakeSpeed = -12.f;
    mid = true;
    wait(0.2, sec);
    autoBottomIntakeSpeed = 120.f;
    wait(0.8, sec);
    autoBottomIntakeSpeed = 0.f;
    mid = false;
    Chassis.driveMaxOutputVolts = 8.f;

    // Chassis.turnMaxOutputVolts = 4.f; 
    // Chassis.drive_inches_from_wall(20, 1);

    Chassis.driveMaxTime = 5000;
    // Chassis.drive_inches(49.5);
    Chassis.driveSettleTime = 300; 
    Chassis.drive_inches(51.5);
    Chassis.driveSettleTime = 100; 
    Chassis.turn_to_angle(180);
    Chassis.driveMaxTime = 500; 
    Chassis.drive_inches(-20);
    Chassis.driveMaxOutputVolts = 10.f;
    autoTopIntakeSpeed = 120.f;
    autoBottomIntakeSpeed = 120.f;
    wait(1, sec);
    Chassis.turn_to_angle(180);

    autoTopIntakeSpeed = 0; 
    Chassis.matchload(11.1, -180, 2900);

    RBCrossover();
    clearPark();
}

void rightSplit(){
    Chassis.driveMaxTime = 800;
    Chassis.turnMaxTime = 600;
    Chassis.turnSettleTime = 600;
    scraper = false;
    descoreLeft = true;
    Chassis.driveMaxOutputVolts = 12;
    Chassis.drive_inches(35);
    Chassis.drive_inches_from_wall(21, 1);
    scraper = true;
    Chassis.turn_to_angle(90);
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    autoBottomIntakeSpeed = 120.f;
    Chassis.driveMaxOutputVolts = 5;
    Chassis.matchload(11.1, 90, 800);

    Chassis.driveMaxOutputVolts = 5;
    Chassis.driveMaxTime = 1200;
    Chassis.drive_inches(-30.0);
    autoTopIntakeSpeed = 11.f;  
    Chassis.driveMaxOutputVolts = 11;
    intakeRight.spin(vex::directionType::rev, 120, vex::voltageUnits::volt); 
    scraper = false;
    wait(1, seconds);
    
    autoTopIntakeSpeed = 0.f;
    autoBottomIntakeSpeed = 6.f;
    Chassis.driveMaxTime = 1000;
    Chassis.drive_inches(22.0, 260);
    autoTopIntakeSpeed = 120.f;
    Chassis.driveMaxOutputVolts = 10;
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(23.3,220);
    wait(1, seconds);
    autoBottomIntakeSpeed = -120.f;
    Chassis.driveMaxOutputVolts = 5;
    Chassis.drive_inches(5);
    wait(0.8, seconds);

    Chassis.turnMaxOutputVolts = 1;
    Chassis.drive_inches(-35, -90);

    Chassis.turnMaxOutputVolts = 6;
    Chassis.driveMaxOutputVolts = 11;
    descoreLeft = false;
    Chassis.drive_inches(16,-90);
    
}

void soloAWP(){
  scraper = false;
    descoreLeft = true;
    Chassis.driveMaxOutputVolts = 12;

    intakeRight.spin(vex::directionType::fwd, 120, vex::voltageUnits::volt); 
    intakeLeft.spin(vex::directionType::rev, 120, vex::voltageUnits::volt); 


    
    
    //pick up preload
   
    Chassis.drive_inches(-42.0);

    Chassis.driveMaxTime = 800;
    Chassis.turnMaxTime = 600;
    Chassis.turnSettleTime = 600;
    scraper = true;
        // Chassis.driveMaxTime = 5000;

    Chassis.drive_inches_from_wall(16.5, 0);
    //matchload
    Chassis.driveSettleTime = 500;
    Chassis.turnSettleTime = 400;
    Chassis.turn_to_angle(-90);
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    // vex::task antiJamTop = vex::task(topAntiJam);
    autoBottomIntakeSpeed = 120.f;
    Chassis.driveMaxOutputVolts = 5;
    Chassis.matchload(11.2, -90, 700);
    Chassis.turn_to_angle(-90);

    //long goal 1

    Chassis.driveMaxOutputVolts = 12;
    Chassis.drive_inches(-30.0);
    Chassis.driveMaxTime = 800;
    autoTopIntakeSpeed = 120.f;
    scraper = false;
    

    vex::task antiJamTop = vex::task(topAntiJam);
    autoTopIntakeSpeed = 120.f; 
    wait(1, sec); 
    
    autoTopIntakeSpeed = 0.f; 
    // Chassis.set_heading(-90);
    //motion chain/crossing
    Chassis.driveMaxTime = 500;
    Chassis.turn_to_angle(-90);
    Chassis.drive_inches(12);
    descoreLeft = true;
    Chassis.turnMaxOutputVolts = 11;
    Chassis.driveMaxOutputVolts = 11; 
    autoTopIntakeSpeed = 0.f; 
    Chassis.turn_to_angle(45);

    Chassis.driveMaxTime = 1000;
    Chassis.driveSettleTime = 200;
    Chassis.turnSettleTime = 400;
    Chassis.turnMaxTime = 500;
    Chassis.turnMaxOutputVolts = 5.5;

    Chassis.driveMaxTime = 3000;
    Chassis.turnMaxTime = 3000;

    Chassis.drive_inches(77 ,0);
    Chassis.turnMaxOutputVolts = 9;
    Chassis.driveMaxTime = 1000;
    Chassis.driveSettleTime = 200;
    Chassis.turnSettleTime = 400;
    Chassis.turnMaxTime = 500;
    Chassis.turn_to_angle(-45);
    Chassis.drive_inches(-21);
    mid = true;
    autoTopIntakeSpeed = -120.f;
    autoBottomIntakeSpeed = 120.f;
    
    wait(1,sec); 
    mid = false;
    autoTopIntakeSpeed = 0.f;
    Chassis.turn_to_angle(-38);
    Chassis.drive_inches(43);
    Chassis.turnMaxOutputVolts = 11;
    // // Chassis.drive_inches(50);
    // Chassis.drive_inches_from_wall(21,1,1);
    // scraper = true;
    

    // //long goal 2
    Chassis.turn_to_angle(-90);
    Chassis.driveMaxTime = 500;
    Chassis.drive_inches(-15);
    autoTopIntakeSpeed = 120.f;
    // wait(1,sec); 
    // Chassis.turn_to_angle(-90);
    // Chassis.matchload(11.2, -90, 800);

    
    // Chassis.turnMaxOutputVolts = 7;
    // Chassis.drive_inches(60.0, 0);
    // Chassis.drive_inches(40.0, -40);
    // Chassis.driveMaxTime = 200;
    // Chassis.turnMaxTime = 500;
    // Chassis.turn_to_angle(0);
    // Chassis.driveMaxTime = 500;
    // Chassis.driveSettleTime = 500;
    // Chassis.drive_inches_from_wall(21, 1);
    // Chassis.turnMaxOutputVolts = 12;
    // Chassis.turn_to_angle(-90);
    // Chassis.drive_inches(-15);
    // intakeRight.spin(vex::directionType::rev, 120, vex::voltageUnits::volt); 
    // wait(0.8, sec); 
    // scraper = true;
    // Chassis.driveMaxOutputVolts = 5;
    // Chassis.drive_inches(35);
    // intakeRight.spin(vex::directionType::rev, 0, vex::voltageUnits::volt); 
    // Chassis.driveMaxTime = 700;
    // Chassis.driveMaxOutputVolts = 7;
    // Chassis.matchload(11.2, -90, 950);
    // Chassis.driveMaxOutputVolts = 11;
    // Chassis.turnMaxOutputVolts = 10;
    //     Chassis.driveMaxTime = 1000;

    // Chassis.drive_inches(-50.0, -45);
    //     Chassis.driveMaxOutputVolts = 5;

    //  Chassis.drive_inches(-30, -45);
    // intakeRight.spin(vex::directionType::rev, 120, vex::voltageUnits::volt); 
   
}

void right7ball(){
    scraper = false;
    descoreLeft = true;
    

    Chassis.driveMaxTime = 800;
    Chassis.turnMaxTime = 600;
    Chassis.turnSettleTime = 600;

    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);

    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 

    
    
    Chassis.driveMaxOutputVolts = 10;
    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(29,10);
    Chassis.driveMaxTime = 500;
    Chassis.driveMaxOutputVolts = 6;
    scraper = true;
    Chassis.drive_inches(8,10);
    
    
    Chassis.driveMaxTime = 2000;
    //Chassis.turnMaxTime = 2000; //delete if no time to tune
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-34,-90);
    Chassis.turnMaxOutputVolts = 10;
    Chassis.driveSettleTime = 700;
    Chassis.drive_inches_from_wall(17,-90,0);
    scraper = true;
    Chassis.driveMaxTime = 800;
    Chassis.turn_to_angle(180);
    Chassis.turnMaxOutputVolts = 7;
    Chassis.matchload(11, 180, 700);
    Chassis.turn_to_angle(-180);
    Chassis.driveMaxOutputVolts = 8;
    Chassis.drive_inches(-36);
    autoTopIntakeSpeed = 120.f; 
    wait(2.2,sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;
    Chassis.turn_to_angle(120);
    Chassis.drive_inches(8.5);
    Chassis.turn_to_angle(180);
    descoreLeft = false;
    Chassis.drive_inches(-28);
}

void right4ball(){
    scraper = false;
    descoreLeft = true;
    
    Chassis.driveMaxTime = 800;
    Chassis.turnMaxTime = 600;
    Chassis.turnSettleTime = 600;

    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);

    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 

    Chassis.driveMaxOutputVolts = 11;
    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(29,10);
    Chassis.driveMaxTime = 500;
    Chassis.driveMaxOutputVolts = 10;
    scraper = true;
    Chassis.drive_inches(8,10);
    
    
    Chassis.driveMaxTime = 2000;
    //Chassis.turnMaxTime = 2000; //delete if no time to tune
    Chassis.turnMaxOutputVolts = 9;
    Chassis.drive_inches(-34,-90);
    Chassis.turnMaxOutputVolts = 10;
    Chassis.driveSettleTime = 200;
    Chassis.driveMaxTime = 400;
    Chassis.drive_inches_from_wall(17,-90,0);
    scraper = true;
    Chassis.driveMaxTime = 800;
    Chassis.turn_to_angle(180);
    Chassis.driveMaxOutputVolts = 8;
    Chassis.drive_inches(-24);
    autoTopIntakeSpeed = 120.f; 
    wait(1.3,sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;
    Chassis.turn_to_angle(120);
    Chassis.drive_inches(8.5);
    Chassis.turn_to_angle(180);
    descoreLeft = false;
    Chassis.drive_inches(-26);    
}

void right9ball(){
    scraper = false;
    descoreLeft = true;
    
    Chassis.driveMaxTime = 800;
    Chassis.turnMaxTime = 600;
    Chassis.turnSettleTime = 600;

    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);

    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 

    Chassis.driveMaxOutputVolts = 11;
    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(30.5,8);
    Chassis.driveMaxOutputVolts = 9;
    Chassis.driveMaxTime = 1000;
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(36.5,50);
    Chassis.driveMaxTime = 1300;
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-40,-90);
    Chassis.drive_inches_from_wall(15.5,-90,0);
    scraper = true;
    Chassis.turn_to_angle(180);
    Chassis.matchload(11, 180, 700);
    Chassis.turn_to_angle(180);
    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(-26);  
    autoTopIntakeSpeed = 120.f;
}

void SkillsControlMiddle(){
    scraper = false;
    descoreLeft = true;
    mid = false;
    
    Chassis.driveMaxTime = 800;
    Chassis.turnMaxTime = 600;
    Chassis.turnSettleTime = 600;

    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);

    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 

    Chassis.driveMaxOutputVolts = 11;

    Chassis.driveMaxTime = 1000;
    Chassis.drive_inches(40);
    Chassis.turn_to_angle(-135);
    Chassis.drive_inches(-15, -135);
    mid = true;
    autoTopIntakeSpeed = -120.f; 
    autoBottomIntakeSpeed = 120.f;
    wait(3,sec);
    mid = false;

}
