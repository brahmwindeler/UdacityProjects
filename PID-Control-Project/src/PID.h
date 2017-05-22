#ifndef PID_H
#define PID_H

class PID {
protected:
  /*
  * Errors
  */
  double p_error = 0;
  double i_error = 0;
  double d_error = 0;
  double total_error = 0;

  int numOfSteps = 0;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  double dpp = 2./3.*Kp;
  double dpi = 2./3.*Ki;
  double dpd = .3;
  double bestErr = 1e9;

  int switchTweeker = 0;
  bool up = true;

public:
  /*
  * Constructor
  */
  PID(double Kp, double Ki, double Kd);

  /*
  * Destructor.
  */
  virtual ~PID();

  double getValue();

  int getNumberOfSteps() { return numOfSteps; }

  void setNumberOfSteps(int num) { numOfSteps = num; }

  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

  void twiddle();
};

#endif /* PID_H */
