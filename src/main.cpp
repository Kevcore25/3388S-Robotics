#include "main.h"
#include "autons.hpp"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rtos.hpp"
#include "robodash/views/image.hpp"
#include "robodash/views/selector.hpp"
#include <cmath>
#include <ctime>
#include <exception>
#include <ios>
#include <string>
#include <fstream>


// Declare RoboDash stuff
// ALSO why is selecting it crashing the brain bruh

void noth() {}

rd::Selector selector({
    {"Blue Ring Side", slowRightBlue, "", 240},
    {"Blue Ring Rush", ringRushBlue, "", 240},
    {"Blue Mogo Rush", mogoRushBlue, "", 240},
    {"Solo AWP Blue", soloAWPBlue, "", 240},
    {"Run Autonomous", noth, "", 120},
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
    while (ringsort.get_proximity() >= 210 && bruh < 300) {
      bruh++;
      pros::delay(1); // Add a short delay. 2 is the minimum polling rate of the
    }
   
    // Wait until a certain amount of pos before ejecting and set a timeout of a certain amount of MS in case of the chain failing
    double currentChain = chain.get_position();
    int i = 0;
    //          amt of rotations until eject    v           v   timeout in MS
    while (chain.get_position()-currentChain < 63 && i < 100) {
      i++;
      pros::delay(1);
    }

    chain.move(-15);

    pros::delay(200);
    // pros::lcd::print(6, "nope");

    ejecting = false;
  }
}



void testpid() {
  pros::MotorGroup dt = pros::MotorGroup({-20, -19, -18, 11, 12, 13});
  std::ofstream outfile;

  outfile.open("/usd/test1.txt", std::ios_base::app); // append instead of overwrite
  for (int i = 0; i < 12000; i += 1000) {
    dt.move_voltage(i);
    pros::delay(1000);
    controller.print(1, 0, "A: %.6f     ", dt.get_actual_velocity() / 1000);
    controller.print(1, 0, "T: %.6f     ", dt.get_target_velocity() / 1000);
    outfile << std::format("(%.6f,%.6f),", dt.get_target_velocity() / 1000, dt.get_actual_velocity() / 1000);
    dt.move_voltage(0);
    pros::delay(500);

    if (i == 5) {
      pros::delay(10000);
    }
  }
  outfile.close();
}

void testPID2() {
  pros::MotorGroup dt = pros::MotorGroup({-20, -19, -18, 11, 12, 13});
  std::ofstream outfile;

  outfile.open("/usd/test2.txt", std::ios_base::app); // append instead of overwrite
  double i = 0;
  dt.move_voltage(10000);
  for (int k = 0; k < 100; k++) {
    outfile << std::format("(%.3f,%.6f),", i, dt.get_actual_velocity() / 1000);
    i += 0.01;
    pros::delay(10);
  }
  outfile.close();
}


void flexWheelIntakeFunc() {
  int fwamt = 0;
  int yesfw = 0;
  int amtofdetect = 0;
  while (true) {
    // Spin the intake if on
    int intakespd = intake * 1.27;

    int prox = ringsort.get_proximity();
    int hue = ringsort.get_hue();
    if (!ejecting) {

      if (!onmatch &&prox >= 210 && ejectcounter < 3 &&
          ((team == 0 && hue >= 200 && hue <= 250) ||
           (team == 1 && hue >= 3 && hue <= 20))) {
        // console.println("Detect ring");
        amtofdetect++;
        pros::lcd::print(6, "Detected ring x%d", amtofdetect);

        pros::Task ertask(ejectring);
        // ejectring();
      } else {
        if (ejectcounter > 0) {
          ejectcounter--;
        }

        chain.move(intakespd);



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


  pros::Task putdownlb([&]() {
    armMotor.move(-127);
    pros::delay(500);

    armMotor.tare_position();
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    armMotor.brake();
  });
  
  // pros::lcd::initialize(); // initialize brain screen
  chassis.calibrate();     // calibrate sensors
  chassis.setPose(0.0, 0.0, 0.0);
  
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


      // controller.print(0,0 ,"%.1f",     chain.get_position()    );

      // Detect motor burn our
      if (chain.get_temperature() > 55) {
        controller.print(0, 0, "Intake: %.0fC ", chain.get_temperature());
      } else if (chain.get_temperature() > 55) {
        controller.print(0, 0, "LB: %.0fC   ", armMotor.get_temperature());
      } else {
        // DEBUG
        controller.print(0, 0, "%.1f %.1f %.0f  ", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
      }

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
  // std::cout << "Initialized";
  // // Yes.
  // rd::Selector::routine_t lastr;
  // selector.on_select([&lastr](std::optional<rd::Selector::routine_t> routine) {
	// 	if (routine.value().name == "Run Autonomous") {
	// 		std::cout << "Running autonomous" << std::endl;
  //     lastr.action();
	// 	} else {
  //     lastr = *routine;
  //   }
	// });
}

void disabled() {

  
}

void competition_initialize() {

  pros::Task putdownlb([&]() {
    armMotor.move(-127);
    pros::delay(500);

    armMotor.tare_position();
    armMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    armMotor.brake();
  });
  
}

void autonomous() {
    onmatch = false;
    // Set team for ring sort
    // chassis.turnToHeading(90, 10000);

    // chassis.turnToHeading(180, 10000);

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
    // ringRushBlue();
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
bool lb180 = false;

void opcontrol() {
    // onmatch = true; // Disables the ring sort

    // controller.rumble("-  -  -  .-");
  // testpid();
  // pros::delay(2000);

  // autonomous();
  // pros::delay(10000);
  pros::Task driveTask([&]() {
    while (true) {
      int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
      int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);  
      int l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
      int r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);

      // Drive function (Lemlib)
      chassis.arcade(leftY, rightX, false, 0.4);

      // Activate intake
      intake = r2 * 100 + l2 * -100;

      pros::delay(10);
    }

  });

  try {
    teamLogo.focus();
  } catch (std::exception e) {}

  while (true) {
    // Get Values of the controller
   
    int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

    int r1 = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1);
    int l1 = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);

    int a = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A);
    int b = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B);
    int x = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X);
    int y = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);

    int downArrow = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);
    int upArrow = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP);
    int rightArrow = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT);
    int leftArrow = controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT);

    // Should manual arm be on, arm motors can be moved using the joystick
    if (manualarm && !nomovearm) {
      armMotor.move(-rightY);
    }

    if (leftArrow) {
      onmatch = !onmatch;
      if (onmatch) {
        ringsort.set_led_pwm(0);
      } else {
        ringsort.set_led_pwm(50);
      }
      controller.print(2, 0, "RING SORT %s          ", onmatch ? "OFF" : "ON");
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

    if (downArrow) {
      lb180 = !lb180;
      if (lb180) {
          LBmoveToAngle(200);

      } else {
          LBmoveToAngle(0);
      }
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

 

    // Delay to save resources. This also makes sure that code runs properly
    // (e.g. 10 ticks = 100 milliseconds)
    pros::delay(10);

  }
}
