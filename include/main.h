/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convenient for most student programmers.
 */
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"
#define PROS_USE_SIMPLE_NAMES

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
#define PROS_USE_LITERALS

#include "api.h"
#include "robodash/api.h"
#include "lemlib/api.hpp"

/**
 * You should add more #includes here
 */
//#include "okapi/api.hpp"

/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
// using namespace pros;
// using namespace pros::literals;
// using namespace okapi;

/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */
#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
inline int intake = 0;
inline int rollerIntake = 0;



inline pros::MotorGroup left_motors({-16, -15, -14}, pros::MotorGearset::blue);
inline pros::MotorGroup right_motors({17, 19, 18}, pros::MotorGearset::blue);

// inline pros::Motor chain(-12, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
// inline pros::Motor flexWheels(5, pros::MotorGearset::green);

inline pros::Motor chain(-12);
inline pros::Motor rollers(-4);


inline pros::Imu imu(8);


inline pros::Motor armMotor(5, pros::MotorGearset::rpm_600, pros::MotorEncoderUnits::degrees);


inline pros::adi::DigitalOut mogo('E');
inline pros::adi::DigitalOut doinker('G');
inline pros::adi::DigitalOut hangADI('H');




inline pros::Rotation horizontalTrackingWheel(-10);
inline pros::Rotation LBtracking(-2);

inline pros::Rotation verticalTrackingWheel(20);


inline pros::Optical ringsort(13);




inline pros::Controller controller(pros::E_CONTROLLER_MASTER);











 // Controller LCD screen:




/*
inline Line 1: Bat: X% |
inline Line 2: Errors: (e.g. dc IMU)
inline Line 3: Log
*/




inline lemlib::Drivetrain drivetrain(
   &left_motors,
   &right_motors,
   11.8,
   lemlib::Omniwheel::NEW_275,
   400,
   8
);


inline lemlib::ControllerSettings lateral_controller(7, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                                                      3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);


 // angular PID controller
inline lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                             0.05, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             4.420714285714281, // anti windup
                                             1, // small error range, in inches
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in inches
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);


 // lemlib::TrackingWheel vertical_tracking_wheel(&vs, lemlib::Omniwheel::NEW_275, -6);


 inline lemlib::TrackingWheel horizontalTracking(&horizontalTrackingWheel, lemlib::Omniwheel::NEW_2, -1.6);
//  inline lemlib::TrackingWheel verticalTracking(&verticalTrackingWheel, lemlib::Omniwheel::NEW_2, 0.8);

 // to not break, disable vertical tracking, set rpm to 400, horizontal tracking wheel to -1.6
 
 inline lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                             nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                             &horizontalTracking, // horizontal tracking wheel 1
                             nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                             &imu//&imu // inertial sensor
 );
 







// input curve for throttle input during driver control
inline lemlib::ExpoDriveCurve throttle_curve(5, // joystick deadband out of 127
                                    10, // minimum output where drivetrain will move out of 127
                                    1.021 // expo curve gain
);








// input curve for steer input during driver control
inline lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                 0, // minimum output where drivetrain will move out of 127
                                 1.021 // expo curve gain
);








// create the  uwu Jacob Kim is gay
inline lemlib::Chassis chassis(drivetrain, // drivetrain settings
                       lateral_controller, // lateral PID settings
                       angular_controller, // angular PID settings
                       sensors,
                       &throttle_curve,
                       &steer_curve
);


// Controller LCD screen:




/*
Line 1: Bat: X% |
Line 2: Errors: (e.g. dc IMU)
Line 3: Log
*/




extern lemlib::Drivetrain drivetrain;


extern lemlib::ControllerSettings lateral_controller;


// angular PID controller
extern lemlib::ControllerSettings angular_controller;

// lemlib::TrackingWheel vertical_tracking_wheel(&vs, lemlib::Omniwheel::NEW_275, -6);


extern lemlib::TrackingWheel horizontalTracking;

extern lemlib::OdomSensors sensors;







// input curve for throttle input during driver control
extern lemlib::ExpoDriveCurve throttle_curve;








// input curve for steer input during driver control
extern lemlib::ExpoDriveCurve steer_curve;








// create the  uwu Jacob Kim is gay
extern lemlib::Chassis chassis;






inline bool movingArmMotor = false;


