# CarND-Controls-PID
This repository is my implementation of the Udacity self-driving car project. The readme is mainly an extract of the one provided by Udacity.

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets) == 0.13, but the master branch will probably work just fine
  * Follow the instructions in the [uWebSockets README](https://github.com/uWebSockets/uWebSockets/blob/master/README.md) to get setup for your platform. You can download the zip of the appropriate version from the [releases page](https://github.com/uWebSockets/uWebSockets/releases). Here's a link to the [v0.13 zip](https://github.com/uWebSockets/uWebSockets/archive/v0.13.0.zip).
  * If you run OSX and have homebrew installed you can just run the ./install-mac.sh script to install this
* Simulator. You can download these from the [project intro page](https://github.com/udacity/CarND-PID-Control-Project/releases) in the classroom.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

## Determination of the PID values

The PID values were determined using initial values which are stable and then using the twiddle algorithm. The values are hereby updated if the mean squared error of the cte is smaller and no manual interaction happend. If manual interaction happens and it is early in the twiddling the twiddle is rested, else the value is skipped as the parameters are assumed unstable.

### Initial Values

#### P-Value
The P-Value gives the conversion factor from the current CTE error to the action (steering angle). As the CTE error is given in meters (which is approximatly between -5 and 5 on the pavement) and the steering angle as a value between -1 and 1, the P-value is expected to have an order of magnitude of approx. 0.1 as this would lead a steering angle of approx. 0.5 when being at the edge of the road.

#### I-Value
The I-Value gives the conversion from all accumulated CTE errors to the action (steering angle). As the integral CTE error is simply the sum of all the previous values which are handed approx. every 0.1s, the I-value is expected to be ten times smaller than the P-value, with an order of magnitude of 0.01. This change of the order of magnitude accounts for the normalising time to unit length, as the for the correct integration each summand has to be multiplied with length of the integration time, which is times 0.1.

#### D-Value
The D-Value gives the conversion from the derivative CTE errors to the action (steering angle). As the derivataive CTE error is approximated by the difference between the last two values which are handed with a difference of approx. 0.1s, the D-value is expected to be ten times bigger than the P-value, with an order of magnitude of 1. This change of the order of magnitude accounts for the normalising time to unit length, as the for the correct derivative the difference has be divided by the length of the integration time, which is times 10.

### Final Values
After running the twiddle algorithm the following final values were determined:
- The final P-Value is 0.071769, which is close to the expected order of magnitude of 0.1. 
- The final I-Value is 0.00411344, which is a bit smaller than a tenth of the P-value, but within the expected range.
- The final D-Value is 0.974954, which is close to ten times the P-value and as well in the expected range.

