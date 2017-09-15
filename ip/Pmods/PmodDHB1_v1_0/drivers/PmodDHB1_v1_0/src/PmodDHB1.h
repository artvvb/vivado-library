/******************************************************************************/
/*                                                                            */
/* PmodDHB1.h -- Driver definitions for the PmodMAXSONAR                      */
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

#ifndef PmodDHB1_H
#define PmodDHB1_H


/************ Include Files ************/

#include "xil_types.h"
#include "xstatus.h"


/************ Type Definitions ************/

typedef struct PmodDHB1 {
   u32 GPIO_addr;
   u32 PWM_addr;
} PmodDHB1;


/************ Function Prototypes ************/

void DHB1_begin(PmodDHB1* InstancePtr, u32 GPIO_Address, u32 PWM_Address);

void setDirs(PmodDHB1* InstancePtr, u8 dir1, u8 dir2);

void setDir1(PmodDHB1* InstancePtr, u8 dir1);

void setDir2(PmodDHB1* InstancePtr, u8 dir2);

void setMotorSpeeds(PmodDHB1* InstancePtr, u8 m1, u8 m2);

void setMotor1Speed(PmodDHB1* InstancePtr, u8 m1);

void setMotor2Speed(PmodDHB1* InstancePtr, u8 m2);

#endif // PmodDHB1_H
