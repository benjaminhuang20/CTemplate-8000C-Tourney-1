#pragma once

float reduce_heading(float heading); //reduce heading from -180 to 180
float clamp(float lower, float toClamp, float higher); //clamps the toClamp between lower and higher
int clamp(int lower, int toClamp, int higher);
float deadband(int num, float deadRange);
void headingPIDTuner(float Pincrement, float Iincrement, int Dincrement);
void drivePIDTuner(float Pincrement, float Iincrement, int Dincrement);