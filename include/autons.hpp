// #pragma once
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/adi.hpp"
#include "pros/rtos.hpp"

inline int team = 2;

inline void turnTo(float angle, int timeout = -1, bool async = false) {
    if (timeout == -1) {
        timeout = std::abs(chassis.getPose().theta - angle) * 4 + 100;
    }
    chassis.turnToHeading(angle, timeout, {}, async);
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



inline void ringRushBlue(bool elim) {
    team = 1;

    // Assuming odom works properly, all you need to do for tuning is just to change this value below
    // Fine tune by changing the code below
    chassis.setPose(30, 16, 18);

    // Get middle ring
    chassis.moveToPoint(46, 70, 2000);
    chassis.waitUntilDone();

    // Doinker activate to get the ring
    doinker.set_value(1);
    pros::delay(100);

    // Move backwards
    chassis.moveToPoint(30, 30, 1000, {.maxSpeed=70});
    chassis.waitUntilDone();

    // Get mogo
    doinker.set_value(0);
    chassis.moveToPoint(24, 48, 1000, {.forwards=false});

    // Score all 3 rings
    chassis.moveToPose(72, 48, 90, 2000);

    // Go to corner
    chassis.moveToPoint(70, 2, 2000);
    chassis.moveToPoint(72, 0, 1000);

    // Get the middle ring in the field
    chassis.moveToPose(0, 24, -90, 2000);

    // Score alliance stake
    turnTo(-180, -1, true);
    chassis.moveToPoint(0, 8, 750);
    chassis.waitUntilDone();
    LBmoveToAngle(170, 70, 5, 1000);
    pros::delay(250);
    resetLB0();
    
    if (elim) {
        // Go to left side of field
        chassis.moveToPoint(-48, 24, 2000);
    } else {
        // Touch ladder
        chassis.moveToPoint(0, 50, 2000);
    }
}


inline void ringRushRed(bool elim) {
    team = 0;

    // Assuming odom works properly, all you need to do for tuning is just to change this value below
    // Fine tune by changing the code below
    chassis.setPose(-30, 16, -30);

    // Get middle ring
    chassis.moveToPoint(-46, 70, 2000, {});
    chassis.waitUntilDone();

    // Doinker activate to get the ring
    doinker.set_value(1);
    pros::delay(100);

    // Move backwards
    chassis.moveToPoint(-30, 30, 1000, {.maxSpeed=70});
    chassis.waitUntilDone();

    // Get mogo
    doinker.set_value(0);
    chassis.moveToPoint(-24, 48, 1000, {.forwards=false});

    // Score all 3 rings
    chassis.moveToPose(-72, 48, 90, 2000);

    // Go to corner
    chassis.moveToPoint(-70, 2, 2000);
    chassis.moveToPoint(-72, 0, 1000);

    // Get the middle ring in the field
    chassis.moveToPose(0, 24, -90, 2000);

    // Score alliance stake
    turnTo(180, -1, true);
    chassis.moveToPoint(0, 8, 750);
    chassis.waitUntilDone();
    LBmoveToAngle(170, 70, 5, 1000);
    //add code to resetLB0
    pros::delay(250);
    
    if (elim) {
        // Go to left side of field
        chassis.moveToPoint(-48, 24, 2000);
    } else {
        // Touch ladder
        chassis.moveToPoint(0, 50, 2000);
    }
}

inline void rightBLUE() {
    team = 1;
    chassis.setPose(0, 0, 90);

    // Get alliance stake
    chassis.turnToHeading(45, 500, {}, false);
    LBmoveToAngle(170, 100, 5, 2000);
    pros::delay(100);

    // Get Mogo
    chassis.moveToPoint(-16, -32, 1800, {.forwards=false, .maxSpeed=100}, false);
    getMogo();    
    pros::Task backLB(resetBackLB);

    // Get 2 rings
    quick_turn_to(-135, 1000);
    intake=100;
    chassis.moveToPose(-54, -56, -90, 4000, {.lead=0.7,.minSpeed=50}, false);
    pros::delay(1000);
    // chassis.moveToPose(-18, -30, -180, 2000, {.forwards=false, .lead=0.7, .minSpeed=70});
    move_forward(-9, 500);
    chassis.swingToPoint(-34, -34, lemlib::DriveSide::RIGHT, 1200, {.minSpeed=100});
    // Get the 1 ring in the middle of the field 
    chassis.moveToPose(6, -20, 90, 3000, {.lead=0.75}, false);
    pros::delay(1000);
    intake = 10;
    chassis.moveToPoint(2, -46, 2000, {.maxSpeed=90});
    intake = 0;
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
