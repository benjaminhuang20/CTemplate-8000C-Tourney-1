#include "vex.h"
#include <chrono>

double matchLoaderDistance = 11.5;

int bottomAntiJam()
{
    while (auto_started == 1)
    {
        double voltage = intakeLeft.voltage(vex::voltageUnits::volt);
        double efficiency = intakeLeft.efficiency();

        if (fabs(voltage) > 0 && efficiency < 5)
        {
            intakeLeft.spin(vex::directionType::fwd, autoBottomIntakeSpeed, vex::voltageUnits::volt);
        }
        else
        {
            intakeLeft.spin(vex::directionType::rev, autoBottomIntakeSpeed, vex::voltageUnits::volt);
        }

        wait(0.2, sec);
    }

    return 0;
}

int topAntiJam()
{
    while (auto_started == 1)
    {
        double voltage = intakeRight.voltage(vex::voltageUnits::volt);
        double efficiency = intakeRight.efficiency();

        if (fabs(voltage) > 0 && efficiency < 5)
        {
            intakeRight.spin(vex::directionType::fwd, autoTopIntakeSpeed, vex::voltageUnits::volt);
        }
        else
        {
            intakeRight.spin(vex::directionType::rev, autoTopIntakeSpeed, vex::voltageUnits::volt);
        }

        wait(0.2, sec);
    }

    return 0;
}

void Push()
{
    Chassis.set_heading(180);
    Chassis.turn_to_angle(60);
    Chassis.drive_inches(8);
    Chassis.turn_to_angle(180);
    descoreLeft = false;
    Chassis.drive_inches(-20);
}

