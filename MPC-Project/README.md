# CarND-Controls-MPC
This folder contains my solution for the MPC project of the Self-Driving Car Engineer Nanodegree Program.

## Rubric Points

### Implementation

#### The Model

The model is quite similar to the one provided in the lecture. The state and the acuators remain the same. The big difference is, that the actuation from two timesteps before is used to calculate the change in the model. This is done to take the delay of the actuation into account and leads to the follwoing equations:

x_{t+1} = x_t + v_t * cos(psi_t) * dt
y_{t+1 = y_t + v_t * sin(psi_t) * dt
psi_{t+1} = psi_t + v_t/Lf * delta_{t-2} * dt
v_{t+1} = v_t + a_t-2 * dt;
cte_{t+1} = (f_t -y_t) + (v_t * sin(epsi_t) * dt);
epsi_{t+1} = (psi_t - psides_t) + v_t/Lf * delta_{t-2} * dt;

with:
f_t = coeffs[0] + coeffs[1] * x_t + coeffs[2] * x_t^2+ coeffs[3] * x_t^3;
psides_t = atan(coeffs[1]); 

To migrate a different sign convention in unity the calculated steering angle is multiplied by -1 before it is fed to the simulator.

#### Timestep Length and Frequency
The timestep length was choosen to be 15 and the frequency to be .05s. This provides good results, as it takes the trajectory within the next 0.75s into account when deciding upon the current steering angle. In addition the frequency of .05s should be relatively close to the frequency with which the environment of the car changes. 
A higher N led to longer computation time and oscillations in the steering, a smaller N let to delayed steerings in turns.
Lowering dt caused similar issues as increasing N and increasing dt similar issues as decreasing N.

#### Polynomial Fitting and MPC Preprocessing
Before fitting the polynomial the values provided by the simulator had to be converted into the reference frame of the car using:
ptsx[i] = (ptsx[i] - px)*cos(psi) + (ptsy[i] - py)*sin(psi);
ptsy[i] = (ptsy[i]- py)*cos(psi) - (ptsx[i] - px)*sin(psi);
psi = 0
In addition the received telemetry steering is handed with negative sign as the last actuation and the throttle is fed as the last acceleration.
Afterwards the ptsx and ptsy values are fitted using a polynomial of third order.

#### Model Predictive Control with Latency 
The latency was migrated by taking it in the actuation model into account as explaine above.


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
* [uWebSockets](https://github.com/uWebSockets/uWebSockets) == 0.14, but the master branch will probably work just fine
  * Follow the instructions in the [uWebSockets README](https://github.com/uWebSockets/uWebSockets/blob/master/README.md) to get setup for your platform. You can download the zip of the appropriate version from the [releases page](https://github.com/uWebSockets/uWebSockets/releases). Here's a link to the [v0.14 zip](https://github.com/uWebSockets/uWebSockets/archive/v0.14.0.zip).
  * If you have MacOS and have [Homebrew](https://brew.sh/) installed you can just run the ./install-mac.sh script to install this.
* [Ipopt](https://projects.coin-or.org/Ipopt)
  * Mac: `brew install ipopt --with-openblas`
  * Linux
    * You will need a version of Ipopt 3.12.1 or higher. The version available through `apt-get` is 3.11.x. If you can get that version to work great but if not there's a script `install_ipopt.sh` that will install Ipopt. You just need to download the source from the Ipopt [releases page](https://www.coin-or.org/download/source/Ipopt/) or the [Github releases](https://github.com/coin-or/Ipopt/releases) page.
    * Then call `install_ipopt.sh` with the source directory as the first argument, ex: `bash install_ipopt.sh Ipopt-3.12.1`. 
  * Windows: TODO. If you can use the Linux subsystem and follow the Linux instructions.
* [CppAD](https://www.coin-or.org/CppAD/)
  * Mac: `brew install cppad`
  * Linux `sudo apt-get install cppad` or equivalent.
  * Windows: TODO. If you can use the Linux subsystem and follow the Linux instructions.
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page). This is already part of the repo so you shouldn't have to worry about it.
* Simulator. You can download these from the [releases tab](https://github.com/udacity/CarND-MPC-Project/releases).



## Basic Build Instructions


1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./mpc`.

