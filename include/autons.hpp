// #pragma once
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"

inline int team = 1;
inline bool elim = false;

inline void turnTo(float angle, int timeout = -1, bool async = false) {
    if (timeout == -1) {
        timeout = std::abs(chassis.getPose().theta - angle) * 4 + 100;
    }
    chassis.turnToHeading(angle, timeout, {}, async);
}
inline void setLB0() {
    LBmoveToAngle(0, 80);
}

inline void resetLBPos() {
    LBtracking.reset();
    LBtracking.reset_position();
    armMotor.tare_position();
}

inline void resetLB() {
    pros::Task rsttask(resetLBPos);
}

inline void resetBackLB(){
    LBmoveToAngle(-30, 100, 5, 3000);
    resetLBPos();
}


inline void turn(float degrees, int timeout, bool async = false) {
   float angle = chassis.getPose().theta + degrees;


   chassis.turnToHeading(angle, timeout, {}, async);
}



inline void getMogo() {
    mogo.set_value(1);
    pros::delay(200);
}

        
inline void leftBLUE() {
    team = 1;
    // Get the mogo
    doinker.set_value(1);
    chassis.moveToPoint(1.5, 40, 2000, {}, false);
    move_forward(-24, 2000);
    doinker.set_value(0);

    // Get the mogo x2
    chassis.moveToPose(34, 25, -135, 2000, {.forwards=false}, false);
    getMogo();
    intake = 100;
    
    // Prepare for the grabbed mogo
    pros::delay(500);
    // chassis.swingToPoint(10, 30, lemlib::DriveSide::RIGHT, 1000);
    quick_turn_to(-55, 750);
    move_forward(15, 1000);
    pros::delay(500);
    mogo.set_value(0);
    move_forward(-16, 1000);
    quick_turn_to(-135, 1000);
    chassis.moveToPoint(4, 32, 2000, {.forwards=false,.maxSpeed=100}, false);
    getMogo();

    // Get the ring
    chassis.moveToPose(16.5, 33, 30, 1000);

    chassis.moveToPose(4, 0, 0, 3000, {.forwards=false, .minSpeed=60}, false);    
    mogo.set_value(0);
    move_forward(4, 500);
    quick_turn_to(-130);

    // Go clear the corner lol
    // doinker.set_value(1);
    // chassis.moveToPose(-4.5, -6, -180, 3000);
    // chassis.turnToHeading(-245, 1000, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE}, false);
    // doinker.set_value(0);
}

inline void putDownLB() {

  pros::Task putdownlb([&]() {
    armMotor.move(-127);
    pros::delay(500);

    armMotor.tare_position();
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    armMotor.brake();
  });
  
}

inline void stop(){
    pros::delay(1000);
    intake = 0;
    pros::delay(10000000);
}

inline void stopWhenRing() {
    while (ringsort.get_proximity() >= 210 ) {
        pros::delay(1);
    }
    intake = 5;
}

inline void ringRushBlue() {
    team = 1;
    
    putDownLB();

    // Assuming odom works properly, all you need to do for tuning is just to change this value below
    // Fine tune by changing the code below
    chassis.setPose(32, 16, 21);
    
    // Doinker activate to get the ring
    doinker.set_value(1);

    // Get middle ring
    intake = 100;
    chassis.moveToPoint(47.5, 58, 2000);
    chassis.waitUntilDone();
    intake = 75;
    pros::delay(500);
    pros::Task bru(stopWhenRing);

    // Move backwards
    chassis.moveToPoint(42, 30, 1000, {.forwards=false, .maxSpeed=75});
    // move_forward(-25, 1000, true, {.maxSpeed=70});
    chassis.waitUntilDone();

    intake = 0;
    // Get mogo
    doinker.set_value(0);
    pros::delay(200);
    chassis.turnToHeading(140, 650);
    chassis.moveToPoint(24, 55, 1000, {.forwards=false, .maxSpeed=60});
    chassis.waitUntilDone();    
    mogo.set_value(1);
    pros::delay(500);

    // Score all 3 rings
    intake = 100;
    chassis.moveToPose(52, 55, 80, 2500, {.lead=0.5, .minSpeed=80});

    // pros::delay(100000);

    // Go to corner
    chassis.moveToPoint(70, 2, 2000, {.maxSpeed=70});
    chassis.waitUntilDone();
    move_forward(-12, 500);
    chassis.moveToPoint(72, 0, 1000, {.maxSpeed=70});

    // Get the middle ring in the field
    chassis.turnToHeading(-75, 500);
    chassis.moveToPose(16, 12, -90, 2000, {.minSpeed=80});
    armStagesOneRing();
    armMotorCounter = 0;

    // Score alliance stake
    turnTo(-180, -1);
    move_forward(8, 750);

    LBmoveToAngle(170, 60, 5, 1500);
    pros::delay(250);
    intake = 0;
    pros::Task rstlbfunc(setLB0);
    if (elim) {
        // Go to left side of field
        chassis.moveToPoint(-48, 24, 1000, {.minSpeed=80});
    } else {
        // Touch ladder
        chassis.moveToPoint(14, 50,1000, {.minSpeed=80});
    }
}

