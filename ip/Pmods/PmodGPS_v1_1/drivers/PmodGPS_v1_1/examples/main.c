/******************************************************************************/
/*                                                                            */
/* main.c -- Example program using the PmodGPS IP                             */
/*                                                                            */
/******************************************************************************/
/* Author: Thomas Kappenman                                                   */
/*                                                                            */
/******************************************************************************/
/* File Description:                                                          */
/*                                                                            */
/* This demo continuously prints information given by the PmodGPS, including  */
/* latitude, longitude, altitude, and number of satellites.                   */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    04/17/2016(tom21091): Created                                           */
/*    09/26/2017(atangzwj): Validated for Vivado 2015.4                       */
/*                                                                            */
/******************************************************************************/

/************ Include Files ************/

#include "PmodGPS.h"
#include "xil_cache.h"
#include "xparameters.h"


/************ Function Prototypes ************/

void DemoInitialize();

void DemoRun();

void EnableCaches();

void DisableCaches();


/************ Global Variables ************/

PmodGPS GPS;


/************ Function Definitions ************/

int main(void) {
   DemoInitialize();
   DemoRun();
   return 0;
}

void DemoInitialize() {
   GPS_begin(&GPS, XPAR_PMODGPS_0_AXI_LITE_GPIO_BASEADDR,
         XPAR_PMODGPS_0_AXI_LITE_UART_BASEADDR);
#ifndef NO_IRPT // If there are interrupts
#ifdef XPAR_MICROBLAZE_ID
   // Set up interrupts for a MicroBlaze system
   GPS_SetupInterruptSystem(&GPS, XPAR_INTC_0_DEVICE_ID,
         XPAR_INTC_0_PMODGPS_0_VEC_ID);
#else
   // Set up interrupts for a Zynq system
   GPS_SetupInterruptSystem(&GPS, XPAR_PS7_SCUGIC_0_DEVICE_ID,
         XPAR_FABRIC_PMODGPS_0_GPS_UART_INTERRUPT_INTR);
#endif
#endif

   GPS_setUpdateRate(&GPS, 1000);
}

void DemoRun() {
   while (1) {
#ifdef NO_IRPT // If no interrupts
      GPS_getData(&GPS); // Receive data from GPS in Polled Mode
#endif
      if (GPS.ping) {
         GPS_formatSentence(&GPS);
         if (GPS_isFixed(&GPS)) {
            xil_printf("Latitude: %s\n\r", GPS_getLatitude(&GPS));
            xil_printf("Longitude: %s\n\r", GPS_getLongitude(&GPS));
            xil_printf("Altitude: %s\n\r", GPS_getAltitudeString(&GPS));
            xil_printf("Number of Satellites: %d\n\n\r", GPS_getNumSats(&GPS));
         } else {
            xil_printf("Number of Satellites: %d\n\r", GPS_getNumSats(&GPS));
         }
         GPS.ping = GPS_false;
      }
   }
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
