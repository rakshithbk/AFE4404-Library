#ifndef AFE_CONNECT_H
#define AFE_CONNECT_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <stdint.h>
#include <Wire.h>

/* AFE I2C address */
#define AFE_ADDR 88

/* AFE4404 Registers*/

//Diagnosis
#define DIAGNOSIS       0x00

//PRPCT ( timer counter )
#define PRPCT            0x1D   /**< Bits 0-15 for writing counter value      */

//Timer Module enable / NUMAV ( # of times to sample and average )
#define TIM_NUMAV        0x1E

#define TIA_GAINS1       0x21

//TIA Gains 2
#define TIA_GAINS2       0x20

//LED1 Start / End
#define LED1_ST          0x03
#define LED1_END         0x04

//Sample LED1 Start / End
#define SMPL_LED1_ST     0x07
#define SMPL_LED1_END    0x08

//LED1 Convert Start / End
#define LED1_CONV_ST     0x11
#define LED1_CONV_END    0x12

//Sample Ambient 1 Start / End
#define SMPL_AMB1_ST     0x0B
#define SMPL_AMB1_END    0x0C

//Ambient 1 Convert Start / End
#define AMB1_CONV_ST     0x13
#define AMB1_CONV_END    0x14

//LED2 Start / End
#define LED2_ST          0x09
#define LED2_END         0x0A

//Sample LED2 Start / End
#define SMPL_LED2_ST     0x01
#define SMPL_LED2_END    0x02

//LED2 Convert Start / End
#define LED2_CONV_ST     0x0D
#define LED2_CONV_END    0x0E

//Sample Ambient 2 ( or LED3 ) Start / End
#define SMPL_LED3_ST     0x05
#define SMPL_LED3_END    0x06

//Ambient 2 ( or LED3 ) Convert Start / End
#define LED3_CONV_ST     0x0F
#define LED3_CONV_END    0x10

//ADC Reset Phase 0 Start / End
#define ADC_RST_P0_ST    0x15
#define ADC_RST_P0_END   0x16

//ADC Reset Phase 1 Start / End
#define ADC_RST_P1_ST    0x17
#define ADC_RST_P1_END   0x18

//ADC Reset Phase 2 Start / End
#define ADC_RST_P2_ST    0x19
#define ADC_RST_P2_END   0x1A

//ADC Reset Phase 3 Start / End
#define ADC_RST_P3_ST    0x1B
#define ADC_RST_P3_END   0x1C

//LED Current Control
#define LED_CONFIG       0x22
/* ********************************************************************
 * LED current control is a 24 bit register where                     *
 * LED1: bits 0-5 LED2: bits 6-11 LED3: bits 12-17 and the rest are 0 *
 * ****************************************************************** *
 * LED1, LED2, LED3 Register Values | LED Current Setting (mA)        *
 *                   0              |              0                  *
 *                   1              |             0.8                 *
 *                   2              |             1.6                 *
 *                   3              |             2.4                 *
 *                  ...             |             ...                 *
 *                   63             |              50                 *
 **********************************************************************/


#define SETTINGS          0x23  /**< Settings Address */


//Clockout Settings
#define CLKOUT            0x29  /**< Clockout Address */
/* *****************************************************************************
  *  CLKOUT_DIV Register Settings Graph
  * ****************************************************************************
  * CLKOUT_DIV Register Settings | Division Ratio | Output Clock Freq. ( MHz ) *
  *             0                |         1      |             4              *
  *             1                |         2      |             2              *
  *             2                |         4      |             1              *
  *             3                |         8      |            0.5             *
  *             4                |        16      |           0.25             *
  *             5                |        32      |          0.125             *
  *             6                |        64      |         0.0625             *
  *             7                |       128      |        0.03125             *
  *         8..15                |    Do not use  |      Do not use            *
  *****************************************************************************/

//LED1 Output Value
#define LED1VAL           0x2C  /**< LED1 Output code in twos complement      */

//LED2 Output Value
#define LED2VAL           0x2A  /**< LED2 Output code in twos complement      */

//LED3 or Ambient 2 Value
#define LED3VAL           0x2B  /**< LED3 / Ambient 2 value in twos complement*/

//Ambient 1 Value
#define ALED1VAL          0x2D  /**< Ambient 1 value in twos complement       */

//LED1-Ambient 1 Value
#define LED1_ALED1VAL     0x2F  /**< LED1-ambient1 in twos complement         */

//LED2-Ambient 2 Value
#define LED2_ALED2VAL     0x2E  /**< LED2-ambient2 in twos complement         */


//Diagnostics Flag
#define PD_SHORT_FLAG     0x30  /**< 0: No short across PD 1: Short across PD */