inline void mogoRushBlue() {
    team = 1;

    // Main set pose. Tune this, then fine tune the other code
    chassis.setPose(-60, 18, 0);

    // Rush for the mogo
    chassis.moveToPoint(-50, 70, 2000, {.minSpeed=80});
    chassis.waitUntilDone();

    // Move it back
    doinker.set_value(1);
    pros::delay(150);
    chassis.moveToPoint(-50, 24, 2000, {.forwards=false});
    chassis.waitUntilDone();

    // Get mogo
    doinker.set_value(0);
    chassis.moveToPoint(-24, 48, 1000, {.forwards=false});
    chassis.waitUntilDone();
    mogo.set_value(1);
    pros::delay(200);

    // Score ring. Due to the rush, ring needs to be tuned
    intake = 100;
    chassis.moveToPoint(-48, 48, 1000);
    chassis.swingToPoint(-50, 30, DriveSide::LEFT, 1000);

    // Get the other mogo
    mogo.set_value(0);
    chassis.moveToPoint(-50, 24, 1000, {.forwards=false});
    chassis.waitUntilDone();
    mogo.set_value(1);

    // Go to corner
    chassis.moveToPoint(-70, 2, 2000);
    chassis.waitUntilDone();
    move_forward(-12, 500);
    chassis.moveToPoint(-72, 0, 1000);

    // Get the middle ring in the field
    chassis.moveToPose(0, 24, -90, 2000);
    if (!elim) {
        // Drop mogo
        chassis.turnToHeading(45, 750);
        chassis.waitUntilDone();
        mogo.set_value(0);

        // Touch ladder
        chassis.moveToPoint(0, 50, 2000, {.minSpeed=70});
    }
}


inline void slowRightBlue() {
    team = 1;

    // Tune this first
    chassis.setPose(0, 4, -90);

    // Get alliance stake
    chassis.turnToHeading(-138, 500);
    chassis.waitUntilDone();
    LBmoveToAngle(170, 70, 5, 2000);
    pros::delay(100);
    pros::Task rstlbfunc(setLB0);

    // Get mogo
    chassis.moveToPose(24, 48, -180, 2000, {.forwards=false, .lead=0.7});
    chassis.waitUntilDone();
    mogo.set_value(1);
    pros::delay(100);

    // Get 2 rings in the middle. WARNING: RISKY
    chassis.turnToHeading(45, 750);
    chassis.moveToPose(72, 64, 90, 2500);
    chassis.waitUntilDone();
    pros::delay(500);

    // Get middle ring
    chassis.swingToPoint(48, 48, DriveSide::RIGHT, 1500);


    // Go to corner
    chassis.moveToPoint(-70, 2, 2000);
    chassis.waitUntilDone();
    move_forward(-12, 500);
    chassis.moveToPoint(-72, 0, 1000);

    // Get the middle ring in the field
    chassis.moveToPose(0, 24, -90, 2000);

    if (elim) {
        // Go to left side of field
        chassis.moveToPoint(-48, 24, 2000, {.minSpeed=80});
    } else {
        // Touch ladder
        chassis.moveToPoint(0, 50, 2000, {.minSpeed=80});
    }
}

