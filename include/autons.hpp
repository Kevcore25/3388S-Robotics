// #pragma once
#include "main.h"

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
    // Get the mogo
    chassis.moveToPoint(3, 37, 1300, {.minSpeed=10}, false);
    doinker.set_value(1);
    pros::delay(200);
    chassis.moveToPoint(0, 25, 2000, {.forwards=false}, false);
    doinker.set_value(0);

    // Grab the mogo

    chassis.turnToHeading(-90, 1000, {}, false);
    move_forward(-28, 1000, false);
    pros::delay(250);
    getMogo();

    intake = 100;

    pros::delay(1000);

    mogo.set_value(0);

    move_forward(28, 1000, false);

    chassis.turnToHeading(-150, 1000, {}, false);

    move_forward(-12, 700, false);

    pros::delay(250);
    getMogo();

    chassis.turnToHeading(75, 600, {}, false);

    move_forward(15, 700, false);

    pros::delay(300);

    chassis.turnToHeading(30, 1000, {}, false);

    move_forward(-35, 700, false);

    mogo.set_value(0);

    move_forward(3, 700, false);

    chassis.turnToHeading(-90, 1000, {}, false);

    move_forward(-24, 1000, false);

    pros::delay(300);
    getMogo();


}

// POV: When realtive move makes your auton code much faster than before for no reason
// and somehow makes it more consistent wtf??
inline void rightBLUE() {
    LBmoveToAngle(170, 100, 5, 2000);

    chassis.moveToPoint(20, -28, 1500, {.forwards=false, .maxSpeed=100}, false);

    pros::Task backLB(resetBackLB);

    getMogo();

    chassis.turnToHeading(170, 1000, {}, false);

    intake=100;

    move_forward(26, 1000);

    move_forward(-14, 1000);

    chassis.turnToHeading(183, 1000, {}, false);

    move_forward(26, 1000);

    move_forward(-28, 1000);

    chassis.turnToHeading(200, 500, {}, false);

    move_forward(17, 700);

    move_forward(-30, 1500);


}


inline void leftRed() {
    chassis.moveToPoint(0, 2.6, 200);
    
    chassis.turnToHeading(45, 400);

    // Raise LB
    LBmoveToAngle(110, 50, 2, 700);
    LBmoveToAngle(210, 100, 2, 300);

    chassis.moveToPoint(-16, 0, 800, {.forwards=false}, false);
    LBmoveToAngle(-100, 100, 5, 500);

    chassis.turnToHeading(0, 500, {}, false);

    doinker.set_value(1);
    pros::delay(100);
    move_forward(-10, 700, false);
    pros::delay(100);

    doinker.set_value(0);
    resetLB();


    chassis.moveToPoint(-35.6, -10.3, 1000, {.forwards=false}, false);
    pros::delay(250);
    getMogo();

    chassis.turnToHeading(180, 700, {}, false);
    intake=100;
    move_forward(20, 700, false);

    chassis.turnToHeading(-80, 700, {}, false);

    move_forward(15, 700, false);

    pros::delay(300);

    move_forward(-10, 700, false);

    chassis.turnToHeading(-110, 700, {}, false);

    move_forward(10.5, 700, false);

    pros::delay(300);

    move_forward(-35, 1500, false);

    chassis.turnToHeading(0, 700, {}, false);

    move_forward(23, 700, false);
    
    pros::delay(100);

    chassis.turnToHeading(-70, 700, {}, false);

    move_forward(40, 2000, false, {.maxSpeed=70});
}


inline void rightRed(){
    chassis.moveToPoint(3, 37, 1300, {.minSpeed=10}, false);
    doinker.set_value(1);
    pros::delay(200);
    chassis.moveToPoint(0, 25, 2000, {.forwards=false}, false);
    doinker.set_value(0);

    // Grab the mogo

    chassis.turnToHeading(90, 1000, {}, false);
    move_forward(-14, 1000, false);
    pros::delay(250);
    getMogo();

    intake = 100;

    pros::delay(300);

    mogo.set_value(0);

    chassis.turnToHeading(-135, 700, {}, false);

    move_forward(-24, 1000, false);

    pros::delay(300);
    getMogo();

    chassis.turnToHeading(120, 700, {}, false);

    move_forward(15, 700, false);

    pros::delay(300);

    chassis.turnToHeading(-20, 700, {}, false);

    move_forward(-30, 1000, false);

    mogo.set_value(0);

    move_forward(25, 1000, false);

    chassis.turnToHeading(90, 700, {}, false);

    move_forward(-25, 700, false);

    pros::delay(100);
    getMogo();


}

    
inline void soloAWPRed(){
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