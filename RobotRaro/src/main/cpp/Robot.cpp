/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>
#include "cameraserver/CameraServer.h"


/**
 * Funcion que se corre cuando se prende el robot
 * Solo se corre una vez. 
 */
void Robot::RobotInit() {
  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
 
  m_rMotor1.SetInverted(true); 
  m_lMotor1.SetInverted (true);  
  m_rMotor2.SetInverted(true);

  m_Disp1.SetInverted (true);
  frc::CameraServer::GetInstance()->StartAutomaticCapture(); 
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {}

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit() {
  m_autoSelected = m_chooser.GetSelected();
  //m_autoSelected = SmartDashboard::GetString("Auto Selector",
     //  kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::AutonomousPeriodic() {
  double correccion = 0; 
  double errorX = table->GetNumber("tx", 0.0);

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } 
  else {
    for ( int i = 0; i <= 1000; i++ )
    {
      m_robot.CurvatureDrive (-1, 0, false);
    }
    m_robot.CurvatureDrive (0, 0, false);
  
    m_Disp1.Set(1); 
    m_Disp2.Set(-1);

    while ( errorX > 0.2 || errorX < 0.2 )
    {
      errorX = table->GetNumber("tx", 0.0);
      if(errorX > 0.2){
        correccion = errorX * 0.022 + 0.069; 
      } 
      else if(errorX < -0.2){
        correccion = errorX * 0.022  - 0.069; 
      }

      m_robot.CurvatureDrive (0, correccion, true);
    }
    
    m_robot.CurvatureDrive (0, 0, false);
    m_Torre.Set (-1);
  } 
}


/**
 * Funcion que se corre cuando se inicia el teleoperado
 * Solo se corre una vez
 */
void Robot::TeleopInit() {
  //c.SetClosedLoopControl(true); 
  m_robot.SetSafetyEnabled(true); 
  //m_medir.SetSelectedSensorPosition(0); 

  m_Disp1.Set(0); 
  m_Disp2.Set(0);
  m_Torre.Set(0);
}

/**
 * Funcion que se repite cada 15?ms en el teleoperado
 */

void Robot::TeleopPeriodic() {
  double correccion = 0; 
  double errorX = table->GetNumber("tx", 0.0);

  //Apuntar con la limelight
    //Los numeros decimales son los valores ks y kp
  if(m_control.GetRawButton(2)){
    if(errorX > 0.2){
      correccion = errorX*0.022 + 0.069; 
    } 
    else if(errorX < -0.2){
      correccion = errorX*0.022  - 0.069; 
    }
  }

  //Controlar el rodillo del intake
  if(m_control.GetRawButton(1)){
    m_IntakeM.Set(.6);  //El numero adentro es porcentaje de voltaje
  }
  else {
    m_IntakeM.Set(0); 
  }

  //Control de los pistones del intake
  if(m_stick.GetRawButton(3)){ //Arriba
    m_intake.Set(frc::DoubleSolenoid::Value::kForward); 
  }
  else if(m_stick.GetRawButton(4)){ //Abajo
    m_intake.Set(frc::DoubleSolenoid::Value::kReverse); 
  }
  else m_intake.Set(frc::DoubleSolenoid::Value::kOff); //Control

  //Cambiar el voltaje del shooter
  if(m_control.GetXButtonPressed()){ //X para sumar
    voltajeD+=0.05;
  }
  else if(m_control.GetYButtonPressed()){ //Y para restar
    voltajeD-=0.05; 
  }

  //Este es para acelerar el intake con la joystick
  if(m_stick.GetRawButton(1)){
    m_Disp1.Set(-voltajeD); 
    m_Disp2.Set(voltajeD); 
  }
  else {
    m_Disp1.Set(0); 
    m_Disp2.Set(0); 
  }
  
  //Control de la torre
  if(m_stick.GetRawButton(2)){ //Subir la pelota
    m_Torre.Set(-.8);
  }
  else if(m_stick.GetRawButton(5)){ //Bajar la pelota
    m_Torre.Set(.8); 
  }
  else if ( m_stick.GetRawButton (6) )
    m_Torre.Set (-1);

  else m_Torre.Set(0); 

  //Control de los pistones en la transmision
  if(m_control.GetBumperPressed(frc::GenericHID::JoystickHand::kLeftHand)){ //Fuerza
    m_GearBox.Set(frc::DoubleSolenoid::Value::kForward); 
  }
  else if(m_control.GetBumperPressed(frc::GenericHID::JoystickHand::kRightHand)){ //Velocidad
    m_GearBox.Set(frc::DoubleSolenoid::Value::kReverse); 
  }
  else m_GearBox.Set(frc::DoubleSolenoid::Value::kOff); 
  
  //Control del chasis del robot
  m_robot.CurvatureDrive((m_control.GetTriggerAxis(frc::GenericHID::JoystickHand::kRightHand)-m_control.GetTriggerAxis(frc::GenericHID::JoystickHand::kLeftHand))*0.7,
                           m_control.GetX(frc::GenericHID::JoystickHand::kLeftHand)*0.5 + correccion, 
                           true); 

  //output de variables por control
 // std::cout<<m_medir.GetSelectedSensorPosition(0)/4096<<" "<<m_medir.GetSelectedSensorPosition(0)<<" "<<m_medir.GetSelectedSensorVelocity(0)<<" "<<errorX<<" "<<correccion<<" "<<voltajeD<<"\n"; 
 std::cout << voltajeD << "\n";
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
