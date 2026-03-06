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

void drive_test(){
    Chassis.drive_inches(24); //testing big and small drives
    Chassis.drive_inches(-24); 
    Chassis.drive_inches(12); 
    Chassis.drive_inches(12); 
    Chassis.drive_inches(-8); 
    Chassis.drive_inches(-4); 
    Chassis.drive_inches(-4);
    Chassis.drive_inches(-2); 
    Chassis.drive_inches(-2);
    Chassis.drive_inches(-1);
    Chassis.drive_inches(-1);
    Chassis.drive_inches(-1);
    Chassis.drive_inches(-1);
}

void turn_test(){
    Chassis.turn_to_angle(45); //testing how fast fast turns are
    Chassis.turn_to_angle(90);
    Chassis.turn_to_angle(135);
    Chassis.turn_to_angle(180);
    Chassis.turn_to_angle(0);

    for (int i = 0; i < 720; i+=10){ //testing how fast small turns are
        Chassis.turn_to_angle(i); 
    }
}

void soloAWP_PUSH(){
    //constants
    scraper = false;
    descoreLeft = true;
    Chassis.driveMaxOutputVolts = 12;
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);

    //store mode
    autoTopIntakeSpeed = -6.f;
    autoBottomIntakeSpeed = 120.f;

    //push other bot off park zone
    Chassis.driveMaxTime = 500;
    Chassis.drive_inches(8);
    //reset angle if adjusted by other bot
    Chassis.turn_to_angle(0.0);
    Chassis.driveMaxTime = 1000;
    //reduce speed for better control and go backwards
    Chassis.driveMaxOutputVolts = 8;
    Chassis.drive_inches(-50.0);

    Chassis.driveMaxTime = 800;
    Chassis.turnMaxTime = 600;
    Chassis.turnSettleTime = 600;
    //scraper down to prepare for matchload
    scraper = true;
    Chassis.driveMaxTime = 1000;
    Chassis.driveSettleTime = 800;
    //distance sensor to align with wall
    Chassis.drive_inches_from_wall(16, 0);
    Chassis.driveMaxTime = 800;
    Chassis.driveSettleTime = 500;
    Chassis.turnSettleTime = 400;
    //turn to face matchloader
    Chassis.turn_to_angle(-90);
    autoBottomIntakeSpeed = 120.f;
    Chassis.driveMaxOutputVolts = 5;
    Chassis.driveSettleTime = 1500;
    Chassis.driveMaxOutputVolts = 12;
    //function to use distance sensor to align with matchloader and then load
    Chassis.matchload(10.6, -90, 750);
    Chassis.turn_to_angle(-90);
    Chassis.driveMaxOutputVolts = 12;
    Chassis.driveMaxTime = 650;
    //score on first long goal
    Chassis.drive_inches(-30.0);
    Chassis.driveMaxTime = 800;
    //put scraper up to prevent interference with turn
    scraper = false;
    autoTopIntakeSpeed = 120.f; 
    wait(1, sec); 
    //switch to storing again
    autoTopIntakeSpeed = -6.f;
    //turn inside of long goal to face corner blocks
    Chassis.turn_to_angle(10);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.driveMaxTime = 1100;
    //drive across corner blocks and collect
    Chassis.drive_inches(54.5,0);
    //put scraper down to prevent blocks from escaping
    scraper = true;
    //turn to face middle goal and drive into it
    Chassis.turn_to_angle(-45);
    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(-17,-45);
    //outtake to stop jamming on mid goal
    autoBottomIntakeSpeed = -120.f;
    wait(0.1,sec);
    autoBottomIntakeSpeed = 120.f;

    //spin hood backwards full speed to get any blocks out of hood and score
    autoTopIntakeSpeed = -12.f;
    autoBottomIntakeSpeed = -12.f;
    //middle goal piston is inverted, so mid = true means down and mid = false means up
    mid = true;
    wait(0.2, sec);
    autoBottomIntakeSpeed = 120.f;
    wait(0.8, sec);
    //go back to storing
    autoBottomIntakeSpeed = 0.f;
    mid = false;
    autoBottomIntakeSpeed = 120.f;

    Chassis.driveMaxTime = 1300;
    Chassis.driveMaxOutputVolts = 12;
    //drive towards second long goal
    Chassis.drive_inches(54.5,-45);
    //turn to face second match loader
    Chassis.turn_to_angle(-90);
    Chassis.driveSettleTime = 1500;
    Chassis.driveMaxOutputVolts = 12;
    //use distance sensor to align with matchloader and load
    Chassis.matchload(10.6, -90, 750);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.driveMaxTime = 1000;
    //back into long goal
    Chassis.drive_inches(-35,-90);
    //score until time runs out
    autoTopIntakeSpeed = 120.f;
}