//PD disconnect / INP, INN settings / EXT clock Division settings
#define PD_INP_EXT        0x31
#define PD_DISCONNECT        2  /**< Disconnects PD signals (INP, INM)        */
#define ENABLE_INPUT_SHORT   5  /**< INP, INN are shorted to VCM when TIA dwn */
#define CLKDIV_EXTMODE       0  /**< Ext Clock Div Ration bits 0-2            */

/* *****************************************************************************
  *  CLKDIV_EXTMODE Register Settings Graph
  * ****************************************************************************
  * CLKDIV_EXTMODE Settings      | Division Ratio | Allowed Clock Freq ( MHz ) *
  *             0                |         2      |           8-12             *
  *             1                |         8      |          32-48             *
  *             2                |    Do Not Use  |       Do Not Use           *
  *             3                |         8      |          48-60             *
  *             4                |        16      |          16-24             *
  *             5                |        32      |            4-6             *
  *             6                |        64      |          24-36             *
  *             7                |    Do Not Use  |       Do Not Use           *
  *****************************************************************************/

//PDN_CYCLE Start / End
#define PDNCYCLESTC       0x32  /**< Bits 0-15                                */
#define PDNCYCLEENDC      0x33  /**< Bits 0-15                                */

//Programmable Start / End time for ADC_RDY replacement
#define PROG_TG_STC       0x34  /**< Bits 0-15 Define Start Time              */
#define PROG_TG_ENDC      0x35  /**< Bits 0-15 Define End Time                */

//LED3C Start / End
#define LED3LEDSTC        0x36  /**< LED3 Start, if not used set to 0         */
#define LED3LEDENDC       0x37  /**< LED3 End, if not used set to 0           */

//PRF Clock Division settings
#define CLKDIV_PRF        0x39  /**< Clock Division Ratio for timing engine   */
/* ****************************************************************************
  *   CLKDIV_PRF Register Settings Graph
  * ****************************************************************************
  * CLKDIV_PRF Settings|Division Ratio|Clock Freq(MHz)| Lowest PRF Setting (Hz)*
  *          0         |       1      |       4       |           61           *
  *          1         |  Do Not Use  |  Do Not Use   |      Do Not Use        *
  *          2         |  Do Not Use  |  Do Not Use   |      Do Not Use        *
  *          3         |  Do Not Use  |  Do Not Use   |      Do Not Use        *
  *          4         |       2      |       2       |           31           *
  *          5         |       4      |       1       |           15           *
  *          6         |       8      |     0.5       |            8           *
  *          7         |      16      |    0.25       |            4           *
  *****************************************************************************/

//DAC Settings
#define DAC_SETTING       0x3A  /**< DAC Settings Address                     */
#define POL_OFFDAC_LED2     19  /**< Polarity for LED2                        */
#define I_OFFDAC_LED2       15  /**< Setting for LED2                         */
#define POL_OFFDAC_AMB1     14  /**< Polarity for Ambient 1                   */
#define I_OFFDAC_AMB1       10  /**< Setting for Ambient 1                    */
#define POL_OFFDAC_LED1      9  /**< Polarity for LED1                        */
#define I_OFFDAC_LED1        5  /**< Setting for LED1                         */
#define POL_OFFDAC_LED3      4  /**< Polarity for LED3                        */
#define I_OFFDAC_LED3        0  /**< Setting for LED3                         */
/* **************************************************************
 *  I_OFFDAC Register Settings                                  *
 * **************************************************************
 * * Reg. Settings | Offset Cancellation | Offset Cancellation  |
 * *               |   POL_OFFDAC = 0    |   POL_OFFDAC = 1     |
 * **************************************************************
 *        0        |          0          |           0          *
 *        1        |       0.47          |       –0.47          *
 *        2        |       0.93          |       –0.93          *
 *        3        |        1.4          |        –1.4          *
 *        4        |       1.87          |       –1.87          *
 *        5        |       2.33          |       –2.33          *
 *        6        |        2.8          |        –2.8          *
 *        7        |       3.27          |       –3.27          *
 *        8        |       3.73          |       –3.73          *
 *        9        |        4.2          |        –4.2          *
 *       10        |       4.67          |       –4.67          *
 *       11        |       5.13          |       –5.13          *
 *       12        |        5.6          |        –5.6          *
 *       13        |       6.07          |       –6.07          *
 *       14        |       6.53          |       –6.53          *
 *       15        |          7          |          –7          *
 ****************************************************************/



class AFE
{

public:
	void init();
	uint32_t get_led1_val();
	uint32_t get_led2_val();
	uint32_t get_led3_val();
	
};

#endif // AFE_CONNECT_H