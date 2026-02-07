#include "vex.h"

pid::pid(float kP, float kI, float kD, float updateTime, float maxTime, float settleTime, float settleError, float maxOutputVolts){
    this->kP = kP;
    this->kI = kI;
    this->kD = kD; 
    this->updateTime = max(1.f, updateTime); // makes sure that updateTime is always not 0
    this->maxTime = maxTime;
    this->settleTime = settleTime; //if you set this to zero you are a menace to society ._.
    this->settleError = settleError;
    this->maxOutputVolts = maxOutputVolts; 
}

void pid::set_pid_constants(float kP, float kI, float kD){
    this->kP = kP;
    this->kI = kI;
    this->kD = kD; 
}
void pid::set_breakout_constants(float settleTime, float settleError, float maxTime){
    this->settleTime = settleTime;
    this->settleError = settleError;
    this->maxTime = maxTime; 
}
void pid::set_settings_constants(float updateTime, float maxOutputVolts){
    this->updateTime = updateTime;
    this->maxOutputVolts = maxOutputVolts; 
}

float pid::compute(float error){
    float proportional = error;
    float derivitive = (error - prevError) / updateTime;

    errorBuildup += updateTime * (error + prevError) / 2;
    if(errorBuildup > maxOutputVolts){
        errorBuildup = maxOutputVolts; 
    }
    if(errorBuildup < -maxOutputVolts){
        errorBuildup = -maxOutputVolts; 
    }

    prevError = error;

    float output = proportional * kP + errorBuildup * kI + derivitive * kD;
    if(output > maxOutputVolts){
        output = maxOutputVolts; 
    }
    if(output < -maxOutputVolts){
        output = -maxOutputVolts; 
    }

    elapsedTime += updateTime; 

    if(fabs(error) <= settleError){
        timeSettled += updateTime; 
    } else{
        timeSettled = 0; 
    }

    return output;
}

bool pid::settled(){
    if(elapsedTime >= maxTime){
        return true; 
    }

    if(timeSettled >= settleTime){
        return true; 
    }

    return false; 
}