#include "vex.h"


chassis::chassis(vex::motor_group left, vex::motor_group right, vex::inertial gyroscope, float gyroScale, float wheelDiameter, float gearRatio, vex::rotation verticalOdom, float verticalOdomDiameter, vex::distance leftDistance, vex::distance rightDistance, vex::distance frontDistance, vex::distance backDistance) : 
    left(left), 
    right(right), 
    gyroscope(gyroscope), 
    verticalOdom(verticalOdom), 
    leftDistance(leftDistance), 
    rightDistance(rightDistance), 
    frontDistance(frontDistance), 
    backDistance(backDistance)
    {
    this->gyroScale = gyroScale;
    this->wheelDiameter = wheelDiameter;
    this->gearRatio = gearRatio; 
}

void chassis::set_drive_constants(float driveP, float driveI, float driveD, float driveMaxTime, float driveSettleTime, float driveSettleError, float driveMaxOutputVolts){
    this->driveP = driveP;
    this->driveI = driveI;
    this->driveD = driveD;
    this->driveMaxTime = driveMaxTime;
    this->driveSettleTime = driveSettleTime;
    this->driveSettleError = driveSettleError;
    this->driveMaxOutputVolts = driveMaxOutputVolts;
}

void chassis::set_odom_constants(float odomP, float odomI, float odomD, float odomMaxTime, float odomSettleTime, float odomSettleError, float odomMaxOutputVolts){
    this->odomP = odomP;
    this->odomI = odomI;
    this->odomD = odomD;
    this->odomMaxTime = odomMaxTime;
    this->odomSettleTime = odomSettleTime;
    this->odomSettleError = odomSettleError;
    this->odomMaxOutputVolts = odomMaxOutputVolts;
}

void chassis::set_turn_constants(float turnP, float turnI, float turnD, float turnMaxTime, float turnSettleTime, float turnSettleError, float turnMaxOutputVolts){
    this->turnP = turnP;
    this->turnI = turnI;
    this->turnD = turnD;
    this->turnMaxTime = turnMaxTime;
    this->turnSettleTime = turnSettleTime;
    this->turnSettleError = turnSettleError;
    this->turnMaxOutputVolts = turnMaxOutputVolts; 
}

float chassis::get_heading(){
    return reduce_heading(gyroscope.rotation() * 360.f / gyroScale); 
}

float chassis::leftPositionInches(){
    /*
        distance = circumference * driver / driven * #rotations
    */
    return 3.14159 * wheelDiameter * gearRatio * left.position(degrees) / 360; 
}

float chassis::rightPositionInches(){
    return 3.14159 * wheelDiameter * gearRatio * right.position(degrees) / 360; 
}

void chassis::arcade(float driveVolt, float turnVolt){
    float throttle = deadband(controller(primary).Axis3.position(), 3) * driveVolt / 100.f;
    float turn = deadband(controller(primary).Axis1.position(), 3) * turnVolt / 100.f;

    right.spin(fwd, throttle - turn, volt);
    left.spin(fwd, throttle + turn, volt); 
}

void chassis::drive_inches(float distance){
    drive_inches(distance, get_heading(), {}, {}); 
}

void chassis::drive_inches(float distance, float heading){
    drive_inches(distance, heading, {}, {}); 
}

void chassis::drive_inches(float distance, float heading, std::vector<float> commandDistances,std::vector<std::function<void()>> commands){
    pid drivePID = pid(driveP, driveI, driveD, pidUpdateTime, driveMaxTime, driveSettleTime, driveSettleError, driveMaxOutputVolts); 
    pid turnPID = pid(turnP, turnI, turnD, pidUpdateTime, turnMaxTime, turnSettleTime, turnSettleError, turnMaxOutputVolts);

    float startPosition = (leftPositionInches() + rightPositionInches()) / 2.f;
    float averagePosition;
    float driveError, turnError;
    float driveVoltage, turnVoltage;

    int currentCommand = 0; 
    float direction = (distance >= 0) ? 1.0f : -1.0f;

    heading = reduce_heading(heading); 

    while (!drivePID.settled() || !turnPID.settled())
    {
        averagePosition = (leftPositionInches() + rightPositionInches()) / 2.f;
        driveError = distance + startPosition - averagePosition; //
        
        while(currentCommand < commands.size() && direction * (averagePosition - startPosition) >= direction * commandDistances[currentCommand]){
            commands[currentCommand]();
            currentCommand++; 
        }

        turnError = reduce_heading(heading - get_heading()); 
        if(turnError > 180) turnError -= 360; 

        driveVoltage = drivePID.compute(driveError);
        turnVoltage = turnPID.compute(turnError); 
        left.spin(fwd, driveVoltage + turnVoltage, volt);
        right.spin(fwd, driveVoltage - turnVoltage, volt);

        wait(pidUpdateTime, msec); 
    }

    left.stop(brake);
    right.stop(brake); 
}


void chassis::drive_inches_odom(float distance){
    drive_inches_odom(distance, get_heading()); 
}

