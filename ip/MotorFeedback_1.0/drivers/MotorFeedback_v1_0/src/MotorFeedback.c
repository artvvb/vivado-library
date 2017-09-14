
/************ Include Files ************/

#include "MotorFeedback.h"
#include "xil_io.h"


/************ Function Definitions ************/

/*
 * void initMotorFeedback(MotorFeedback* motorFeedback,
 *                        u32 baseAddr,
 *                        u32 clkFreqHz,
 *                        u32 edgesPerRev,
 *                        u32 gearboxRatio)
 * -----------------------------------------------------------------------------
 * Parameters:
 *       motorFeedback: Pointer to a MotorFeedback object to initialize
 *       baseAddr:      Base address of MotorFeedback registers
 *       clkFreqHz:     Frequency of MotorFeedback's clock in Hz
 *       edgesPerRev:   Number of rising edges detected by Hall effect sensor in
 *                      1 revolution of encoder disk
 *       gearboxRatio:  Wheel gear to encoder gear ratio (number of encoder
 *                      revolutions for every wheel revolution)
 *
 * Return:
 *       void
 *
 * Description:
 *       Initialize the MotorFeedback object with motor and encoder parameters
 */
void initMotorFeedback(MotorFeedback* motorFeedback,
                       u32 baseAddr,
                       u32 clkFreqHz,
                       u32 edgesPerRev,
                       u32 gearboxRatio) {
   motorFeedback->baseAddr = baseAddr;
   motorFeedback->clkFreqHz = clkFreqHz;
   motorFeedback->edgesPerRev = edgesPerRev;
   motorFeedback->gearboxRatio = gearboxRatio;
}

/*
 * void getMotorSpeeds(MotorFeedback* motorFeedback, int motor_speed[])
 * -----------------------------------------------------------------------------
 * Parameters:
 *       motorFeedback: Pointer to a MotorFeedback object to interact with
 *       motor_speed:   Array to store motor speeds
 *
 * Return:
 *       void
 *
 * Description:
 *       Measure current angular speeds of motor1 and motor2 (in RPM) and store
 *       them in motor_speed. Clears counts after taking measurements
 */
void getMotorSpeeds(MotorFeedback* motorFeedback, int motor_speed[]) {
   int m1[2];
   int m2[2];

   getEdgeCounts(motorFeedback, m1, m2);

   double conversionFactor = 60.0 * motorFeedback->clkFreqHz
                                  / motorFeedback->edgesPerRev
                                  / motorFeedback->gearboxRatio;
   motor_speed[0] = conversionFactor * m1[0] / m1[1];
   motor_speed[1] = conversionFactor * m2[0] / m2[1];
   clearSpeedCounters(motorFeedback);
}

/*
 * int16_t getDistanceTraveled()
 * -----------------------------------------------------------------------------
 * Parameters:
 *       none
 *
 * Return:
 *       Mean distance traveled by motor1 and motor2 (in sensor posedges)
 *
 * Description:
 *      Return mean distance traveled by motor1 and motor2
 */
int16_t getDistanceTraveled(MotorFeedback* motorFeedback) {
   int16_t motor_pos[2];
   getMotorPositions(motorFeedback, motor_pos);
   return (motor_pos[0] + motor_pos[1]) / 2;
}

/*
 * void getEdgeCounts(MotorFeedback* motorFeedback, int m1[], int m2[])
 * -----------------------------------------------------------------------------
 * Parameters:
 *       motorFeedback: Pointer to a MotorFeedback object to interact with
 *       m1:            Array to store motor1 sensor and clock edge counts
 *       m2:            Array to store motor2 sensor and clock edge counts
 *
 * Return:
 *       void
 *
 * Description:
 *       Store sensor and clock edge counts of each motor for computing speed
 */
void getEdgeCounts(MotorFeedback* motorFeedback, int m1[], int m2[]) {
   m1[0] = (int) (Xil_In32(motorFeedback->baseAddr + M1_POS_OFFSET) >> 16);
   m1[1] = (int)  Xil_In32(motorFeedback->baseAddr + CLK_OFFSET);

   m2[0] = (int) (Xil_In32(motorFeedback->baseAddr + M2_POS_OFFSET) >> 16);
   m2[1] = (int)  Xil_In32(motorFeedback->baseAddr + CLK_OFFSET);
}

/*
 * void clearSpeedCounters(u32 MotorFeedback* motorFeedback)
 * -----------------------------------------------------------------------------
 * Parameters:
 *       motorFeedback: Pointer to a MotorFeedback object to interact with
 *
 * Return:
 *       void
 *
 * Description:
 *       Clear the registers storing counts of sensor and clock edges for
 *       computing speed
 */
void clearSpeedCounters(MotorFeedback* motorFeedback) {
   Xil_Out8(motorFeedback->baseAddr + CLEAR_OFFSET, 0x1);
   Xil_Out8(motorFeedback->baseAddr + CLEAR_OFFSET, 0x0);
}

/*
 * void getMotorPositions(MotorFeedback* motorFeedback, int16_t motor_pos[])
 * -----------------------------------------------------------------------------
 * Parameters:
 *       motorFeedback: Pointer to a MotorFeedback object to interact with
 *       motor_pos:     Array to store positions of motor1, motor2, respectively
 *
 * Return:
 *       void
 *
 * Description:
 *       Store positions of motor1, motor2 in given array
 */
void getMotorPositions(MotorFeedback* motorFeedback, int16_t motor_pos[]) {
   motor_pos[0] = Xil_In16(motorFeedback->baseAddr + M1_POS_OFFSET);
   motor_pos[1] = Xil_In16(motorFeedback->baseAddr + M2_POS_OFFSET);
}

/*
 * int16_t getPositionDifference(MotorFeedback* motorFeedback)
 * -----------------------------------------------------------------------------
 * Parameters:
 *       motorFeedback: Pointer to a MotorFeedback object to interact with
 *
 * Return:
 *       Current position difference between motor1 and motor2
 *
 * Description:
 *       Return the difference in position (sensor edges) between motor1, motor2
 */
int16_t getPositionDifference(MotorFeedback* motorFeedback) {
   return (int16_t) Xil_In16(motorFeedback->baseAddr + POS_DIFF_OFFSET);
}

/*
 * void clearPosCounter(MotorFeedback* motorFeedback)
 * -----------------------------------------------------------------------------
 * Parameters:
 *       motorFeedback: Pointer to a MotorFeedback object to interact with
 *
 * Return:
 *       void
 *
 * Description:
 *       Clear the cumulative position counters for both motors
 */
void clearPosCounter(MotorFeedback* motorFeedback) {
   Xil_Out8(motorFeedback->baseAddr + CLEAR_OFFSET, 0x2);
   Xil_Out8(motorFeedback->baseAddr + CLEAR_OFFSET, 0x0);
}