void RBCrossover(){
    Chassis.turn_to_angle(135);
    Chassis.driveMaxTime = 5000; 
    Chassis.turnMaxOutputVolts = 4;
    Chassis.drive_inches(-100,180);
    Chassis.turnMaxOutputVolts = 12;
    Chassis.turn_to_angle(90);
    Chassis.distanceMaxTime = 2000; 
    Chassis.drive_inches_from_wall(18, 90, 0);
    Chassis.turn_to_angle(0);

    Chassis.driveMaxTime = 350; 
    Chassis.drive_inches(-22);
    Chassis.driveMaxTime = 750; 
    autoTopIntakeSpeed = 120.f;
    wait(2.5, sec);
    Chassis.turn_to_angle(0); 

    autoTopIntakeSpeed = 0.f; 
    Chassis.matchload(11.1, 0, 2900);

    Chassis.drive_inches(-50);
    autoTopIntakeSpeed = 120.f;
    wait(2.5, sec);
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
    scraper = false; 

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
    Chassis.drive_inches(-22);
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
    Chassis.drive_inches(51.75);
    Chassis.driveSettleTime = 100; 
    Chassis.turn_to_angle(180);
    Chassis.driveMaxTime = 500; 
    Chassis.drive_inches(-25);
    Chassis.driveMaxOutputVolts = 10.f;
    autoTopIntakeSpeed = 120.f;
    autoBottomIntakeSpeed = 120.f;
    wait(1, sec);
    Chassis.turn_to_angle(180);

    autoTopIntakeSpeed = 0; 
    Chassis.matchload(11.1, -180, 2900);

    RBCrossover();
    clearPark();

    Chassis.drive_inches_from_wall(30,90,1);
    Chassis.turn_to_angle(180);
    Chassis.drive_inches_from_wall(30,90,0);
}

