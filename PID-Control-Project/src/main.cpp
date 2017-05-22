#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>
#define TWIDDLE 0

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}


int main() {
	// Sometimes the manual mode is engaged because of s being empty
	// The debugCoundter filters such messages
	int debugCounter = 0;
  	  uWS::Hub h;
  	  PID speedPid = PID(.125 , .0001 , 0.797906);
  	  PID steerPid = PID(0.071769, 0.00411344, 0.974954);

  	  h.onMessage([&steerPid, &speedPid, &debugCounter](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data));
      if (s != "") {
    	  debugCounter = 0;
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          steerPid.UpdateError(cte);
          double steerAngle = steerPid.getValue();
          double targetSpeed = 30.*(1.-abs(steerAngle)) + 20.;
          speedPid.UpdateError(speed - targetSpeed);
          //std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steerAngle;
          msgJson["throttle"] = speedPid.getValue();
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          if(TWIDDLE == 0 && (steerPid.getNumberOfSteps() > 1000)) steerPid.twiddle();
          if(TWIDDLE == 1 && (speedPid.getNumberOfSteps() > 1000)) speedPid.twiddle();
          //std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
    	  // Manual driving
    	  debugCounter++;
    	  if(TWIDDLE <2  && (debugCounter > 2)) {
    		  std::string msg = "42[\"manual\",{}]";
    		  if(TWIDDLE == 0 && steerPid.getNumberOfSteps() > 100){
    			  steerPid.twiddle();
    			  steerPid.setNumberOfSteps(0);
    		  }
    		  else if(TWIDDLE == 1 && speedPid.getNumberOfSteps() > 100) {
    			  speedPid.twiddle();
    			  speedPid.setNumberOfSteps(0);
    		  }
    		  ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
    	  } else {
    		  std::string msg = "42[\"manual\",{}]";
    		  //std::cout << "Ignored : " << msg << std::endl;
    		  ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
    	  }
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