void chassis::drive_inches_odom(float distance, float heading){
    pid drivePID = pid(driveP, driveI, driveD, pidUpdateTime, driveMaxTime, driveSettleTime, driveSettleError, driveMaxOutputVolts); //switch to odom?
    pid turnPID = pid(turnP, turnI, turnD, pidUpdateTime, turnMaxTime, turnSettleTime, turnSettleError, turnMaxOutputVolts);

    float startPosition = verticalOdom.position(deg) / 360 * verticalOdomDiameter * 3.14159;
    float currentPosition;
    float driveError, turnError;
    float driveVoltage, turnVoltage; 

    heading = reduce_heading(heading); 

    while (!drivePID.settled() || !turnPID.settled())
    {
        currentPosition = verticalOdom.position(deg) / 360 * verticalOdomDiameter * 3.14159;
        driveError = distance + startPosition - currentPosition; //

        turnError = reduce_heading(heading - get_heading()); 
        if(turnError > 180) turnError -= 360; 

        driveVoltage = drivePID.compute(driveError);
        turnVoltage = turnPID.compute(turnError); 
        left.spin(fwd, driveVoltage + turnVoltage, volt);
        right.spin(fwd, driveVoltage - turnVoltage, volt);

        wait(pidUpdateTime, msec); 
    }

    left.stop(brake);
    right.stop(brake); 
}


void chassis::turn_to_angle(float heading){
    pid turnPID = pid(turnP, turnI, turnD, pidUpdateTime, turnMaxTime, turnSettleTime, turnSettleError, turnMaxOutputVolts);
    float turnError, turnVoltage;

    heading = reduce_heading(heading); 

    while (!turnPID.settled())
    {
        turnError = reduce_heading(heading - get_heading());
        if(turnError > 180) turnError -= 360;
        turnVoltage = turnPID.compute(turnError);
        left.spin(fwd, turnVoltage, volt); 
        right.spin(fwd, -turnVoltage, volt); 
        wait(pidUpdateTime, msec); 
    }

    left.stop(brake); 
    right.stop(brake); 
}

void chassis::drive_inches_from_wall(float distance, Wall wall){
    drive_inches_from_wall(distance, get_heading(), wall); 
}

void chassis::drive_inches_from_wall(float distance, float heading, Wall wall){
    pid drivePID = pid(driveP, driveI, driveD, pidUpdateTime, driveMaxTime, driveSettleTime, driveSettleError, driveMaxOutputVolts); 
    pid turnPID = pid(turnP, turnI, turnD, pidUpdateTime, turnMaxTime, turnSettleTime, turnSettleError, turnMaxOutputVolts);
    float currentPosition;
    float driveError, turnError;
    float driveVoltage, turnVoltage; 

    heading = reduce_heading(heading); 

    while (!drivePID.settled() || !turnPID.settled())
    {
        if (wall == BACK_WALL){
            currentPosition = backDistance.objectDistance(vex::distanceUnits::in);
        } else if (wall = FRONT_WALL){
            currentPosition = frontDistance.objectDistance(vex::distanceUnits::in);
        }
        driveError = (wall == BACK_WALL) ? distance - currentPosition : currentPosition - distance;

        turnError = reduce_heading(heading - get_heading()); 
        if(turnError > 180) turnError -= 360; 

        driveVoltage = drivePID.compute(driveError);
        turnVoltage = turnPID.compute(turnError); 
        left.spin(fwd, driveVoltage + turnVoltage, volt);
        right.spin(fwd, driveVoltage - turnVoltage, volt);

        wait(pidUpdateTime, msec); 
    }

    left.stop(brake);
    right.stop(brake); 
}

void chassis::left_swing_to_angle(float heading){
    pid turnPID = pid(turnP, turnI, turnD, pidUpdateTime, turnMaxTime, turnSettleTime, turnSettleError, turnMaxOutputVolts);
    float turnError, turnVoltage;

    heading = reduce_heading(heading); 

    while (!turnPID.settled())
    {
        turnError = reduce_heading(heading - get_heading());
        if(turnError > 180) turnError -= 360;
        turnVoltage = 2 * turnPID.compute(turnError); //the turning is 1/2 as powerful because only one side is used, so we should double the sensititivty for fast turns
        left.spin(vex::fwd, turnVoltage, vex::volt);
        right.stop(vex::brake); 
        wait(pidUpdateTime, vex::msec);
    }

    left.stop(vex::brake); 
    right.stop(vex::brake); 
}
void chassis::right_swing_to_angle(float heading){
    pid turnPID = pid(turnP, turnI, turnD, pidUpdateTime, turnMaxTime, turnSettleTime, turnSettleError, turnMaxOutputVolts);
    float turnError, turnVoltage;

    heading = reduce_heading(heading); 

    while (!turnPID.settled())
    {
        turnError = reduce_heading(heading - get_heading());
        if(turnError > 180) turnError -= 360;
        turnVoltage = 2 * turnPID.compute(turnError); //the turning is 1/2 as powerful because only one side is used, so we should double the sensititivty for fast turns
        left.stop(vex::brake); 
        right.spin(vex::fwd, -turnVoltage, vex::volt);
        wait(pidUpdateTime, vex::msec);
    }

    left.stop(vex::brake); 
    right.stop(vex::brake); 
}