void worseSkills(){
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    mid = false;
    scraper = true; 

    autoTopIntakeSpeed = -120.f;
    autoBottomIntakeSpeed = 120.f;

    Chassis.set_heading(90);
    Chassis.drive_inches_from_wall(18, 90, 0);
    Chassis.turn_to_angle(180);
    Chassis.matchload(11.1, 180, 2900);

    RBCrossover();
    Chassis.drive_inches(10);
    Chassis.turn_to_angle(-90);
    Chassis.drive_inches_from_wall(17, -90, 0);
    Chassis.turn_to_angle(0);

    Chassis.set_heading(180 - Chassis.get_heading());
    Chassis.matchload(11.1, 180, 2900);

    RBCrossover();
    
    Chassis.drive_inches(55, 90);
    Chassis.driveMaxTime = 4000;
    Chassis.drive_inches(30);
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



//not aligned no constant block
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
//not aligned no constant block
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
//aligned constant block
void right4ballcorner(){
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.turnMaxTime = 600;
    Chassis.driveMaxTime = 800;
    Chassis.turnSettleTime = 600;
    Chassis.driveSettleTime = 700;
    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 
    scraper = false;
    descoreLeft = true;

    Chassis.turn_to_angle(15);
    Chassis.drive_inches(28.f, Chassis.get_heading(), {10, 24}, {[](void)
                                                                    { Chassis.driveMaxOutputVolts = 3.f;}, [](void)
                                                                    { scraper = true; }});
    Chassis.driveMaxOutputVolts = 6;
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-34,-90);
    Chassis.turnMaxOutputVolts = 10;
    Chassis.drive_inches_from_wall(17,-90,0);
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
//aligned, constant block
void right4ballmatchload(){
    
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.turnMaxTime = 600;
    Chassis.driveMaxTime = 800;
    Chassis.turnSettleTime = 600;
    Chassis.driveSettleTime = 700;
    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 
    scraper = false;
    descoreLeft = true;
    
    Chassis.drive_inches(35,0);
    Chassis.drive_inches_from_wall(19,0,1);
    Chassis.turn_to_angle(90);
    scraper = true;
    Chassis.matchload(11, 90, 700);
    Chassis.drive_inches(-34);
    autoTopIntakeSpeed = 120.f; 
    wait(1.3,sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;
    Chassis.turn_to_angle(60);
    Chassis.drive_inches(8.5);
    Chassis.turn_to_angle(90);
    descoreLeft = false;
    Chassis.drive_inches(-26);    
}
//aligned constant block
void right7ball(){
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.turnMaxTime = 600;
    Chassis.driveMaxTime = 800;
    Chassis.turnSettleTime = 600;
    Chassis.driveSettleTime = 700;
    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 
    scraper = false;
    descoreLeft = true;

    Chassis.turn_to_angle(15);
    Chassis.drive_inches(28.f, Chassis.get_heading(), {10, 24}, {[](void)
                                                                    { Chassis.driveMaxOutputVolts = 3.f;}, [](void)
                                                                    { scraper = true; }});
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-34,-90);
    Chassis.turnMaxOutputVolts = 10;
    Chassis.drive_inches_from_wall(17,-90,0);
    scraper = true;
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
//theory coded, aligned, constant block
void right10ball(){
    //modular constant block
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.turnMaxTime = 600;
    Chassis.driveMaxTime = 800;
    Chassis.turnSettleTime = 600;
    Chassis.driveSettleTime = 700;
    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 
    scraper = false;
    descoreLeft = true;

    //4 block matchload code
    //drive forward to align to wall
    Chassis.drive_inches(35,0);
    Chassis.drive_inches_from_wall(19,0,1);
    //turn to face matchloader and load
    Chassis.turn_to_angle(90);
    scraper = true;
    //matchloader distance sensor alignment function
    Chassis.matchload(11, 90, 700);
    //drive into long goal
    Chassis.drive_inches(-34);
    autoTopIntakeSpeed = 120.f; 
    wait(1.3,sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;

    //solo awp code without the long goal scoring
    Chassis.turn_to_angle(-10);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.driveMaxTime = 1100;
    //drive across corner blocks and collect
    Chassis.drive_inches(54.5,0);
    //put scraper down to prevent blocks from escaping
    scraper = true;
    //turn to face middle goal and drive into it
    Chassis.turn_to_angle(-45);
    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(-17,-45);
    //outtake to stop jamming on mid goal
    autoBottomIntakeSpeed = -120.f;
    wait(0.1,sec);
    autoBottomIntakeSpeed = 120.f;

    //spin hood backwards full speed to get any blocks out of hood and score
    autoTopIntakeSpeed = -12.f;
    autoBottomIntakeSpeed = -12.f;
    //middle goal piston is inverted, so mid = true means down and mid = false means up
    mid = true;
    wait(0.2, sec);
    autoBottomIntakeSpeed = 120.f;
}

void leftSplit(){

}
//PLS INVERT ANGLE AND SHIT
void left7ball(){
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.turnMaxTime = 600;
    Chassis.driveMaxTime = 800;
    Chassis.turnSettleTime = 600;
    Chassis.driveSettleTime = 700;
    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 
    scraper = false;
    descoreLeft = true;

    Chassis.turn_to_angle(15);
    Chassis.drive_inches(28.f, Chassis.get_heading(), {10, 24}, {[](void)
                                                                    { Chassis.driveMaxOutputVolts = 3.f;}, [](void)
                                                                    { scraper = true; }});
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-34,-90);
    Chassis.turnMaxOutputVolts = 10;
    Chassis.drive_inches_from_wall(17,-90,0);
    scraper = true;
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

//pls invert angles and shit
void left4ballcorner(){
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.turnMaxTime = 600;
    Chassis.driveMaxTime = 800;
    Chassis.turnSettleTime = 600;
    Chassis.driveSettleTime = 700;
    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 
    scraper = false;
    descoreLeft = true;
    
    Chassis.turn_to_angle(15);
    Chassis.drive_inches(28.f, Chassis.get_heading(), {10, 24}, {[](void)
                                                                    { Chassis.driveMaxOutputVolts = 3.f;}, [](void)
                                                                    { scraper = true; }});
    Chassis.driveMaxOutputVolts = 6;
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-34,-90);
    Chassis.turnMaxOutputVolts = 10;
    Chassis.drive_inches_from_wall(17,-90,0);
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
//pls invert angles and shit
void left4ballmatchload(){
 vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.turnMaxTime = 600;
    Chassis.driveMaxTime = 800;
    Chassis.turnSettleTime = 600;
    Chassis.driveSettleTime = 700;
    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f; 
    scraper = false;
    descoreLeft = true;
    
    Chassis.drive_inches(35,0);
    Chassis.drive_inches_from_wall(19,0,1);
    Chassis.turn_to_angle(90);
    scraper = true;
    Chassis.matchload(11, 90, 700);
    Chassis.drive_inches(-34);
    autoTopIntakeSpeed = 120.f; 
    wait(1.3,sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;
    Chassis.turn_to_angle(60);
    Chassis.drive_inches(8.5);
    Chassis.turn_to_angle(90);
    descoreLeft = false;
    Chassis.drive_inches(-26);    
}