inline int sign(int num) {
  if (num == 0)
    return 0;
  return (num >= 0) ? 1 : -1;
}

inline float lateralInputRemap(float input, float scale) {
  if (scale != 0) {
    auto eq1 = [](float x, float a) {
      return powf(a, std::abs(x) - 127) * (std::abs(x)) * sign(x);
    };

    return (eq1(input, scale) * 127.0 / (eq1(127, scale)));
  }
  return input;
}

inline float angularInputRemap(float input, float scale) {
  if (scale != 0) {
    return (powf(2.718, -(scale / 10)) + powf(2.718, (fabs(input) - 127) / 10) *
                                             (1 - powf(2.718, -(scale / 10)))) *
           input;
  }
  return input;
}

inline void arcade(int throttle, int angular) {
  int deadzone = 5;

  throttle = (abs(throttle) <= deadzone) ? 0 : throttle;
  angular = (abs(angular) <= deadzone) ? 0 : angular;

  throttle = lateralInputRemap(throttle, 1.021);

  angular = angularInputRemap(angular, 7.5) * 0.6;

  if (throttle + angular < 100) {
    angular /= 0.6;
  }

  left_motors.move(throttle + angular);
  right_motors.move(throttle - angular);
}


/**
 * Moves the arm motor to an angle
 * Speed is a percentage value. 100 is 127 in voltage
 * Precision is a delay to save resources. Lower is more accurate and higher is
 * more efficient
 */
inline bool LBmoveToAngle(double angle, int speed = 100, int precision = 5, int timeout = 2000) {
    movingArmMotor = true;
   // If the rotational sensor is connected, then use the rotational sensor to move the ladybrown
   // This is done by constantly checking the angle of the rotatoional sensor until it meets the angle specified
   // However, if the rotational sensor is not connected, for example, it disconnects, it should use the internal motor encoding units instead


   int actualtimeout = timeout / precision;


   if (LBtracking.is_installed()) {
       // If move more
       if (angle * 100 > LBtracking.get_position()) {
           armMotor.move(speed * 1.27);
           while (LBtracking.get_position() < angle * 100 && actualtimeout >= 0) {
               pros::delay(precision);
               actualtimeout -= 1;
           }
       } else {
           armMotor.move(-(speed * 1.27));
           while (LBtracking.get_position() > angle * 100 && actualtimeout >= 0) {
               pros::delay(precision);
               actualtimeout -= 1;
           }
       }
       armMotor.move(0);
       armMotor.brake();
   } else {
       // This function should only run if the rotational sensor is not installed
    //    pros::lcd::print(6, "LB tracking gone, using IME");
       armMotor.move_absolute(angle * 5, speed * 10);
       pros::delay(30000 / speed);
   }

   movingArmMotor = false;
  
   return true;
}

inline void move_forward(float inches, int timeout, bool async = false, lemlib::MoveToPointParams params = {}) {
    bool forwards = true;

    if (inches < 0) {
        forwards = false;
    } 

    params.forwards = forwards;


    double angle = -(chassis.getPose(true).theta) + M_PI_2;

    float x = cos(angle) * inches + chassis.getPose().x;
    float y = sin(angle) * inches + chassis.getPose().y;
    
    chassis.moveToPoint(x, y, timeout, params, async);
}


inline void move_to_relative_point(float x, float y, int timeout, bool async = false, lemlib::MoveToPointParams params = {}){
    chassis.moveToPoint(chassis.getPose().x + x, chassis.getPose().y + y, timeout, params, async);
}


/* 
Turns to a specified angle and stops when it is around that range.
The normal chassis.turnToHeading requires full timeout in order to move onto the next move, since our PID is bad.
*/
inline void quick_turn_to(float theta, int maxTimeout = 2000, float errorRange = 1.5, lemlib::TurnToHeadingParams params = {}) {
    chassis.turnToHeading(theta, maxTimeout, params, true);

    int maxWhileLoops = maxTimeout / 5;    

    while (maxWhileLoops > 0) {
        float currentAngle = chassis.getPose().theta;
        float error = std::abs(theta - currentAngle);
        
        // If the current angle is within the error range then stop
        if (error <= errorRange) {
            chassis.cancelMotion();
            break;
        }
        maxWhileLoops--;
        pros::delay(5);
    }
}

