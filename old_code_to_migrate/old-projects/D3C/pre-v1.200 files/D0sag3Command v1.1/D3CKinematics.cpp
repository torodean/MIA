/*
 * D3CKinematics.cpp
 *
 *  Created on: Oct 7, 2014
 *      Author: antonius
 */

#include <iostream>
#include "D3CKinematics.h"
#include <math.h>

namespace std {

D3CKinematics::D3CKinematics() : dimension(0), decideCase(0){
    // TODO Auto-generated constructor stub
}

//=====================================================================================
//=====================================================================================
// 2 Dimensional kinematic equations
//=====================================================================================
//=====================================================================================
void D3CKinematics::finalPositionWithConstantAcceleration1d(double initialPosition, double initialVelocity, double acceleration, double time){
    finalPosition = initialPosition + (initialVelocity*time) + (acceleration*time*time)/2;
    knowFinalPosition = true;
}
void D3CKinematics::finalPositionWithConstantVelocity1d(double initialPosition, double constantVelocity, double time){
    finalPosition = initialPosition + (constantVelocity*time);
    knowFinalPosition = true;
}
void D3CKinematics::initialPositionWithConstantAcceleration1d(double finalPosition, double initialVelocity, double acceleration, double time){
    initialPosition = finalPosition - (initialVelocity*time) - (acceleration*time*time)/2;
    knowInitialPosition = true;
}
void D3CKinematics::initialPositionWithConstantVelocity1d(double finalPosition, double constantVelocity, double time){
    initialPosition = finalPosition - (constantVelocity*time);
    knowInitialPosition = true;
}
void D3CKinematics::finalVelocityWithConstantAcceleration1d(double initialVelocity, double acceleration, double time){
    finalVelocity = initialVelocity + acceleration*time;
    knowFinalVelocity = true;
}
void D3CKinematics::averageAcceleration1d(double finalVelocity, double initialVelocity, double time){
    acceleration = (finalVelocity - initialVelocity)/time;
    knowAcceleration = true;
}
void D3CKinematics::initialVelocityWithConstantAcceleration1d(double finalVelocity, double acceleration, double time){
    initialVelocity = finalVelocity - acceleration*time;
    knowInitialVelocity = true;
}
void D3CKinematics::totalTimeWithConstantAcceleration1d(double initialVelocity, double finalVelocity, double acceleration){
    totalTime = (finalVelocity - initialVelocity)/acceleration;
    knowTime = true;
}
void D3CKinematics::totalTimeWithConstantVelocity1d(double constantVelocity, double initialPosition, double finalPosition){
    totalTime = (finalPosition - initialPosition)/constantVelocity;
    knowTime = true;
}
void D3CKinematics::initialVelocityWithConstantAcceleration1d(double finalPosition, double initialPosition, double acceleration, double time){
    initialVelocity = (finalPosition - initialPosition - (acceleration*time*time)/2)/time;
    knowInitialVelocity = true;
}
void D3CKinematics::acceleration1d(double finalPosition, double initialPosition, double initialVelocity, double time){
    acceleration = ((finalPosition - initialPosition - initialVelocity*time)*2)/(time*time);
    knowAcceleration = true;
}
void D3CKinematics::totalTime1d(double initialVelocity, double initialPosition, double finalPosition, double acceleration){
    totalTime = (-initialVelocity + sqrt(initialVelocity*initialVelocity - 2*(initialPosition - finalPosition)*acceleration))/(2*(initialPosition - finalPosition));
    totalTime2 = (-initialVelocity - sqrt(initialVelocity*initialVelocity - 2*(initialPosition - finalPosition)*acceleration))/(2*(initialPosition - finalPosition));
    knowTime = true;
}
void D3CKinematics::displacement1d(double initialPosition, double finalPosition){
    displacement = finalPosition - initialPosition;
    knowDisplacement = true;
}
void D3CKinematics::constantVelocity1d(double initialPosition, double finalPosition, double time){
    constantVelocity = (finalPosition - initialPosition)/time;
    knowConstantVelocity = true;
}
void D3CKinematics::finalPosition1d(double initialPosition, double initialVelocity, double finalVelocity, double time){
    finalPosition = ((initialVelocity + finalVelocity)*time)/2 + initialPosition;
    knowFinalPosition = true;
}
void D3CKinematics::initialPosition1d(double finalPosition, double initialVelocity, double finalVelocity, double time){
    initialPosition = finalPosition - ((initialVelocity + finalVelocity)*time)/2;
    knowInitialPosition = true;
}
void D3CKinematics::finalVelocityWithConstantAcceleration1dAlternative(double initialVelocity, double acceleration, double displacement){
    finalVelocity = sqrt(initialVelocity*initialVelocity + 2*acceleration*(displacement));
    knowFinalVelocity = true;
}
void D3CKinematics::initialVelocityWithConstantAcceleration1dAlternative(double finalVelocity, double acceleration, double displacement){
    initialVelocity = sqrt(finalVelocity*finalVelocity - 2*acceleration*(displacement));
    knowInitialVelocity = true;
}
void D3CKinematics::acceleration1dAlternative(double initialVelocity, double finalVelocity, double displacement){
    acceleration = (finalVelocity*finalVelocity - initialVelocity*initialVelocity)/(2*(displacement));
    knowAcceleration = true;
}
void D3CKinematics::time1d(double displacement, double initialVelocity, double finalVelocity){
    totalTime = 2*(displacement)/(initialVelocity + finalVelocity);
    knowTime = true;
}
void D3CKinematics::finalPositionWithConstantAcceleration1dAlternative(double finalVelocity, double initialVelocity, double acceleration, double initialPosition){
    finalPosition = (finalVelocity*finalVelocity - initialVelocity*initialVelocity)/(2*acceleration) + initialPosition;
    knowFinalPosition = true;
}
void D3CKinematics::initialPositionWithConstantAcceleration1dAlternative(double finalVelocity, double initialVelocity, double acceleration, double finalPosition){
    initialPosition = finalPosition - (finalVelocity*finalVelocity - initialVelocity*initialVelocity)/(2*acceleration);
    knowInitialPosition = true;
}
void D3CKinematics::initialVelocity1d(double displacement, double finalVelocity, double time){
    initialVelocity = (2*displacement)/time - finalVelocity;
    knowInitialVelocity = true;
}
void D3CKinematics::finalVelocity1d(double displacement, double initialVelocity, double time){
    finalVelocity = (2*displacement)/time - initialVelocity;
    knowFinalVelocity = true;
}
void D3CKinematics::changeInTime(double initialTime, double finalTime){
    totalTime = finalTime - initialTime;
    knowTime = true;
}
//=====================================================================================
//=====================================================================================
// end of 2 Dimensional kinematic equations
//=====================================================================================
//=====================================================================================

double D3CKinematics::getDisplacement(){
    return displacement;
}
double D3CKinematics::getFinalVelocity(){
    return finalVelocity;
}
double D3CKinematics::getInitialVelocity(){
    return initialVelocity;
}
double D3CKinematics::getInitialPosition(){
    return initialPosition;
}
double D3CKinematics::getFinalPosition(){
    return finalPosition;
}
double D3CKinematics::getAcceleration(){
    return acceleration;
}
double D3CKinematics::getInitialTime(){
    return initialTime;
}
double D3CKinematics::getFinalTime(){
    return finalTime;
}
double D3CKinematics::getTime(){
    return totalTime;
}
double D3CKinematics::getVelocity(){
    return velocity;
}
double D3CKinematics::getInitialAcceleration(){
    return initialAcceleration;
}
double D3CKinematics::getFinalAcceleration(){
    return finalAcceleration;
}

void D3CKinematics::kinematicsRunner1d() {

        if(knowInitialTime == true && knowFinalTime == true){
            changeInTime(initialTime, finalTime);
            knowTime = true;
        }

        if (knowInitialPosition == true && knowAcceleration == true	&& knowInitialVelocity == true && knowTime == true) {
            decideCase = 1;
        } else if (knowFinalPosition == true && knowAcceleration == true && knowInitialVelocity == true && knowTime == true) {
            decideCase = 2;
        } else if (knowFinalVelocity == true && knowInitialVelocity == true && knowAcceleration == true) {
            decideCase = 3;
        } else if (knowInitialPosition == true && knowFinalPosition == true) {
            decideCase = 4;
        } else if (knowFinalVelocity == true && knowInitialVelocity == true && knowTime == true) {
            decideCase = 5;
        } else if (knowConstantVelocity == true && knowInitialPosition == true && knowTime == true) {
            decideCase = 6;
        } else if (knowFinalPosition == true && knowInitialPosition == true && knowTime == true) {
            decideCase = 7;
        } else if (knowInitialVelocity == true && knowAcceleration == true && knowTime == true) {
            decideCase = 8;
        } else if (knowFinalVelocity == true && knowAcceleration == true && knowTime == true) {
            decideCase = 9;
        } else if (knowConstantVelocity == true && knowFinalPosition == true && knowTime == true) {
            decideCase = 10;
        } else if (knowConstantVelocity == true && knowFinalPosition == true && knowInitialPosition == true) {
            decideCase = 11;
        } else if (knowInitialPosition == true && knowFinalPosition == true && knowAcceleration == true && knowTime == true) {
            decideCase = 12;
        } else if (knowFinalPosition == true && knowInitialPosition == true && knowInitialVelocity == true && knowTime == true) {
            decideCase = 13;
        } else if (knowInitialVelocity == true && knowInitialPosition == true && knowFinalPosition == true && knowAcceleration == true) {
            decideCase = 14;
        } else if (knowInitialVelocity == true && knowAcceleration == true && knowInitialPosition == true && finalPosition == true) {
            decideCase = 15;
        } else if (knowFinalVelocity == true && knowAcceleration == true && knowInitialPosition == true && finalPosition == true) {
            decideCase = 16;
        } else if (knowFinalVelocity == true && knowInitialVelocity == true && knowInitialPosition == true && finalPosition == true) {
            decideCase = 17;
        } else if (knowFinalVelocity == true && knowInitialVelocity == true && knowInitialPosition == true && knowTime == true) {
            decideCase = 18;
        } else if (knowFinalVelocity == true && knowInitialVelocity == true && knowFinalPosition == true && knowTime == true) {
            decideCase = 19;
        } else if (knowFinalVelocity == true && knowInitialVelocity == true && knowInitialPosition == true && knowFinalPosition == true) {
            decideCase = 20;
        } else if (knowFinalVelocity == true && knowInitialVelocity == true && knowInitialPosition == true && knowAcceleration == true) {
            decideCase = 21;
        } else if (knowFinalVelocity == true && knowInitialVelocity == true && knowFinalPosition == true && knowAcceleration == true) {
            decideCase = 22;
        } else if (knowFinalVelocity == true && knowFinalPosition == true && knowInitialPosition == true && knowTime == true) {
            decideCase = 23;
        } else if (knowInitialVelocity == true && knowFinalPosition == true && knowInitialPosition == true && knowTime == true) {
            decideCase = 24;
        }

        switch (decideCase) {
        case 1:
            finalPositionWithConstantAcceleration1d(initialPosition, initialVelocity, acceleration, totalTime);
            finalVelocityWithConstantAcceleration1d(initialVelocity, acceleration, totalTime);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 2:
            initialPositionWithConstantAcceleration1d(finalPosition, initialVelocity, acceleration, totalTime);
            finalVelocityWithConstantAcceleration1d(initialVelocity, acceleration, totalTime);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 3:
            totalTimeWithConstantAcceleration1d(initialVelocity, finalVelocity, acceleration);
            initialPosition = 0;
            finalPositionWithConstantAcceleration1d(initialPosition, initialVelocity, acceleration, totalTime);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 4:
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Displacement = " << displacement << endl;
            break;
        case 5:
            averageAcceleration1d(finalVelocity, initialVelocity, totalTime);
            initialPosition = 0;
            finalPositionWithConstantAcceleration1d(initialPosition, initialVelocity, acceleration, totalTime);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 6:
            finalPositionWithConstantVelocity1d(initialPosition, constantVelocity, totalTime);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = 0" << endl;
            cout << "Constant velocity = " << constantVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 7:
            constantVelocity1d(initialPosition, finalPosition, totalTime);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = 0" << endl;
            cout << "Constant velocity = " << constantVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 8:
            finalVelocityWithConstantAcceleration1d(initialVelocity, acceleration, totalTime);
            initialPosition = 0;
            finalPositionWithConstantAcceleration1d(initialPosition, initialVelocity, acceleration, totalTime);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 9:
            initialVelocityWithConstantAcceleration1d(finalVelocity, acceleration, totalTime);
            initialPosition = 0;
            finalPositionWithConstantAcceleration1d(initialPosition, initialVelocity, acceleration, totalTime);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 10:
            initialPositionWithConstantVelocity1d(finalPosition, constantVelocity, totalTime);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = 0" << endl;
            cout << "Constant velocity = " << constantVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 11:
            totalTimeWithConstantVelocity1d(constantVelocity, initialPosition, finalPosition);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = 0" << endl;
            cout << "Constant velocity = " << constantVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 12:
            initialVelocityWithConstantAcceleration1d(finalPosition, initialPosition, acceleration, totalTime);
            displacement1d(initialPosition, finalPosition);
            finalVelocityWithConstantAcceleration1d(initialVelocity, acceleration, totalTime);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 13:
            acceleration1d(finalPosition, initialPosition, initialVelocity, totalTime);
            displacement1d(initialPosition, finalPosition);
            finalVelocityWithConstantAcceleration1d(initialVelocity, acceleration, totalTime);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 14:
            totalTime1d(initialVelocity, initialPosition, finalPosition, acceleration);
            finalVelocityWithConstantAcceleration1d(initialVelocity, acceleration, totalTime);
            displacement1d(initialPosition, finalPosition);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time 1 = " << totalTime << endl;
            cout << "Total time 2 = " << totalTime2 << endl;
            break;
        case 15:
            displacement1d(initialPosition, finalPosition);
            finalVelocityWithConstantAcceleration1dAlternative(initialVelocity, acceleration, displacement);
            totalTimeWithConstantAcceleration1d(initialVelocity, finalVelocity, acceleration);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = +/-" << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 16:
            displacement1d(initialPosition, finalPosition);
            initialVelocityWithConstantAcceleration1dAlternative(finalVelocity, acceleration, displacement);
            totalTimeWithConstantAcceleration1d(initialVelocity, finalVelocity, acceleration);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = +/-" << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 17:
            displacement1d(initialPosition, finalPosition);
            acceleration1dAlternative(initialVelocity, finalVelocity, displacement);
            totalTimeWithConstantAcceleration1d(initialVelocity, finalVelocity, acceleration);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 18:
            finalPosition1d(initialPosition, initialVelocity, finalVelocity, totalTime);
            displacement1d(initialPosition, finalPosition);
            acceleration1dAlternative(initialVelocity, finalVelocity, displacement);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 19:
            initialPosition1d(finalPosition, initialVelocity, finalVelocity, totalTime);
            displacement1d(initialPosition, finalPosition);
            acceleration1dAlternative(initialVelocity, finalVelocity, displacement);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 20:
            displacement1d(initialPosition, finalPosition);
            acceleration1dAlternative(initialVelocity, finalVelocity, displacement);
            totalTimeWithConstantAcceleration1d(initialVelocity, finalVelocity, acceleration);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 21:
            finalPositionWithConstantAcceleration1dAlternative(finalVelocity, initialVelocity, acceleration, initialPosition);
            displacement1d(initialPosition, finalPosition);
            totalTimeWithConstantAcceleration1d(initialVelocity, finalVelocity, acceleration);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 22:
            initialPositionWithConstantAcceleration1dAlternative(finalVelocity, initialVelocity, acceleration, finalPosition);
            displacement1d(initialPosition, finalPosition);
            totalTimeWithConstantAcceleration1d(initialVelocity, finalVelocity, acceleration);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 23:
            displacement1d(initialPosition, finalPosition);
            initialVelocity1d(displacement, finalVelocity, totalTime);
            acceleration1dAlternative(initialVelocity, finalVelocity, displacement);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        case 24:
            displacement1d(initialPosition, finalPosition);
            finalVelocity1d(displacement, initialVelocity, totalTime);
            acceleration1dAlternative(initialVelocity, finalVelocity, displacement);
            cout << endl;
            cout << "Initial position = " << initialPosition << endl;
            cout << "Final position = " << finalPosition << endl;
            cout << "Acceleration = " << acceleration << endl;
            cout << "Initial Velocity = " << initialVelocity << endl;
            cout << "Final velocity = " << finalVelocity << endl;
            cout << "Displacement = " << displacement << endl;
            cout << "Total time = " << totalTime << endl;
            break;
        default:
            cout << "There is nothing to be solved for 'or' you entered an impossible scenario." << endl;
            break;
        }
}

D3CKinematics::~D3CKinematics() {
    // TODO Auto-generated destructor stub
}

} /* namespace std */