inline void leftRED() {
    team = 0;
    chassis.setPose(4, -16.75, -90);

    // Get alliance stake
    chassis.turnToHeading(-45, 500, {}, false);
    LBmoveToAngle(170, 100, 5, 2000);
    pros::delay(100);

    // Get Mogo
    chassis.moveToPoint(13, -42, 2000, {.forwards=false, .maxSpeed=100}, false);
    pros::delay(590);
    getMogo();    pros::Task backLB(resetBackLB);


    // Get 2 rings
    quick_turn_to(135, 1000);
    intake=100;
    chassis.moveToPose(54, -56, 90, 3000, {.lead=0.7,.minSpeed=50}, false);
    pros::delay(2000);

    // Get the 1 ring in the middle of the field 
    chassis.moveToPose(15, -45, 90, 2000, {.forwards=false, .lead=0.7, .minSpeed=70});
    chassis.moveToPoint(36, -28, 2000);
    pros::delay(500);
    chassis.moveToPose(-12, -18, -90, 3000, {.maxSpeed=80});
    chassis.moveToPoint(0, -46, 3000, {.maxSpeed=70});
}


inline void rightRed(){

    team = 0;

    // Get the mogo
    doinker.set_value(1);
    chassis.moveToPoint(3, 40, 2000, {}, false);
    move_forward(-26, 2000);
    doinker.set_value(0);

    // Get the mogo x2
    chassis.moveToPose(-18, 35, 135, 2000, {.forwards=false}, false);
    getMogo();
    intake = 100;
    
    // Prepare for the grabbed mogo
    pros::delay(500);
    chassis.moveToPose(-12, 28, 0, 2000, {}, false);    
    mogo.set_value(0);
    chassis.moveToPose(4, 36, -160, 3000, {.forwards=false}, false);
    getMogo();
    move_forward(4, 500);
    chassis.swingToPoint(7, 37, lemlib::DriveSide::LEFT, 2000);

    // chassis.turnToHeading(0, 1000, {}, false);
    mogo.set_value(0);

    chassis.moveToPose(-2, 36, -80, 2000);
    // move_forward(-8, 2000, false, {.maxSpeed=80});

}

inline void soloAWPRed(){
    team = 0;

    LBmoveToAngle(180, 100, 5, 2000);

    chassis.moveToPoint(-20, -28, 1500, {.forwards=false, .maxSpeed=110}, false);

    LBmoveToAngle(-50, 100, 5, 300);

    getMogo();

    chassis.turnToHeading(-175, 750, {}, false);

    intake = 100;

    move_forward(20, 1000, false);

    move_forward(-20, 700, false);

    chassis.moveToPoint(0, -40, 1500, {}, false);

    chassis.turnToHeading(10, 1000, {}, false);

    move_forward(30, 2500, false);
    
    mogo.set_value(0);

    chassis.turnToHeading(-45, 500, {}, false);

    move_forward(40, 2000, false, {.maxSpeed=80});

    pros::delay(500);

    intake = 0;

    chassis.turnToHeading(50, 500, {}, false);

    move_forward(-20, 1000, false, {.forwards=false, .maxSpeed=100});

    getMogo();

    // chassis.turnToHeading(-40, 700, {}, false);

    // move_forward(24, 1000, false);

    // move_forward(-50, 10000, false);
}

inline void stopIn() {
    pros::delay(3000);
    intake = 0;
}

inline void soloAWPBlue() {
    team = 1;

    chassis.setPose(0, 0, 90);

    // Get alliance stake
    chassis.turnToHeading(45, 400, {}, false);
    LBmoveToAngle(170, 100, 5, 1000);
    pros::delay(50);

    // Get Mogo    
    pros::Task backLB(resetBackLB);
    chassis.moveToPoint(-16, -32, 1400, {.forwards=false, .maxSpeed=100}, false);
    getMogo();    
    
    // Intake 1 ring
    turnTo(-90);
    intake = 100;
    move_forward(30,1000);

    // Intake 1 ring from corner
    turnTo(-30);
    move_forward(50, 2000);
    move_forward(10, 500);

    // Get the ring in the middle
    chassis.moveToPoint(10, -4, 2000, {}, false);

    // Get mogo
    mogo.set_value(0);
    pros::delay(500);
    intake = 0;
    turnTo(-45);
    move_forward(-32, 750);
    mogo.set_value(1);

    // Get ring
    turnTo(90);
    intake = 100;
    move_forward(30, 1000);
    move_forward(-54, 2000);


}