/* 
Just like the quick_turn_to function, this will attempt to move the robot to x inches and stop when it reaches the error range.
The purpose is to provide quicker movements at the cost of accuracy.
E.g. you can have 2 "chained" move forward functions like:

void autonomous() {
    quick_move_forward(5);
    quick_turn_to(90);
    quick_move_forward(5);
}
*/
inline void quick_move_forward(float inches, int maxTimeout = 5000, float errorRange = 0.5, lemlib::MoveToPointParams params = {}) {
    bool forwards = true;

    if (inches < 0) {
        forwards = false;
    } 

    params.forwards = forwards;

    double angle = -(chassis.getPose(true).theta) + M_PI_2;

    float x = cos(angle) * inches + chassis.getPose().x;
    float y = sin(angle) * inches + chassis.getPose().y;
    
    chassis.moveToPoint(x, y, maxTimeout, params, {});

    int maxWhileLoops = maxTimeout / 5;    

    while (maxWhileLoops > 0) {
        float currentX = chassis.getPose().x;
        float currentY = chassis.getPose().y;

        double xDiff = std::abs(x - currentX);
        double yDiff = std::abs(y - currentY);

        float error = std::sqrt(xDiff * xDiff + yDiff * yDiff);
        
        // If the current distance is within the error range then stop
        if (error <= errorRange) {
            chassis.cancelMotion();
            break; // Once the while loop is broken, the function completes
        }
        maxWhileLoops--;
        pros::delay(5);
    }

}
inline bool imudc = false;
inline bool nomoveflex = false;
inline bool nomovearm = false;
inline bool onmatch = false;
inline bool ejecting = false;
inline int ejectcounter = 0;
inline int conveyTurnAmt = 0;

inline int armMotorCounter = 0;
inline int armMotorCounterDouble = 0;
inline int alliancecounter = 0;
inline bool moreLB = false;

// Arm motor
inline void armStagesOneRing() {
  if (armMotorCounter == 0) {
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    LBmoveToAngle(17.2, 30, 1);

  } else if (armMotorCounter == 1) {
    // Complicated steps to push it down for the next step
    intake = -20;
    LBmoveToAngle(155 + moreLB * 60, 100, 5, 1000);
    intake = 0;
    moreLB = false;

  } else if (armMotorCounter == 2) {
    LBmoveToAngle(-2, 100, 2, 750);
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // resetLBPos();
    armMotorCounter = -1;
  }

  armMotorCounter++;
  nomovearm = false;
}

inline void armStagesTwoRing() {
  if (armMotorCounterDouble == 0) {
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    LBmoveToAngle(24, 30, 2);
  } else if (armMotorCounterDouble == 1) {
    // intake = 100;
    // pros::delay(20);
    // intake = 0;
    // pros::delay(10);
    intake = -30;
    pros::delay(20);
    intake = 0;
    LBmoveToAngle(70, 40, 2);
  } else if (armMotorCounterDouble == 2) {
    // Complicated steps to push it down for the next step
    LBmoveToAngle(160, 100, 5);
  } else if (armMotorCounterDouble == 3) {
    LBmoveToAngle(40, 50, 2);
  } else if (armMotorCounterDouble == 4) {
    LBmoveToAngle(160, 100, 5);
  } else if (armMotorCounterDouble == 5) {
    LBmoveToAngle(0, 100, 2, 200);
    armMotorCounterDouble = -1;
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // resetLBPos();
  }

  armMotorCounterDouble++;
  nomovearm = false;
}

inline void allianceStakeCode() {
  // Wall stake mech code
  // Should be ran inside a thread because it uses delay commands which can
  // interrupt the main while true loop
  move_forward(-5.8, 500, true, {.minSpeed = 30});
  moreLB = true;
}

/* Function to "double tap" the intake to make the ring in the ladybrown
 * actually get "in there" so it can stay there and won't fall off*/
inline void intakeLB(int amt = 2) {
  for (int i = 0; i < amt; i++) {
    intake = 100;
    pros::delay(150);
    intake = 0;
    pros::delay(150);
  }
}

inline void lbLater() {
  pros::delay(750);
  armStagesOneRing();
}

inline void intakeLBT() { intakeLB(); }


// homemade PID???

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
#endif
#endif