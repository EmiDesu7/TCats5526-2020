/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <string>

#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>

#include <frc/XboxController.h>
#include "ctre/phoenix.h"
#include <frc/DoubleSolenoid.h>
#include <frc/Solenoid.h>
#include <frc/Compressor.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/SpeedControllerGroup.h>

#include <frc/Joystick.h>

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>

class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void TestPeriodic() override;

 private:
  
  //Variable para el voltaje del motor
  double voltajeD = 0;
  
  //Controladores de motor
  WPI_VictorSPX m_Disp1{4}, m_Disp2 {5};
  WPI_VictorSPX m_IntakeM{6}, m_lMotor3{9}, m_rMotor2{8}, m_rMotor3{7}, m_rueda {3};
  WPI_TalonSRX m_Torre{11}, m_lMotor1{13}, m_lMotor2{12}, m_rMotor1{10}; 

  //Controles
  frc::XboxController m_control{0}; //El numero de los botones del control se pueden ver en DriverStation 
  frc::Joystick m_stick{1}; //El numero de los botones de la joystick estan marcados

  //Solenoides
  frc::DoubleSolenoid m_intake {2, 3};
  frc::DoubleSolenoid m_GearBox{0, 1}; 

  //Compressor
  frc::Compressor c{0};

  frc::SpeedControllerGroup m_LeftMotors{m_lMotor3, m_lMotor2, m_lMotor1}; 
  frc::SpeedControllerGroup m_RightMotors{m_rMotor3, m_rMotor2, m_rMotor1};

  frc::DifferentialDrive m_robot {m_LeftMotors, m_RightMotors}; 

  //Limelight (de aqui salen los datos)
  //  table->GetNumber("tx", 0.0); para dato en x
  //  table->GetNumber("ty", 0.0); para dato en y
  //  table->GetNumber("ta", 0.0); para dato de area 
  std::shared_ptr<NetworkTable> table = nt::NetworkTableInstance::GetDefault().GetTable("limelight"); 

  //No lo quiten porfa, esto solo es algo de prueba
  frc::SendableChooser<std::string> m_chooser;
  const std::string kAutoNameDefault = "Default";
  const std::string kAutoNameCustom = "My Auto";
  std::string m_autoSelected;
};
