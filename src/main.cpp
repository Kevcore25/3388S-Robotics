#include "main.h"
#include "autons.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rtos.hpp"
#include "robodash/views/image.hpp"
#include "robodash/views/selector.hpp"
#include <cmath>
#include <ctime>
#include <exception>
#include <string>


// Declare RoboDash stuff
// ALSO why is selecting it crashing the brain bruh

rd::Selector selector({
    {"Red Ring Side", leftRED, "", 0},
    {"Blue Ring Side", rightBLUE, "", 240},
    {"Red Mogo Rush", rightRed, "", 0},
    {"Blue Mogo Rush", leftBLUE, "", 240},
    {"Solo AWP Blue", soloAWPBlue, "", 240},
    {"Solo AWP Red", soloAWPRed, "", 0}
});
rd::Image teamLogo("/usd/logo.bin", "Team logo");
rd::Console console;


// Set a bunch of values for controller switches
bool fwSwitch = 0;     
bool mogoValue = false; 
bool doinkerValue = false;
bool intakeValue = false;
int debugStatCount = 0;
bool goDetectRing = false;
bool yesdr = false;
bool usebrake = false;
bool manualarm = false;
int flexwheelstuckamt = 0;
bool hangbool = false;

void ejectring() {
  if (!onmatch) {
    ejectcounter++;
    ejecting = true;
    // pros::lcd::print(9, "INTAKE STOP");

    // Keep detecting until the hole is passed
    int bruh = 0;
    while (ringsort.get_proximity() >= 210 && bruh < 500) {
      bruh++;
      pros::delay(1); // Add a short delay. 2 is the minimum polling rate of the
                      // sensor so this should be 3
    }
    pros::delay(1);
    // pros::delay(50);
    // pros::delay(150);
    // pros::delay(50);

    chain.move(-15);

    pros::delay(200);
    // pros::lcd::print(6, "nope");

    ejecting = false;
  }
}



void flexWheelIntakeFunc() {
  int fwamt = 0;
  int yesfw = 0;
  while (true) {
    // Spin the intake if on
    int intakespd = intake * 1.27;

    int prox = ringsort.get_proximity();
    int hue = ringsort.get_hue();

    if (!ejecting) {

      if (prox >= 210 && ejectcounter < 3 &&
          ((team == 0 && hue >= 200 && hue <= 250) ||
           (team == 1 && hue >= 3 && hue <= 20))) {
        // console.println("Detect ring");
        pros::lcd::print(6, "detect ring   ");

        pros::Task ertask(ejectring);
        // ejectring();
      } else {
        if (ejectcounter > 0) {
          ejectcounter--;
        }

        chain.move(intakespd);

        // if (rollerIntake == 0) {
        //   rollers.move(intakespd);
        // } else {
        //   rollers.move(rollerIntake);
        // }

        if (intakespd > 100 &&
            (armMotorCounter == 0 && armMotorCounterDouble == 0 &&
             alliancecounter == 0)) {
          fwamt++;
          if (fwamt > 250 && chain.get_actual_velocity() < 15 && yesfw < 200) {
            //    pros::lcd::print(5, "Actual velocity:
            //    %i",chain.get_actual_velocity());
            // Move it back then fwd again
            chain.move(-127);
            pros::delay(150);
            fwamt = 0;
            yesfw += 30;
          } else {
            // Ring sort function
            if (!onmatch) {
              int hue = ringsort.get_hue();
            }
          }

        } else {
          fwamt = 0;
          if (yesfw > 0) {
            yesfw--;
          }
        }
      }
    }
    // Save resources
    pros::delay(5);
  }
}

