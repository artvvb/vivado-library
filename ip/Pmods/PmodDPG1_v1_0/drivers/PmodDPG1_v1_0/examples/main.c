/******************************************************************************/
/*                                                                            */
/* main.c -- Example program using the PmodDPG1 IP                            */
/*                                                                            */
/******************************************************************************/
/* Author: Jon Peyron                                                         */
/*                                                                            */
/******************************************************************************/
/* File Description:                                                          */
/*                                                                            */
/* This demo continuously the PmodDPG1 for pressure measurements and prints   */
/* the data in 6 different units (kPa, atm, psi, inH2O, cmH2O, and mmHg).     */
/*                                                                            */
/******************************************************************************/
/* Revision History:                                                          */
/*                                                                            */
/*    08/19/2016(jpeyron):  Created                                           */
/*    09/21/2017(atangzwj): Validated for Vivado 2015.4                       */
/*                                                                            */
/******************************************************************************/

/************ Include Files ************/

#include "PmodDPG1.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "xparameters.h"

#ifdef XPAR_MICROBLAZE_ID
#include "microblaze_sleep.h"
#else
#include "sleep.h"
#endif


/************ Macro Definitions ************/

// Pressure unit types
#define KPA   DPG1_KPA_PRESSURE_TYPE
#define ATM   DPG1_ATM_PRESSURE_TYPE
#define PSI   DPG1_PSI_PRESSURE_TYPE
#define INH2O DPG1_INH2O_PRESSURE_TYPE
#define CMH2O DPG1_CMH2O_PRESSURE_TYPE
#define MMHG  DPG1_MMHG_PRESSURE_TYPE


/************ Function Prototypes ************/

void DemoInitialize();

void DemoRun();

void DemoCleanup();

void EnableCaches();

void DisableCaches();


/************ Global Variables ************/

PmodDPG1 myDevice;


/************ Function Definitions ************/

int main(void) {
   DemoInitialize();
   DemoRun();
   DemoCleanup();
   return 0;
}

void DemoInitialize() {
   EnableCaches();

   DPG1_begin(&myDevice, XPAR_PMODDPG1_0_AXI_LITE_SPI_BASEADDR);
}

void DemoRun() {
   xil_printf("starting...\n\r");

   while (1) {
      int rawData;
      double pressureValue;
      DPG1_readData(&myDevice);

      // read raw data from PmodDPG1 and print it
      rawData = (int) myDevice.data;
      xil_printf("Raw data: %d \n\r", rawData); // format text for display

      int whole; // used to get the whole number for the float
      int not_whole;// used to get the non whole number for the float

      // convert raw data into units of kPa and print it
      pressureValue = DPG1_GetPressure(KPA, &myDevice);
      whole = pressureValue; // get whole number part of the physical value
      not_whole = (pressureValue - whole) * 1000000; // get fractional part
      xil_printf("kPa: %d.%06d \n\r", whole, not_whole); // format and print

      // convert raw data into units of atm and print it
      pressureValue = DPG1_GetPressure(ATM, &myDevice);
      whole = pressureValue;
      not_whole = (pressureValue - whole) * 1000000;
      xil_printf("atm: %d.%06d \n\r", whole, not_whole);

      // convert raw data into units of and print it
      pressureValue = DPG1_GetPressure(PSI, &myDevice);
      whole = pressureValue;
      not_whole = (pressureValue - whole) * 1000000;
      xil_printf("psi: %d.%06d \n\r", whole, not_whole);

      // convert raw data into units of and print it
      pressureValue = DPG1_GetPressure(INH2O,&myDevice);
      whole = pressureValue;
      not_whole = (pressureValue - whole) * 1000000;
      xil_printf("inH20: %d.%06d \n\r", whole, not_whole);

      // convert raw data into units of and print it
      pressureValue = DPG1_GetPressure(CMH2O,&myDevice);
      whole = pressureValue;
      not_whole = (pressureValue - whole) * 1000000;
      xil_printf("cmH20: %d.%06d \n\r", whole, not_whole);

      // convert raw data into units of and print it
      pressureValue = DPG1_GetPressure(MMHG, &myDevice);
      whole = pressureValue;
      not_whole = (pressureValue - whole) * 1000000;
      xil_printf("mmHg: %d.%06d \n\r", whole, not_whole);

      xil_printf("\n\r\n\r");

#ifdef XPAR_MICROBLAZE_ID
      MB_Sleep(1200);
#else
      sleep(2);
#endif
   }
}

void DemoCleanup() {
   DPG1_end(&myDevice);
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
