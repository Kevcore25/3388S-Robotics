// #pragma once
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"

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


    // move_forward(26, 1000);

    // move_forward(-14, 1000);

    // chassis.turnToHeading(183, 1000, {}, false);

    // move_forward(26, 1000);

    // move_forward(-28, 1000);

    // chassis.turnToHeading(200, 500, {}, false);

    // move_forward(17, 700);

    // move_forward(-30, 1500);


}
inline void rightBLUE() {
    chassis.setPose(0, 0, 90);

    // Get alliance stake
    chassis.turnToHeading(45, 500, {}, false);
    LBmoveToAngle(170, 100, 5, 2000);
    pros::delay(100);

    // Get Mogo
    chassis.moveToPoint(-15, -32, 2000, {.forwards=false, .maxSpeed=100}, false);
    pros::delay(590);
    getMogo();    pros::Task backLB(resetBackLB);


    // Get 2 rings
    quick_turn_to(-135, 1000);
    intake=100;
    chassis.moveToPose(-54, -57, -90, 3000, {.lead=0.7,.minSpeed=50}, false);
    pros::delay(2000);

    // Get the 1 ring in the middle of the field 
    chassis.moveToPose(-15, -45, -90, 2000, {.forwards=false, .lead=0.7, .minSpeed=70});
    chassis.moveToPoint(-36, -28, 2000);
    pros::delay(500);
    chassis.moveToPose(12, -18, 90, 3000, {.maxSpeed=80});
    chassis.moveToPoint(0, -46, 3000, {.maxSpeed=70});
}

inline void leftRED() {
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

    // chassis.swingToPoint(10, 30, lemlib::DriveSide::RIGHT, 1000);
    // quick_turn_to(-55, 750);
    // move_forward(15, 1000);
    // pros::delay(500);
    // mogo.set_value(0);
    // move_forward(-16, 1000);
    // quick_turn_to(-135, 1000);
    // chassis.moveToPoint(4, 32, 2000, {.forwards=false,.maxSpeed=100}, false);
    // getMogo();

    // // Get the ring
    // chassis.moveToPose(16.5, 33, 30, 1000);

    // chassis.moveToPose(4, 0, 0, 3000, {.forwards=false, .minSpeed=60}, false);    
    // mogo.set_value(0);
    // move_forward(4, 500);
    // quick_turn_to(-130);

    // // Go clear the corner lol
    // // doinker.set_value(1);
    // // chassis.moveToPose(-4.5, -6, -180, 3000);
    // chassis.turnToHeading(-245, 1000, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE}, false);
    // doinker.set_value(0);
    
    
    

    // chassis.moveToPoint(3, 37, 1300, {.minSpeed=10}, false);
    // doinker.set_value(1);
    // pros::delay(200);
    // chassis.moveToPoint(0, 25, 2000, {.forwards=false}, false);
    // doinker.set_value(0);

    // // Grab the mogo

    // chassis.turnToHeading(90, 1000, {}, false);
    // move_forward(-14, 1000, false);
    // pros::delay(250);
    // getMogo();

    // intake = 100;

    // pros::delay(300);

    // mogo.set_value(0);

    // chassis.turnToHeading(-135, 700, {}, false);

    // move_forward(-24, 1000, false);

    // pros::delay(300);
    // getMogo();

    // chassis.turnToHeading(120, 700, {}, false);

    // move_forward(15, 700, false);

    // pros::delay(300);

    // chassis.turnToHeading(-20, 700, {}, false);

    // move_forward(-30, 1000, false);

    // mogo.set_value(0);

    // move_forward(25, 1000, false);

    // chassis.turnToHeading(90, 700, {}, false);

    // move_forward(-25, 700, false);

    // pros::delay(100);
    // getMogo();


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