void drive_test()
{
    Chassis.drive_inches(24); // testing big and small drives
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

void turn_test()
{
    Chassis.turn_to_angle(45); // testing how fast fast turns are
    Chassis.turn_to_angle(90);
    Chassis.turn_to_angle(135);
    Chassis.turn_to_angle(180);
    Chassis.turn_to_angle(0);

    for (int i = 0; i < 720; i += 10)
    { // testing how fast small turns are
        Chassis.turn_to_angle(i);
    }
}

void soloAWP_PUSH()
{
    // constants
    scraper = false;
    descoreLeft = true;
    Chassis.driveMaxOutputVolts = 10;
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);

    // store mode
    autoTopIntakeSpeed = -6.f;
    autoBottomIntakeSpeed = 120.f;

    // push other bot off park zone
    // Chassis.driveMaxTime = 500;
    // Chassis.drive_inches(8);
    // reset angle if adjusted by other bot
    // Chassis.turn_to_angle(0.0);
    // Chassis.driveMaxTime = 1000;
    // reduce speed for better control and go backwards
    Chassis.driveMaxOutputVolts = 6;
    Chassis.drive_inches(35.0);

    Chassis.driveMaxTime = 800;
    Chassis.turnMaxTime = 700;
    Chassis.turnSettleTime = 600;
    // scraper down to prepare for matchload
    scraper = true;
    Chassis.driveMaxTime = 500;
    Chassis.driveSettleTime = 800;
    // distance sensor to align with wall
    Chassis.driveMaxOutputVolts = 10;
    float prevDistanceP = Chassis.distanceP;
    float prevDistanceI = Chassis.distanceI;
    float prevDistanceD = Chassis.distanceD;
    float prevDistanceMaxTime = Chassis.distanceMaxTime;
    float prevDistanceSettleTime = Chassis.distanceSettleTime;
    float prevDistanceSettleError = Chassis.distanceSettleError;
    float prevDistanceMaxOutputVolts = Chassis.distanceMaxOutputVolts;
    Chassis.set_distance_constants(1.2f, 0.0f, 60.0f, 600.0f, 400.0f, 1.2f, 7.8f, Chassis.distanceSlew);
    Chassis.drive_inches_from_wall(21, 1);
    Chassis.set_distance_constants(
        prevDistanceP,
        prevDistanceI,
        prevDistanceD,
        prevDistanceMaxTime,
        prevDistanceSettleTime,
        prevDistanceSettleError,
        prevDistanceMaxOutputVolts,
        Chassis.distanceSlew);
    Chassis.driveMaxTime = 800;
    Chassis.driveSettleTime = 800;
    Chassis.turnSettleTime = 300;
    // turn to face matchloader
    Chassis.turn_to_angle(90);
    autoBottomIntakeSpeed = 120.f;
    Chassis.driveMaxOutputVolts = 5;
    Chassis.driveSettleTime = 1500;
    Chassis.driveMaxOutputVolts = 12;
    // function to use distance sensor to align with matchloader and then load
    Chassis.matchload(11.2, 90, 850);
    Chassis.driveMaxOutputVolts = 8;
    Chassis.driveMaxTime = 1000;
    // score on first long goal
    Chassis.drive_inches(-35.0);
    Chassis.driveMaxTime = 800;
    // put scraper up to prevent interference with turn
    scraper = false;
    autoTopIntakeSpeed = 120.f;
    wait(1.0, sec);
    // switch to storing again
    autoTopIntakeSpeed = -6.f;
    // turn inside of long goal to face corner blocks
    Chassis.turn_to_angle(190);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.driveMaxTime = 1100;
    // drive across corner blocks and collect
    Chassis.turn_to_angle(180);
        Chassis.drive_inches(50.5, Chassis.get_heading(), {3, 25, 45}, {[](void)
                                                                 { scraper = true;}, [](void)
                                                                 { scraper = false; }, [](void)
                                                                 { scraper = true; }});
    // put scraper down to prevent blocks from escaping
    // turn to face middle goal and drive into it
    Chassis.turn_to_angle(135);
    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(-15.3, 135);
    // outtake to stop jamming on mid goal
    autoBottomIntakeSpeed = -120.f;
    wait(0.1, sec);
    autoBottomIntakeSpeed = 120.f;

    // spin hood backwards full speed to get any blocks out of hood and score
    autoTopIntakeSpeed = -12.f;
    autoBottomIntakeSpeed = -2.f;
    // middle goal piston is inverted, so mid = true means down and mid = false means up
    mid = true;
    wait(0.2, sec);
    autoBottomIntakeSpeed = 120.f;
    wait(0.8, sec);
    // go back to storing
    autoBottomIntakeSpeed = 0.f;
    mid = false;
    autoBottomIntakeSpeed = 120.f;

    Chassis.driveMaxTime = 1300;
    Chassis.driveMaxOutputVolts = 12;
    // drive towards second long goal

    
    Chassis.turnMaxOutputVolts = 11;
    Chassis.driveMaxTime = 1500;
    // Chassis.drive_inches(40, 180);
    Chassis.drive_inches(54.5,135);
    Chassis.turn_to_angle(90);
    Chassis.driveMaxTime = 500;
    Chassis.matchload(11.1, 90, 900);

    //Chassis.drive_inches_from_wall(16, 0, 1);
    // turn to face second match loader
    Chassis.driveSettleTime = 1500;
    Chassis.driveMaxOutputVolts = 12;
    // use distance sensor to align with matchloader and load
    //Chassis.matchload(11.3, -90, 750);
    //Chassis.driveMaxOutputVolts = 10;
    //Chassis.driveMaxTime = 1000;
    // back into long goal
    Chassis.driveMaxTime = 700;
    Chassis.drive_inches(-35, 90);
    // score until time runs out
    autoTopIntakeSpeed = 120.f;
}

void RBCrossover()
{
    // scraper = false; 
    Chassis.turn_to_angle(135);
    Chassis.driveMaxTime = 5000;
    Chassis.turnMaxOutputVolts = 4;
    Chassis.drive_inches(-100, 180);
    Chassis.turnMaxOutputVolts = 12;

    Chassis.turnMaxTime = 2000; 
    Chassis.turn_to_angle(90);

    Chassis.distanceMaxTime = 2000;
    wait(0.2, sec);
    Chassis.drive_inches_from_wall(17.75, 90, 0);
    Chassis.turn_to_angle(0);

    Chassis.driveMaxTime = 550;
    Chassis.drive_inches(-30);
    autoTopIntakeSpeed = 120.f;
    wait(2.5, sec);
    Chassis.turn_to_angle(0);

    autoTopIntakeSpeed = 0.f;
    Chassis.driveMaxOutputVolts = 8.f; 
    scraper = true;
    // Chassis.drive_inches(20, 0);
    Chassis.matchload(11.2, 0, 3200);
    Chassis.driveMaxOutputVolts = 10.f; 

    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(-50);
    autoTopIntakeSpeed = 120.f;
    wait(2.5, sec);
}

