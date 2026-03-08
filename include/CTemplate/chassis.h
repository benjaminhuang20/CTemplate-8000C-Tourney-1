#pragma once
#include "vex.h"

enum Wall
{
    LEFT_WALL,
    RIGHT_WALL,
    FRONT_WALL,
    BACK_WALL
};
enum Distance
{
    LEFT_DISTANCE,
    RIGHT_DISTANCE,
    FRONT_DISTANCE,
    BACK_DISTANCE
};

class chassis{
    private:
        
    public:
        vex::motor_group left;
        vex::motor_group right;
        vex::inertial gyroscope;
        vex::rotation verticalOdom;
        vex::distance leftDistance;
        vex::distance rightDistance;
        vex::distance frontDistance; 
        vex::distance backDistance;

        float pidUpdateTime; 
        float driveP, driveI, driveD, driveMaxTime, driveSettleTime, driveSettleError, driveMaxOutputVolts, driveSlew;
        float odomP, odomI, odomD, odomMaxTime, odomSettleTime, odomSettleError, odomMaxOutputVolts, odomSlew; 
        float turnP, turnI, turnD, turnMaxTime, turnSettleTime, turnSettleError, turnMaxOutputVolts, turnSlew;
        float distanceP, distanceI, distanceD, distanceMaxTime, distanceSettleTime, distanceSettleError, distanceMaxOutputVolts, distanceSlew;

        float gyroScale;
        float wheelDiameter;
        float verticalOdomDiameter; 
        float gearRatio;


        chassis(vex::motor_group left, vex::motor_group right, vex::inertial gyroscope, float gyroScale, float wheelDiameter, float gearRatio, vex::rotation verticalOdom, float verticalOdomDiameter, vex::distance leftDistance, vex::distance rightDistance, vex::distance frontDistance, vex::distance backDistance);
        void set_drive_constants(float driveP, float driveI, float driveD, float driveMaxTime, float driveSettleTime, float driveSettleError, float driveMaxOutputVolts, float driveSlew);
        void set_odom_constants(float odomP, float odomI, float odomD, float odomMaxTime, float odomSettleTime, float odomSettleError, float odomMaxOutputVolts, float odomSlew);
        void set_turn_constants(float turnP, float turnI, float turnD, float turnMaxTime, float turnSettleTime, float turnSettleError, float turnMaxOutputVolts, float turnSlew);
        void set_distance_constants(float distanceP, float distanceI, float distanceD, float distanceMaxTime, float distanceSettleTime, float distanceSettleError, float distanceMaxOutputVolts, float distanceSlew);
        
        float get_heading();
        void set_heading(float heading); 
        float leftPositionInches();
        float rightPositionInches(); 

        void arcade(float driveVolt, float turnVolt);
        void drive_inches(float distance); 
        void drive_inches(float distance, float heading);
        void drive_inches(float distance, float heading, std::vector<float> commandDistances,std::vector<std::function<void()>> commands);
        
        void drive_inches_odom(float distance);
        void drive_inches_odom(float distance, float heading); 

        void drive_inches_distance(float distance);
        void drive_inches_distance(float distance, float heading); 

        void turn_to_angle(float heading);

        void drive_inches_from_wall(float distance, Wall wall); 
        void drive_inches_from_wall(float distance, float heading, Wall wall);

        void drive_inches_from_wall(float distance, int wall); 
        void drive_inches_from_wall(float distance, float heading, int wall);

        void matchload(float distance, float time);
        void matchload(float distance, float heading, float time); 

        void left_swing_to_angle(float heading); 
        void right_swing_to_angle(float heading); 
};