void initialize() {
  // pros::lcd::initialize(); // initialize brain screen
  chassis.calibrate();     // calibrate sensors
  chassis.setPose(0.0, 0.0, 0.0);

  armMotor.move(-127);
  pros::delay(500);

  armMotor.tare_position();
  armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  armMotor.brake();
  // LBtracking.reset();
  LBtracking.set_data_rate(5);
  // LBtracking.reset_position();
  // pros::delay(100);

  // armMotor.move(0);

  ringsort.disable_gesture();
  ringsort.set_integration_time(3);
  ringsort.set_led_pwm(50);

  try {
    selector.focus();
  } catch (std::exception e) {}



  pros::Task screenTask([&]() {
    controller.clear();
    pros::Task intakeTask(flexWheelIntakeFunc);

    while (true) {

      controller.print(0, 0, "%f %f %f", chassis.getPose().x,
                       chassis.getPose().y, chassis.getPose().theta);

      if (!imu.is_installed()) {
        imudc = true;
      }
      if (imudc) {
        controller.print(2, 0, "IMU Disconnected");
      }

  
      // delay to save resources
      pros::delay(500);
    }
  });
}

void disabled() {}

void competition_initialize() {}

void stop() {
  pros::delay(2000);
  chain.move(0);
  pros::delay(1000000);
}

void autonomous() {
    // Set team for ring sort
    // chassis.turnToHeading(90, 10000);

    try {
      if (selector.get_auton().value().name == "Red Ring Side" ||
        selector.get_auton().value().name == "Red Mogo Rush" ||
        selector.get_auton().value().name == "Solo AWP Red") {
          team = 0;
      } else {
          team = 1;
      }
    } catch (int e) {}

    // Run auton
    selector.run_auton();
}

void hang() {
  hangbool = !hangbool;
  if (hangbool) {
    LBmoveToAngle(200);
  } else {
    LBmoveToAngle(0);
  }
}

/* Setup the ROBODASH setups and team */
void rdsetup() {
  if (
    selector.get_auton().value().name == "Red Ring Side" ||
    selector.get_auton().value().name == "Red Mogo Rush" ||
    selector.get_auton().value().name == "Solo AWP Red"
  ) {
    team = 0;
    console.println("Team is Red");
  } else {
    team = 1;
    console.println("Team is Blue");
  }
}

// Driver code
void opcontrol() {
  onmatch = true; // Disables the ring sort

  // autonomous();
  // pros::delay(10000);

  try {
    teamLogo.focus();
  } catch (std::exception e) {}

  while (true) {
    // Get Values of the controller
    int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

    int r1 = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1);
    int r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    int l1 = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);
    int l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

    int a = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
    int b = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B);
    int x = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
    int y = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);

    int downArrow = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);
    int upArrow = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP);
    int rightArrow = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT);

    // Should manual arm be on, arm motors can be moved using the joystick
    if (manualarm && !nomovearm) {
      armMotor.move(-rightY);
    }

    // Mogo mech code
    if (r1) {
      mogoValue = !mogoValue;
      mogo.set_value(mogoValue);
      controller.print(2, 0, "Mogo Piston %s          ", mogoValue ? "ON" : "OFF");
    }

    // Ladybrown code
    if (l1 && !movingArmMotor) {
      pros::Task armmotortask(armStagesOneRing);
    }

    // Buttons
    if (a) {
      doinkerValue = !doinkerValue;
      doinker.set_value(doinkerValue);
      controller.print(2, 0, "Doinker %s          ", doinkerValue ? "ON" : "OFF");
    }

    // Change to brake mode
    if (upArrow) {
      usebrake = !usebrake;
      // // controller.clear_line(2);
      controller.print(2, 0, "Brake Mode: %s          ",
                       usebrake ? "Hold" : "None");
    }

    // 
    if (x) {
      manualarm = !manualarm;
      // armMotor.tare_position();
      // LBtracking.reset();
      // LBtracking.reset_position();
      controller.print(2, 0, "Manual Arm %s          ",
                       doinkerValue ? "ON" : "OFF");
    }

    // Turn on hang
    if (y) {
      hang();
    }

    if (rightArrow) {
      allianceStakeCode();
    }

    // If disconnected, set the bot to be hold mode just in case
    if (controller.is_connected()) {
      if (usebrake) {
        chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
      } else {
        chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
      }

    } else {
        // this code won us provs VVV
        chassis.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    }

    // Drive function (Lemlib)
    chassis.arcade(leftY, rightX, false, 0.4);

    // Activate intake
    intake = r2 * 100 + l2 * -100;

    // Delay to save resources. This also makes sure that code runs properly
    // (e.g. 10 ticks = 100 milliseconds)
    pros::delay(10);

  }
}
