#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>
#include <cassert>


const float kWindowSize = 10;      // samples
const float kSamplingTime = 0.001;  // seconds
unsigned int kServoCycle = 0;

class FakeRobot {
 public:
  FakeRobot() = default;
  ~FakeRobot() = default;

  // Current robot tool position in meters.
  // The following is a simplistic implementation, you will not need to know the
  // specifics in your code.
  float position_x() {
    return 0.01 * cos(2 * M_PI * kSamplingTime * kServoCycle);
  }
  float position_y() {
    return 0.01 * sin(2 * M_PI * kSamplingTime * kServoCycle);
  }
  float position_z() {
    return 0.01 * cos(1 * M_PI * kSamplingTime * kServoCycle);
  }

  // Current force applied by the robot tool on the environment in newtons
  // The following is a simplistic implementation, you will not need to know the
  // specifics in your code.
  float force_x() { return -10 * cos(2 * M_PI * kSamplingTime * kServoCycle); }
  float force_y() { return -10 * sin(2 * M_PI * kSamplingTime * kServoCycle); }
  float force_z() {
    return std::min(50 * kSamplingTime * kServoCycle *
                        sin(2 * M_PI * kSamplingTime * kServoCycle),
                    (double)1.0);
  }
};

class RobotMonitor {
 public:
  RobotMonitor() {
    // add any initialization here
    Average = 0;
    px_prev = 0;
    py_prev = 0;
    pz_prev = 0;    
  }
  ~RobotMonitor() {}
  
    std::vector< float > Power; // Power vector
    float Average; // Moving Average result
    float px_prev; // Record previous position for velocity computation
    float py_prev; //
    float pz_prev; //
    
  float Update(const float& px, const float& py, const float& pz,
               const float& fx, const float& fy, const float& fz) {
                    
                    if (Power.size()==0) { //First cycle
                        float P = 0;
                        Power.push_back(P);
                        return 0;
                    }
                    else{ // Compute Power
                        assert(kSamplingTime!=0);
                        float P = ((px-px_prev)*fx)+((py-py_prev)*fy)+((pz-pz_prev)*fz)/kSamplingTime;
                        Power.push_back(P);
                  
                        if (Power.size() < kWindowSize){ // Average when data size is smaller than kWindowSize
                            assert(Power.size()!=0);
                            Average = (Average*(Power.size()-1)+P)/Power.size(); 
                        }
                        else{ // Computer Moving Average with window size kWindowSize
                            assert(kWindowSize!=0);
                            Average = Average+(P-Power[Power.size()-kWindowSize-1])/kWindowSize; 
                        }
                  
                    px_prev = px;
                    py_prev = py;
                    pz_prev = pz;  
                    std::cout<<"Cycle:"<<kServoCycle<<","<<(P)<<","<<Average<<","<<std::abs(Average)<<std::endl;
                    return std::abs(Average); // return the absolute value result
                      
                    }
    // implement your power monitoring function here and return the mechanical
    // power computed (with a 10 sample moving average filter)
  }
  // add any member variables you need
};

int main() {
  FakeRobot robot;
  RobotMonitor monitor;

  // run the robot for 100 seconds, with a sampling rate of 1000 Hz
  for (kServoCycle = 0; kServoCycle < 100 / kSamplingTime; ++kServoCycle) {
    float mechanical_power = monitor.Update(
        robot.position_x(), robot.position_y(), robot.position_z(),
        robot.force_x(), robot.force_y(), robot.force_z());
    
    // std::cout<<mechanical_power<<std::endl;
    // the threshold used is 10 watts
    if (mechanical_power > 10) {
      std::cout << "Too much massage! Stopped on servo cycle: " << kServoCycle
                << std::endl;
      break;
    }
  }
  
  return 0;
}
