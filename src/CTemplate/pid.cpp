#include "vex.h"

pid::pid(float kP, float kI, float kD, float updateTime, float maxTime, float settleTime, float settleError, float maxOutputVolts){
    this->kP = kP;
    this->kI = kI;
    this->kD = kD; 
    this->updateTime = max(1.f, updateTime);
    this->maxTime = maxTime;
    this->settleTime = settleTime;
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
    this->updateTime = max(1.f, updateTime);
    this->maxOutputVolts = maxOutputVolts; 
}

void pid::set_slew_constants(float slewStep){
    this->slewStep = fabs(slewStep);
}

float pid::slew(float targetOutput){
    if(targetOutput > prevOutput + slewStep){
        return prevOutput + slewStep;
    }

    if(targetOutput < prevOutput - slewStep){
        return prevOutput - slewStep;
    }

    return targetOutput;
}

float pid::compute(float error){
    float proportional = error;
    float rawDerivitive = (error - prevError) / updateTime;
    filteredDerivitive = smoothingStrength * filteredDerivitive + (1.0f - smoothingStrength) * rawDerivitive;

    errorBuildup += updateTime * (error + prevError) / 2;
    if(errorBuildup > maxOutputVolts){
        errorBuildup = maxOutputVolts; 
    }
    if(errorBuildup < -maxOutputVolts){
        errorBuildup = -maxOutputVolts; 
    }

    prevError = error;

    float output = proportional * kP + errorBuildup * kI + filteredDerivitive * kD;

    if(output > maxOutputVolts){
        output = maxOutputVolts; 
    }
    if(output < -maxOutputVolts){
        output = -maxOutputVolts; 
    }

    output = slew(output);
    prevOutput = output;

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

void pid::reset(){
    elapsedTime = 0.f;
    timeSettled = 0.f;
    prevError = 0.f;
    errorBuildup = 0.f;
    filteredDerivitive = 0.f;
    prevOutput = 0.f;
}