void score()
{
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    autoTopIntakeSpeed = -120.f;
    autoBottomIntakeSpeed = 120.f;
    scraper = true;
    Chassis.matchload(11.3, 0, 5000);
}

void clearPark()
{
    scraper = false;

    Chassis.turnMaxOutputVolts = 4;
    Chassis.drive_inches(55, 90);

    Chassis.driveMaxOutputVolts = 6.5f;
    Chassis.driveMaxTime = 5000;

    autoTopIntakeSpeed = -6.f;
    autoBottomIntakeSpeed = 120.f;

    Chassis.drive_inches(52.5, 85,
                         {18}, {[](void)
                                { scraper = true; }});
    Chassis.turnMaxOutputVolts = 12.f;
}

void skills()
{
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    mid = false;
    descoreLeft = true;
    autoTopIntakeSpeed = -6.f;
    autoBottomIntakeSpeed = 120.f;

    Chassis.driveMaxOutputVolts = 6.f;

    // Chassis.drive_inches_from_wall(24.f,0);

    Chassis.turn_to_angle(-22.5);
    Chassis.drive_inches(25.f, Chassis.get_heading(), {10, 24}, {[](void)
                                                                 { Chassis.driveMaxOutputVolts = 3.f; }, [](void)
                                                                 { scraper = true; }});
    // Chassis.drive_inches(28.f);
    // scraper = true;
    Chassis.driveMaxOutputVolts = 5.f;
    Chassis.turn_to_angle(-135);
    Chassis.driveMaxTime = 1500;
    Chassis.drive_inches(-26);
    Chassis.driveMaxTime = 3000;
    autoBottomIntakeSpeed = -12.f;
    mid = true;
    wait(0.2, sec);
    autoBottomIntakeSpeed = 120.f;
    wait(0.4, sec);
    Chassis.turn_to_angle(-135);
    autoBottomIntakeSpeed = 0.f;
    mid = false;
    Chassis.driveMaxOutputVolts = 8.f;

    // Chassis.turnMaxOutputVolts = 4.f;
    // Chassis.drive_inches_from_wall(20, 1);

    Chassis.driveMaxTime = 5000;
    // Chassis.drive_inches(49.5);
    Chassis.driveSettleTime = 300;
    Chassis.driveSettleError = 0.3;
    // Chassis.drive_inches(51.75);
    Chassis.drive_inches(55.65);
    Chassis.driveSettleError = 0.5;
    Chassis.driveSettleTime = 100;
    Chassis.turn_to_angle(180);
    // Chassis.driveMaxTime = 750;
    // Chassis.drive_inches(-35);
    // Chassis.driveMaxOutputVolts = 10.f;
    // autoTopIntakeSpeed = 120.f;
    // autoBottomIntakeSpeed = 120.f;
    // wait(1.5, sec);

    // Chassis.turnMaxTime = 2000; 
    // Chassis.turnSettleTime = 200; 
    // Chassis.turn_to_angle(180); 

    // autoTopIntakeSpeed = 0;
    // // Chassis.matchload(11.3, -180, 3200);

    // Chassis.driveMaxOutputVolts = 8.f; 
    // Chassis.drive_inches(20, 180);
    // Chassis.driveMaxOutputVolts = 10.f; 
    Chassis.matchload(11.3, -180, 3200);

    RBCrossover();

    Chassis.driveMaxTime = 1800;
    clearPark();

    Chassis.distanceMaxOutputVolts = 6.f;
    Chassis.drive_inches_from_wall(50, 90, 1);
    Chassis.distanceMaxOutputVolts = 8.f;
    Chassis.turn_to_angle(180);
    Chassis.drive_inches_from_wall(48, 180, 0);
    Chassis.turn_to_angle(45);
    Chassis.driveMaxOutputVolts = 5;
    Chassis.driveMaxTime = 750;
    Chassis.drive_inches(-20);

    mid = true;
    Chassis.drive_inches(0.4);
    wait(0.2, sec);
    autoBottomIntakeSpeed = 120.f;
    wait(4, sec);
    autoBottomIntakeSpeed = 0.f;
    mid = false;
    Chassis.driveMaxOutputVolts = 8.f;
    Chassis.driveMaxTime = 5000;
    Chassis.driveSettleTime = 300;
    Chassis.drive_inches(55.55);
    Chassis.turn_to_angle(0);
    Chassis.set_heading(180 - Chassis.get_heading());
    autoBottomIntakeSpeed = 120.f;
    Chassis.matchload(11.3, 180, 2900);

    RBCrossover();

    Chassis.drive_inches(55, 90);
    Chassis.drive_inches(20, 85);
}

