
#ifndef MOTORFEEDBACK_H
#define MOTORFEEDBACK_H

/************ Include Files ************/

#include "xil_types.h"
#include "xstatus.h"


/************ Macro Definitions ************/

#define CLEAR_OFFSET    0x00
#define M1_POS_OFFSET   0x04
#define M2_POS_OFFSET   0x08
#define POS_DIFF_OFFSET 0x0C
#define CLK_OFFSET      0x10


/**************************** Type Definitions *****************************/
/**
 *
 * Write a value to a MOTORFEEDBACK register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the MOTORFEEDBACKdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void MOTORFEEDBACK_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define MOTORFEEDBACK_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a MOTORFEEDBACK register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the MOTORFEEDBACK device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 MOTORFEEDBACK_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define MOTORFEEDBACK_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the MOTORFEEDBACK instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus MOTORFEEDBACK_Reg_SelfTest(void * baseaddr_p);


/************ Type Definitions ************/

typedef struct MotorFeedback {
   u32 baseAddr;
   u32 clkFreqHz;
   u32 edgesPerRev;
   u32 gearboxRatio;
} MotorFeedback;


/************ Function Prototypes ************/

void initMotorFeedback(MotorFeedback* motorFeedback,
                       u32 baseAddr,
                       u32 clkFreqHz,
                       u32 edgesPerRev,
                       u32 gearboxRatio);

void getMotorSpeeds(MotorFeedback* motorFeedback, int motor_speed[]);

int16_t getDistanceTraveled(MotorFeedback* motorFeedback);

void getEdgeCounts(MotorFeedback* motorFeedback, int m1[], int m2[]);

void clearSpeedCounters(MotorFeedback* motorFeedback);

void getMotorPositions(MotorFeedback* motorFeedback, int16_t motor_pos[]);

int16_t getPositionDifference(MotorFeedback* motorFeedback);

void clearPosCounter(MotorFeedback* motorFeedback);

#endif // MOTORFEEDBACK_H
