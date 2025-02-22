// #pragma once
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"

inline int team = 2;



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





// POV: When realtive move makes your auton code much faster than before for no reason
// and somehow makes it more consistent wtf??
inline void saferightBLUE() {
    team = 1;
    chassis.setPose(0, 0, 90);

    // Get alliance stake
    chassis.turnToHeading(45, 500, {}, false);
    LBmoveToAngle(170, 100, 5, 2000);
    pros::delay(100);

    // Get Mogo
    chassis.moveToPoint(-15, -32, 2000, {.forwards=false, .maxSpeed=100}, false);
    pros::delay(200);
    getMogo();    pros::Task backLB(resetBackLB);

    // Get 2 rings
    quick_turn_to(-135, 1000);
    intake=100;
    chassis.moveToPose(-54, -57.5, -90, 3000, {.lead=0.7,.minSpeed=50}, false);
    pros::delay(2000);

    // Get the 1 ring in the middle of the field 
    chassis.moveToPose(-15, -32, 0, 2000, {.forwards=false, .lead=0.7, .minSpeed=70});
    chassis.moveToPoint(-38, -27, 2000);
    pros::delay(500);
    chassis.moveToPoint(4, -40, 10000, {.maxSpeed=70});
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
    chassis.moveToPose(-54, -57, -90, 4000, {.lead=0.7,.minSpeed=50}, false);
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
    chassis.moveToPose(54, -57, 90, 3000, {.lead=0.7,.minSpeed=50}, false);
    pros::delay(2000);

    // Get the 1 ring in the middle of the field 
    chassis.moveToPose(15, -45, 90, 2000, {.forwards=false, .lead=0.7, .minSpeed=70});
    chassis.moveToPoint(36, -28, 2000);
    pros::delay(500);
    chassis.moveToPose(-12, -18, -90, 3000, {.maxSpeed=80});
    chassis.moveToPoint(0, -46, 3000, {.maxSpeed=70});
}

// inline void leftRed() {
//     chassis.moveToPoint(0, 2.6, 200);
    
//     chassis.turnToHeading(45, 400);

//     // Raise LB
//     LBmoveToAngle(110, 50, 2, 700);
//     LBmoveToAngle(210, 100, 2, 300);

//     chassis.moveToPoint(-16, 0, 800, {.forwards=false}, false);
//     LBmoveToAngle(-100, 100, 5, 500);

//     chassis.turnToHeading(0, 500, {}, false);

//     doinker.set_value(1);
//     pros::delay(100);
//     move_forward(-10, 700, false);
//     pros::delay(100);

//     doinker.set_value(0);
//     resetLB();


//     chassis.moveToPoint(-35.6, -10.3, 1000, {.forwards=false}, false);
//     pros::delay(250);
//     getMogo();

//     chassis.turnToHeading(180, 700, {}, false);
//     intake=100;
//     move_forward(20, 700, false);

//     chassis.turnToHeading(-80, 700, {}, false);

//     move_forward(15, 700, false);

//     pros::delay(300);

//     move_forward(-10, 700, false);

//     chassis.turnToHeading(-110, 700, {}, false);

//     move_forward(10.5, 700, false);

//     pros::delay(300);

//     move_forward(-35, 1500, false);

//     chassis.turnToHeading(0, 700, {}, false);

//     move_forward(23, 700, false);
    
//     pros::delay(100);

//     chassis.turnToHeading(-70, 700, {}, false);

//     move_forward(40, 2000, false, {.maxSpeed=70});
// }


inline void rightRed(){

    team = 0;

    // Get the mogo
    doinker.set_value(1);
    chassis.moveToPoint(1.5, 40, 2000, {}, false);
    move_forward(-26, 2000);
    doinker.set_value(0);

    // Get the mogo x2
    chassis.moveToPose(-18, 35, 135, 2000, {.forwards=false}, false);
    getMogo();
    intake = 100;
    
    // Prepare for the grabbed mogo
    pros::delay(500);
    chassis.moveToPose(-2, 8, 0, 2000, {}, false);    
    mogo.set_value(0);
    chassis.moveToPose(4, 36, -160, 3000, {.forwards=false}, false);
    getMogo();
    move_forward(4, 500);
    chassis.swingToPoint(6, 41, lemlib::DriveSide::LEFT, 2000);

    chassis.turnToHeading(0, 1000, {}, false);
    move_forward(-18, 2000, false, {.maxSpeed=80});

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
    LBmoveToAngle(170, 100, 5, 2000);
    pros::delay(50);

    // Get Mogo
    chassis.moveToPoint(-16, -32, 1600, {.forwards=false, .maxSpeed=100}, false);
    getMogo();    
    pros::Task backLB(resetBackLB);

    // Get 2 rings
    quick_turn_to(-135, 1000);
    intake=100;
    chassis.moveToPose(-55, -57, -90, 3000, {.lead=0.7,.minSpeed=60}, false);
    pros::delay(300);
    // chassis.moveToPose(-18, -30, -180, 2000, {.forwards=false, .lead=0.7, .minSpeed=70});
    move_forward(-8, 400);
    chassis.swingToPoint(-34, -34, lemlib::DriveSide::RIGHT, 1200, {.minSpeed=100});
    // Get the 1 ring in the middle of the field 
    pros::Task bru(stopIn);
    chassis.moveToPose(11, -20, 90, 3000, {.lead=0.75, .minSpeed=100}, false);
    pros::delay(500);
    // 
    // chassis.turnToHeading(0, 500);
    mogo.set_value(0);
    intake = 100;
    move_forward(-8, 500);    
    intake = 0;
    quick_turn_to(90, 1000);

    chassis.moveToPose(9, -44, 0, 2000, {.forwards=false, .lead=0.8, .minSpeed=70}, false);
    getMogo();
    intake = 100;
    chassis.turnToHeading(90, 500, {}, false);
    move_forward(24, 1000);
    quick_turn_to(-90, 1000);
    move_forward(40, 1000);
    chassis.cancelMotion();
}