void worseSkills()
{
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    mid = false;
    scraper = true;

    autoTopIntakeSpeed = -120.f;
    autoBottomIntakeSpeed = 120.f;

    Chassis.set_heading(90);
    Chassis.drive_inches_from_wall(17.75, 90, 0);
    Chassis.turn_to_angle(180);
    Chassis.matchload(11.3, 180, 2900);

    RBCrossover();
    Chassis.drive_inches(10);
    Chassis.turn_to_angle(-90);
    Chassis.drive_inches_from_wall(17.75, -90, 0);
    Chassis.turn_to_angle(0);

    Chassis.set_heading(180 - Chassis.get_heading());
    Chassis.matchload(11.2, 180, 2900);

    RBCrossover();

    Chassis.turnMaxOutputVolts = 2.f; 
    Chassis.drive_inches(55, 90);
    Chassis.driveMaxTime = 4000;
    Chassis.drive_inches(30);
}
void SkillsControlMiddle()
{
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
    wait(3, sec);
    mid = false;
}

// not aligned no constant block
void rightSplit()
{
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
    Chassis.matchload(11.3, 90, 800);

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
    Chassis.drive_inches(23.3, 220);
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
    Chassis.drive_inches(16, -90);
}
// aligned, constant block, untuned
void right9ball()
{
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.turnMaxTime = 1500;
    Chassis.driveMaxTime = 800;
    Chassis.turnSettleTime = 200;
    Chassis.driveSettleTime = 700;
    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f;
    scraper = false;
    descoreLeft = true;

    Chassis.turn_to_angle(15);
    Chassis.driveMaxOutputVolts = 11;
    Chassis.drive_inches(28.f, Chassis.get_heading(), {20, 24}, {[](void)
                                                                 { Chassis.driveMaxOutputVolts = 3.f; }, [](void)
                                                                 { scraper = false; }});
    Chassis.driveMaxOutputVolts = 9;
    Chassis.driveMaxTime = 1000;
    Chassis.turnMaxOutputVolts = 6;
    Chassis.turn_to_angle(55);
    Chassis.drive_inches(24.f, Chassis.get_heading(), {20}, {[](void)
                                                             { scraper = true; }});
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-35, -90);
    Chassis.drive_inches_from_wall(18.25, -90, 0);
    scraper = true;
    Chassis.turn_to_angle(180);
    Chassis.matchload(11.3, 180, 1400);
    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(-29);
    autoTopIntakeSpeed = 120.f;
}
// aligned constant block
void right4ballcorner()
{
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
                                                                 { Chassis.driveMaxOutputVolts = 3.f; }, [](void)
                                                                 { scraper = true; }});
    Chassis.driveMaxOutputVolts = 6;
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-34, -90);
    Chassis.turnMaxOutputVolts = 10;
    Chassis.drive_inches_from_wall(17, -90, 0);
    Chassis.turn_to_angle(180);
    Chassis.driveMaxOutputVolts = 8;
    Chassis.drive_inches(-24);
    autoTopIntakeSpeed = 120.f;
    wait(1.3, sec);
    scraper = false;
    Push();
}
// aligned, constant block
void right4ballmatchload()
{

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

    Chassis.drive_inches(35, 0);
    Chassis.drive_inches_from_wall(19, 0, 1);
    Chassis.turn_to_angle(90);
    scraper = true;
    Chassis.matchload(11.3, 90, 700);
    Chassis.drive_inches(-34);
    autoTopIntakeSpeed = 120.f;
    wait(1.3, sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;
    Push();
}
// aligned constant block
void right7ball()
{
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.turnMaxTime = 800;
    Chassis.driveMaxTime = 1100;
    Chassis.turnSettleTime = 600;
    Chassis.driveSettleTime = 700;
    autoBottomIntakeSpeed = 120.f;
    autoTopIntakeSpeed = -6.f;
    scraper = false;
    descoreLeft = true;

    Chassis.turn_to_angle(15);
    Chassis.drive_inches(32.f, Chassis.get_heading(), {18, 24}, {[](void)
                                                                 { Chassis.driveMaxOutputVolts = 8.f; }, [](void)
                                                                 { scraper = true; }});
    Chassis.driveMaxTime = 700;
    Chassis.turnMaxOutputVolts = 7;
    Chassis.drive_inches(-33, -90);
    Chassis.turnMaxOutputVolts = 10;
    // Chassis.drive_inches_from_wall(17.25, -90, 0);
    Chassis.drive_inches_from_wall(16.5, -90, 0);
    scraper = true;
    Chassis.turn_to_angle(180);
    Chassis.turnMaxOutputVolts = 7;
    Chassis.matchload(11.2, 180, 1100);
    Chassis.turn_to_angle(-180);
    Chassis.driveMaxOutputVolts = 8;
    Chassis.drive_inches(-36);
    autoTopIntakeSpeed = 120.f;
    wait(2.2, sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;
    Push();
}
// theory coded, aligned, constant block
void right10ball()
{
    // modular constant block
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

    // 4 block matchload code
    // drive forward to align to wall
    Chassis.drive_inches(35, 0);
    Chassis.drive_inches_from_wall(19, 0, 1);
    // turn to face matchloader and load
    Chassis.turn_to_angle(90);
    scraper = true;
    // matchloader distance sensor alignment function
    Chassis.matchload(11.3, 90, 700);
    // drive into long goal
    Chassis.drive_inches(-34);
    autoTopIntakeSpeed = 120.f;
    wait(1.3, sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;

    // solo awp code without the long goal scoring
    Chassis.turn_to_angle(-10);
    Chassis.driveMaxOutputVolts = 10;
    Chassis.driveMaxTime = 1100;
    // drive across corner blocks and collect
    Chassis.drive_inches(54.5, 0);
    // put scraper down to prevent blocks from escaping
    scraper = true;
    // turn to face middle goal and drive into it
    Chassis.turn_to_angle(-45);
    Chassis.driveMaxTime = 800;
    Chassis.drive_inches(-17, -45);
    // outtake to stop jamming on mid goal
    autoBottomIntakeSpeed = -120.f;
    wait(0.1, sec);
    autoBottomIntakeSpeed = 120.f;

    // spin hood backwards full speed to get any blocks out of hood and score
    autoTopIntakeSpeed = -12.f;
    autoBottomIntakeSpeed = -12.f;
    // middle goal piston is inverted, so mid = true means down and mid = false means up
    mid = true;
    wait(0.2, sec);
    autoBottomIntakeSpeed = 120.f;
}

void leftSplit()
{
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    mid = false;
    descoreLeft = true;
    autoTopIntakeSpeed = -6.f;
    autoBottomIntakeSpeed = 120.f;

    Chassis.driveMaxOutputVolts = 6.f;

    // Chassis.drive_inches_from_wall(24.f,0);

    Chassis.turn_to_angle(-22.5);
    Chassis.drive_inches(25.f, Chassis.get_heading(), {10, 24}, {[](void)
                                                                 { Chassis.driveMaxOutputVolts = 3.f; }, [](void)
                                                                 { scraper = true; }});
    // Chassis.drive_inches(28.f);
    // scraper = true;
    Chassis.driveMaxOutputVolts = 5.f;
    Chassis.turn_to_angle(-135);
    Chassis.driveMaxTime = 1500;
    Chassis.drive_inches(-26);
    Chassis.driveMaxTime = 3000;
    autoBottomIntakeSpeed = -12.f;
    mid = true;
    wait(0.2, sec);
    autoBottomIntakeSpeed = 120.f;
    wait(1, sec);
    Chassis.turn_to_angle(-135);
    autoBottomIntakeSpeed = 0.f;
    mid = false;
    Chassis.driveMaxOutputVolts = 8.f;

    // Chassis.turnMaxOutputVolts = 4.f;
    // Chassis.drive_inches_from_wall(20, 1);

    Chassis.driveMaxTime = 5000;
    // Chassis.drive_inches(49.5);
    Chassis.driveSettleTime = 300;
    Chassis.driveSettleError = 0.3;
    // Chassis.drive_inches(51.75);
    Chassis.drive_inches(55.65);
    Chassis.driveSettleError = 0.5;
    Chassis.driveSettleTime = 100;
    Chassis.turn_to_angle(180);
    // Chassis.driveMaxTime = 750;
    // Chassis.drive_inches(-35);
    // Chassis.driveMaxOutputVolts = 10.f;
    // autoTopIntakeSpeed = 120.f;
    // autoBottomIntakeSpeed = 120.f;
    // wait(1.5, sec);

    // Chassis.turnMaxTime = 2000; 
    // Chassis.turnSettleTime = 200; 
    // Chassis.turn_to_angle(180); 

    // autoTopIntakeSpeed = 0;
    // // Chassis.matchload(11.3, -180, 3200);

    // Chassis.driveMaxOutputVolts = 8.f; 
    // Chassis.drive_inches(20, 180);
    // Chassis.driveMaxOutputVolts = 10.f; 
    autoBottomIntakeSpeed = 120.f;
    Chassis.matchload(11.3, -180, 2000);
    Chassis.driveMaxTime = 950; 
    Chassis.driveMaxOutputVolts = 8.f;
    Chassis.drive_inches(-55,180);
    autoTopIntakeSpeed = 120.f;

}
void left7ball()
{
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

    Chassis.turn_to_angle(-15);
    // might not be the right angle
    Chassis.drive_inches(28.f, Chassis.get_heading(), {10, 24}, {[](void)
                                                                 { Chassis.driveMaxOutputVolts = 3.f; }, [](void)
                                                                 { scraper = true; }});
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-34, 90);
    Chassis.turnMaxOutputVolts = 10;
    Chassis.drive_inches_from_wall(17, 90, 0);
    scraper = true;
    Chassis.turn_to_angle(180);
    Chassis.turnMaxOutputVolts = 7;
    Chassis.matchload(11.3, 180, 700);
    Chassis.turn_to_angle(-180);
    Chassis.driveMaxOutputVolts = 8;
    Chassis.drive_inches(-36);
    autoTopIntakeSpeed = 120.f;
    wait(2.2, sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;
    Chassis.turn_to_angle(120);
    Chassis.drive_inches(8.5);
    Chassis.turn_to_angle(180);
    descoreLeft = false;
    Chassis.drive_inches(-28);
}

void left4ballcorner()
{
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

    Chassis.turn_to_angle(-15);
    Chassis.drive_inches(28.f, Chassis.get_heading(), {10, 24}, {[](void)
                                                                 { Chassis.driveMaxOutputVolts = 3.f; }, [](void)
                                                                 { scraper = true; }});
    Chassis.driveMaxOutputVolts = 6;
    Chassis.turnMaxOutputVolts = 6;
    Chassis.drive_inches(-34, 90);
    Chassis.turnMaxOutputVolts = 10;
    Chassis.drive_inches_from_wall(17, 90, 0);
    Chassis.turn_to_angle(180);
    Chassis.driveMaxOutputVolts = 8;
    Chassis.drive_inches(-24);
    autoTopIntakeSpeed = 120.f;
    wait(1.3, sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;
    Push();
}

void left4ballmatchload()
{
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

    Chassis.drive_inches(35, 0);
    Chassis.drive_inches_from_wall(19, 0, 1);
    Chassis.turn_to_angle(-90);
    scraper = true;
    Chassis.matchload(11.3, -90, 700);
    Chassis.drive_inches(-34);
    autoTopIntakeSpeed = 120.f;
    wait(1.3, sec);
    scraper = false;
    autoTopIntakeSpeed = -6.f;
    Push();
}

void test()
{
    scraper = false;
    vex::task antiJamBottom = vex::task(bottomAntiJam);
    vex::task antiJamTop = vex::task(topAntiJam);
    autoTopIntakeSpeed = 120.f;
    autoBottomIntakeSpeed = 120.f;
    scraper = true;

    Chassis.matchload(11.3, 0, 2900);
}
