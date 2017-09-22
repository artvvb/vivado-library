/******************************************************************************/
/*                                                                            */
/* main.c -- Example program using the PmodDHB1 IP                            */
/*                                                                            */
/******************************************************************************/
/* Author: Arvin Tang                                                         */
/*                                                                            */
/******************************************************************************/
/* File Description:                                                          */
/*                                                                            */
/* This demo drives 2 motors in the 4 possible directions. When mounted on a  */
/* 2-wheel chassis, the motors will be driven such that the robot goes        */
/* forward, goes backward, turns left, and turns right.                       */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    09/14/2017(atangzwj): Created                                           */
/*                                                                            */
/******************************************************************************/

/************ Include Files ************/

#include "MotorFeedback.h"
#include "PmodDHB1.h"
#include "PWM.h"
#include "sleep.h"
#include "xil_cache.h"


/************ Macro Definitions ************/

#define GPIO_BASEADDR     XPAR_PMODDHB1_0_AXI_LITE_GPIO_BASEADDR
#define PWM_BASEADDR      XPAR_PMODDHB1_0_PWM_AXI_BASEADDR
#define MOTOR_FB_BASEADDR XPAR_PMODDHB1_0_MOTOR_FB_AXI_BASEADDR

#define CLK_FREQ XPAR_CPU_M_AXI_DP_FREQ_HZ


/************ Function Prototypes ************/

void DemoInitialize();

void DemoRun();

void DemoCleanup();

void drive(int16_t sensor_edges);

void EnableCaches();

void DisableCaches();


/************ Global Variables ************/

PmodDHB1 pmodDHB1;
MotorFeedback motorFeedback;


/************ Function Definitions ************/

int main(void) {
   DemoInitialize();
   DemoRun();
   DemoCleanup();
   return 0;
}

void DemoInitialize() {
   EnableCaches();
   usleep(100000);
   DHB1_begin(&pmodDHB1, GPIO_BASEADDR, PWM_BASEADDR);
   MotorFeedback_init(&motorFeedback, MOTOR_FB_BASEADDR, CLK_FREQ, 4, 48);
   PWM_Disable(PWM_BASEADDR);
}

void DemoRun() {
   DHB1_setMotorSpeeds(&pmodDHB1, 50, 50);

   MotorFeedback_clearPosCounter(&motorFeedback);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before setting/changing direction,
   usleep(6);                 // short circuit possible otherwise
   DHB1_setDirs(&pmodDHB1, 0, 1);  // Set direction forward
   drive(240);
   usleep(2000);

   PWM_Disable(PWM_BASEADDR);
   usleep(6);
   DHB1_setDirs(&pmodDHB1, 1, 0);  // Set direction backward
   drive(240);
   usleep(2000);

   PWM_Disable(PWM_BASEADDR);
   usleep(6);
   DHB1_setDirs(&pmodDHB1, 1, 1);  // Set direction left
   drive(120);
   usleep(2000);

   PWM_Disable(PWM_BASEADDR);
   usleep(6);
   DHB1_setDirs(&pmodDHB1, 0, 0);  // Set direction right
   drive(120);
}

void DemoCleanup() {
   DisableCaches();
}

void drive(int16_t sensor_edges) {
   PWM_Enable(PWM_BASEADDR);
   int16_t dist = MotorFeedback_getDistanceTraveled(&motorFeedback);
   while (dist < sensor_edges) {
      dist = MotorFeedback_getDistanceTraveled(&motorFeedback);
   }
   MotorFeedback_clearPosCounter(&motorFeedback);
   PWM_Disable(PWM_BASEADDR);
}

void EnableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheEnable();
#endif
#endif
}

void DisableCaches() {
#ifdef __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_DCACHE
   Xil_DCacheDisable();
#endif
#ifdef XPAR_MICROBLAZE_USE_ICACHE
   Xil_ICacheDisable();
#endif
#endif
}
