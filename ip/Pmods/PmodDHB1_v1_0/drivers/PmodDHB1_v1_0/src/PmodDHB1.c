/******************************************************************************/
/*                                                                            */
/* PmodDHB1.c -- Driver definitions for the PmodMAXSONAR                      */
/*                                                                            */
/******************************************************************************/
/* Author: Arvin Tang                                                         */
/*                                                                            */
/******************************************************************************/
/* File Description:                                                          */
/*                                                                            */
/* This file contains the drivers for the PmodDHB1 IP from Digilent           */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    09/13/2017(atangzwj): Created                                           */
/*                                                                            */
/******************************************************************************/

/************ Include Files ************/

#include "PmodDHB1.h"
#include "PWM.h"
#include "xil_io.h"


/************ Macro Definition ************/

#define GPIO_CHANNEL_OFFSET 0x8

#define PWM_PER 0x00030000


/************ Function Definitions ************/

/*
 * void DHB1_begin(PmodDHB1* InstancePtr, u32 GPIO_Address, u32 PWM_Address)
 * -----------------------------------------------------------------------------
 * Parameters:
 *       InstancePtr:  Pointer to a PmodDHB1 object to initialize
 *       GPIO_Address: Base address of PmodDHB1 GPIO registers
 *       PWM_Address:  Base address of PmodDHB1 PWM registers
 *
 * Return:
 *       void
 *
 * Description:
 *       Initialize the PmodDHB1 IP
 */
void DHB1_begin(PmodDHB1* InstancePtr, u32 GPIO_Address, u32 PWM_Address) {
   InstancePtr->GPIO_addr = GPIO_Address;
   InstancePtr->PWM_addr  = PWM_Address;

   PWM_Set_Period(PWM_Address, PWM_PER);
}

/*
 * void setDirs(PmodDHB1* InstancePtr, u8 dir1, u8 dir2)
 * void setDir1(PmodDHB1* InstancePtr, u8 dir1)
 * void setDir2(PmodDHB1* InstancePtr, u8 dir2)
 * -----------------------------------------------------------------------------
 * Parameters:
 *       InstancePtr: Pointer to a PmodDHB1 object to write motor directions to
 *       dir1:        8-bit value for motor1 direction, LSB determines direction
 *       dir2:        8-bit value for motor2 direction, LSB determines direction
 *
 * Return:
 *       void
 *
 * Description:
 *       Set direction pins for
 *        - Both motors
 *        - Motor1 only
 *        - Motor2 only
 *       The LSB of dir1 and dir2 determine the value of the DIR1 and DIR2 pins.
 */
void setDirs(PmodDHB1* InstancePtr, u8 dir1, u8 dir2) {
   setDir1(InstancePtr, dir1);
   setDir2(InstancePtr, dir2);
}

void setDir1(PmodDHB1* InstancePtr, u8 dir1) {
   Xil_Out8(InstancePtr->GPIO_addr, dir1);
}

void setDir2(PmodDHB1* InstancePtr, u8 dir2) {
   Xil_Out8(InstancePtr->GPIO_addr + GPIO_CHANNEL_OFFSET, dir2);
}

/*
 * void setMotorSpeeds(PmodDHB1* InstancePtr, u8 m1, u8 m2)
 * void setMotor1Speed(PmodDHB1* InstancePtr, u8 m1)
 * void setMotor2Speed(PmodDHB1* InstancePtr, u8 m2)
 * -----------------------------------------------------------------------------
 * Parameters:
 *       InstancePtr: Pointer to a PmodDHB1 object to set motor speeds for
 *       m1:          8-bit number from 0 to 100 for motor1 speed
 *       m2:          8-bit number from 0 to 100 for motor2 speed
 *
 * Return:
 *       void
 *
 * Description:
 *       Set the duty cycle for one or both motors' enable signals. 0 turns off
 *       the motor(s), 100 for max speed
 */
void setMotorSpeeds(PmodDHB1* InstancePtr, u8 m1, u8 m2) {
   setMotor1Speed(InstancePtr, m1);
   setMotor2Speed(InstancePtr, m2);
}

void setMotor1Speed(PmodDHB1* InstancePtr, u8 m1) {
   double duty_cycle = m1 / 100.0;
   PWM_Set_Duty(InstancePtr->PWM_addr, (u32) (duty_cycle * PWM_PER), 0);
}

void setMotor2Speed(PmodDHB1* InstancePtr, u8 m2) {
   double duty_cycle = m2 / 100.0;
   PWM_Set_Duty(InstancePtr->PWM_addr, (u32) (duty_cycle * PWM_PER), 1);
}
