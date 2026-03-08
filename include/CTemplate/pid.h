#pragma once
#include <vex.h>

class pid{
    public:
        float kP = 0.f;
        float kI = 0.f;
        float kD = 0.f; 

        float updateTime = 10.f;
        float elapsedTime = 0.f;  
        float maxTime = 5000;
        float timeSettled = 0; 
        float settleTime = 150;
        float settleError = 1;

        float maxOutputVolts = 12;

        float prevError = 0; 
        float errorBuildup = 0;

        float filteredDerivitive = 0;
        float smoothingStrength = 0.9f;

        float prevOutput = 0.f;
        float slewStep = 0.3f;

        pid(float kP, float kI, float kD, float updateTime, float maxTime, float settleTime, float settleError, float maxOutputVolts);
        void set_pid_constants(float kP, float kI, float kD);
        void set_breakout_constants(float settleTime, float settleError, float maxTime);
        void set_settings_constants(float updateTime, float maxOutputVolts);
        void set_slew_constants(float slewStep);

        float slew(float targetOutput);
        float compute(float error);
        bool settled();
        void reset();
};