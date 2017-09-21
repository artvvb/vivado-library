/************************************************************************/
/*                                                                      */
/* main.c -- Example program using the PmodMAXSONAR IP                  */
/*                                                                      */
/************************************************************************/
/* Author: Arvin Tang                                                   */
/*                                                                      */
/************************************************************************/
/* File Description:                                                    */
/*                                                                      */
/* This demo continuously polls the PmodMAXSONAR for distance and       */
/* prints the distance.                                                 */
/*                                                                      */
/************************************************************************/
/* Revision History:                                                    */
/*                                                                      */
/*    09/11/2017(atangzwj): Created                                     */
/*                                                                      */
/************************************************************************/

/************ Include Files ************/

#include "PmodMAXSONAR.h"
#include "sleep.h"
#include "xil_cache.h"
#include "xparameters.h"


/************ Macro Definitions ************/

#define PMOD_MAXSONAR_BASEADDR XPAR_PMODMAXSONAR_0_AXI_LITE_GPIO_BASEADDR

#define CLK_FREQ XPAR_CPU_M_AXI_DP_FREQ_HZ


/************ Global Variables ************/

PmodMAXSONAR pmodMAXSONAR;


/************ Function Protoypes ************/

void DemoInitialize();

void DemoRun();

void DemoCleanup();

void EnableCaches();

void DisableCaches();


/************ Function Definitions ************/

int main(void) {
   DemoInitialize();
   DemoRun();
   DemoCleanup();
   return 0;
}

void DemoInitialize() {
   EnableCaches();
   MAXSONAR_begin(&pmodMAXSONAR, PMOD_MAXSONAR_BASEADDR, CLK_FREQ);
}


void DemoRun() {
   print("\n\r");
   u32 dist;
   while (1) {
      dist = MAXSONAR_getDistance(&pmodMAXSONAR);
      xil_printf("dist (in) = %3d\r", dist);
      usleep(200000);
   }
}

void DemoCleanup() {
   DisableCaches();
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