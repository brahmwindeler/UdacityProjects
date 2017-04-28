#include "PID.h"
#include <math.h>
#include <iostream>
#define MIN_NUMBER_STEPS 100

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID(double Kp, double Ki, double Kd) : Kp(Kp), Ki(Ki), Kd(Kd){}

PID::~PID() {}

void PID::UpdateError(double cte) {
	d_error = cte - p_error;
	p_error = cte;
	i_error += cte;
	if(numOfSteps>MIN_NUMBER_STEPS) total_error += pow(cte,2) ;//+ .0001*pow(getValue(),2);
	numOfSteps++;
}

double PID::getValue() {
	double steeringAngle = -Kp*p_error -Kd*d_error - Ki*i_error;
	steeringAngle = steeringAngle<-1?-1:steeringAngle;
	steeringAngle = steeringAngle>1?1:steeringAngle;
	return steeringAngle;
}

double PID::TotalError() {
	return total_error/(numOfSteps-MIN_NUMBER_STEPS);
}

void PID::twiddle() {
	double err = TotalError();
	int numSteps = numOfSteps;
	total_error = 0;
	numOfSteps = 0;
	if(bestErr > 1e8) {
		bestErr = err;
		std::cout << "New best Kp: " << Kp << " Kd: " << Kd
						<< " Ki: " << Ki << " Error: " << bestErr
						<< " Sum dpi: " << dpp + dpi + dpd << std::endl;
		Kp += dpp;
		return;
	}
	i_error = 0;
	d_error = 0;
	p_error = 0;
	if(err < bestErr && numSteps > 300) {
		bestErr = err;
		if(switchTweeker == 0) dpp *= 1.1;
		else if(switchTweeker == 1) dpi *= 1.1;
		else dpd *= 1.1;
		switchTweeker = (switchTweeker+1)%3;
		up = true;
		std::cout << "New best Kp: " << Kp << " Kd: " << Kd
				<< " Ki: " << Ki << " Error: " << bestErr
				<< " Sum dpi: " << dpp + dpi + dpd << std::endl;

	} else {
		std::cout << "Skipped Kp: " << Kp << " Kd: " << Kd
								<< " Ki: " << Ki << " Error: " << err
								<< " Sum dpi: " << dpp + dpi + dpd << std::endl;
		if(up == true) up = false;
		else {
			if (switchTweeker == 0) {
				Kp += dpp;
				dpp *= 0.9;
			}
			else if (switchTweeker == 1) {
				Ki += dpi;
				dpi *= .9;
			}
			else {
				Kd += dpd;
				dpd *= .9;
			}
			switchTweeker = (switchTweeker + 1) % 3;
			up = true;
		}
	}
	if(switchTweeker == 0) {
		if(up) Kp += dpp;
		else Kp-= 2*dpp;
	}
	if(switchTweeker == 1) {
		if(up) Ki += dpi;
		else Ki -= 2*dpi;
	}
	if(switchTweeker == 2) {
		if(up) Kd += dpd;
		else Kd-= 2*dpd;
	}
}

