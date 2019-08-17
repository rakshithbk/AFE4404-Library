#include "AFE_connect.h"


//Wire.begin();
//Prototype declarations
	void AFE_Reg_Write (int reg_address, uint32_t data);
	uint32_t AFE_Reg_Read(int reg_address);
	void I2C_write (int slave_address, int reg_address, byte configData[], int byteCount);
	void I2C_read(int slave_address, int reg_address, byte *read_Data, int byteCount);
	void basic_settings();
	void set_led2_start_end(uint16_t start, uint16_t end);
	void set_led2_sample_start_end(uint16_t start, uint16_t end);
	void set_adc_reset0_start_end(uint16_t start, uint16_t end);
	void set_led2_convert_start_end(uint16_t start, uint16_t end);
	void set_led3_start_stop(uint16_t start, uint16_t end);
	void set_led3_sample_start_end(uint16_t start, uint16_t end);
	void set_adc_reset1_start_end(uint16_t start, uint16_t end);
	void set_led3_convert_start_end(uint16_t start, uint16_t end);
	void set_led1_start_end(uint16_t start, uint16_t end);
	void set_led1_sample_start_end(uint16_t start, uint16_t end);
	void set_adc_reset2_start_end(uint16_t start, uint16_t end);
	void set_led1_convert_start_end(uint16_t start, uint16_t end);
	void set_amb1_sample_start_end(uint16_t start, uint16_t end);
	void set_adc_reset3_start_end(uint16_t start, uint16_t end);
	void set_amb1_convert_start_end(uint16_t start, uint16_t end);
	void set_pdn_cycle_start_end(uint16_t start, uint16_t end);
	void set_prpct_count(uint16_t count);
	void set_timer_and_average_num(bool enable, int avg_cnt);
	void set_seperate_tia_gain(bool separate, uint8_t cf_setting, uint8_t gain_setting);
	void set_tia_gain( bool replace, uint8_t cf_setting, uint8_t gain_setting );
	void set_led_currents( uint8_t led1_current, uint8_t led2_current, uint8_t led3_current);
	void set_clkout_div( bool enable, uint8_t div );
	void set_int_clk_div( uint8_t div );

//Function definitions

void AFE::init()
{
	Wire.begin();
	basic_settings();
	Wire.beginTransmission(AFE_ADDR);
	if(Wire.endTransmission() != 0)
			Serial.println("Unable to communicate with AFE4404.\n Please check the connections");
	// All the below values should be less than 65535 (16 bits)
	set_led2_start_end( 0, 399 );
	set_led2_sample_start_end( 80, 399 );
	set_adc_reset0_start_end( 401, 407 );
    set_led2_convert_start_end( 408, 1467 );
    set_led3_start_stop( 400, 799 );
    set_led3_sample_start_end( 480, 799 );
    set_adc_reset1_start_end( 1469, 1475 );
    set_led3_convert_start_end( 1476, 2535 );
    set_led1_start_end( 800, 1199 );
    set_led1_sample_start_end( 880, 1199 );
    set_adc_reset2_start_end( 2537, 2543 );
    set_led1_convert_start_end( 2544, 3603 );
    set_amb1_sample_start_end( 1279, 1598 );
    set_adc_reset3_start_end( 3605, 3611 );
    set_amb1_convert_start_end( 3612, 4671 );
    set_pdn_cycle_start_end( 5471, 39199 );
    set_prpct_count( 39999 );  

    set_timer_and_average_num( true, 3 ); // take ADC readings 3 times and avg them to reduce noise 
    									  // 4 bit resolution (0 = no avg (only 1 read) and 15 = 16 reads and avg them when sending)
    									  // takes more time when avg num is increased
    set_seperate_tia_gain( true, 0, 4 );  // parm1 -> true = separate TIA, | parm2 -> C2 val.(0-7) , | parm3 -> R2 val.(0-7)
    									  // TODO : what is its use?
    set_tia_gain( false, 0, 3 );		  // parm1 -> true =  control with PROG_TG_STC and ENDC, | parm2 -> C1 val.(0-7) , | parm3 -> R1 val.(0-7)
    set_led_currents( 15,3,3 );			  // parm1 -> LED1, | parm2 -> LED2, | parm3 -> LED3,    each is 6 bit resolution (0-63)
    set_clkout_div( false, 2 );			  // parm1 -> In internal clock mode, generated clk can be o/p on CLK pin.
    									  // parm2 -> if parm1 = true, set o/p freq as per DIV table. (value from 1 - 7)
    set_int_clk_div( 1 );				  //parm1 -> Clock division ratio for the clock to the timing engine. (value from 1-7)

}


uint32_t AFE::get_led1_val()
{
	return AFE_Reg_Read(LED1VAL);
}

uint32_t AFE::get_led2_val()
{
	return AFE_Reg_Read(LED2VAL);
}

uint32_t AFE::get_led3_val()
{
	return AFE_Reg_Read(LED3VAL);
}



void basic_settings()
{
	AFE_Reg_Write(DIAGNOSIS, 8); // 8-> 4th bit = 1. (software reset enabled). 
								 // 2nd bit -> keep counter in reset mode
								 // 1st bit -> Register write mode (see datasheet AFE4404 -> REG 0x00h)
	uint32_t reg_val =0;
	reg_val |= (0 << 20);		//	0: Transmitter not pwrd dwn |  1: pwrd dwn in dynamic pwrd-dwn mode 
	reg_val |= (1 << 17);		//	0: LED crrnt range 0-50 |   1: range 0-100
	reg_val |= (1 << 14);		//	0: ADC not pwrd dwn |  1: ADC pwrd dwn in dynamic pwrd-dwn mode
	reg_val |= (1 << 9);		//	0: External Clock (clk pin acts as input in this mode) | 1: Internal Clock (uses 4Mhz internal osc)
	reg_val |= (1 << 4);		//	0: TIA not pwrd dwn |  1: TIA pwrd dwn in dynamic pwrd-dwn mode
	reg_val |= (0 << 3);		//	0: Rest of ADC is not pwrd dwn |  1: Is pwrd dwn in dynamic pwrd-dwn mode
	reg_val |= (0 << 1);		//	0: Normal Mode |  1: RX portion of AFE pwrd dwn  
	reg_val |= (0 << 0);		//	0: Normal Mode |  1: Entire AFE is pwrd dwn   
	AFE_Reg_Write(SETTINGS, reg_val);
}


inline void set_led2_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(LED2_ST, start);
	AFE_Reg_Write(LED2_END, end);
}

inline void set_led2_sample_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(SMPL_LED2_ST, start);
	AFE_Reg_Write(SMPL_LED2_END, end);
}

inline void set_adc_reset0_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(ADC_RST_P0_ST, start);
	AFE_Reg_Write(ADC_RST_P0_END, end);
}

inline void set_led2_convert_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(LED2_CONV_ST, start);
	AFE_Reg_Write(LED2_CONV_END, end);
}

inline void set_led3_start_stop(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(LED3LEDSTC, start);
	AFE_Reg_Write(LED3LEDENDC, end);
}

inline void set_led3_sample_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(SMPL_LED3_ST, start);
	AFE_Reg_Write(SMPL_LED3_END, end);
}

inline void set_adc_reset1_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(ADC_RST_P1_ST, start);
	AFE_Reg_Write(ADC_RST_P1_END, end);
}

inline void set_led3_convert_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(LED3_CONV_ST, start);
	AFE_Reg_Write(LED3_CONV_END, end);
}

inline void set_led1_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(LED1_ST, start);
	AFE_Reg_Write(LED1_END, end);
}

inline void set_led1_sample_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(SMPL_LED1_ST, start);
	AFE_Reg_Write(SMPL_LED1_END, end);
}

inline void set_adc_reset2_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(ADC_RST_P2_ST, start);
	AFE_Reg_Write(ADC_RST_P2_END, end);
}

inline void set_led1_convert_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(LED1_CONV_ST, start);
	AFE_Reg_Write(LED1_CONV_END, end);
}

inline void set_amb1_sample_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(SMPL_AMB1_ST, start);
	AFE_Reg_Write(SMPL_AMB1_END, end);
}

inline void set_adc_reset3_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(ADC_RST_P3_ST, start);
	AFE_Reg_Write(ADC_RST_P3_END, end);
}

inline void set_amb1_convert_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(AMB1_CONV_ST, start);
	AFE_Reg_Write(AMB1_CONV_END, end);
}

inline void set_pdn_cycle_start_end(uint16_t start, uint16_t end)
{
	AFE_Reg_Write(PDNCYCLESTC, start);
	AFE_Reg_Write(PDNCYCLEENDC, end);
}

inline void set_prpct_count(uint16_t count)
{
	AFE_Reg_Write(PRPCT, count);
}

inline void set_timer_and_average_num(bool enable, int avg_cnt)
{
	uint32_t temp = 0;
	temp |= (enable << 8);	// Timer Enable bit -> to use internal timing engine to do sync for sampling, data conv etc.
	temp |= avg_cnt;  		// LSB 4 bits (aka 0 - 15)
	AFE_Reg_Write(TIM_NUMAV, temp);
}

inline void set_seperate_tia_gain(bool separate, uint8_t cf_setting, uint8_t gain_setting)
{
	uint16_t val =0;
	val |= (separate << 15); 	//  Separate TIA gains if this bit = 1; else single gain if = 0;
	val |= (cf_setting << 3);	//  Control of C2 settings (3 bits -> 0-7)
	val |= (gain_setting << 0);	//  Control of R2 settings (3 bits -> 0-7)
	AFE_Reg_Write(TIA_GAINS2, val);
}

inline void set_tia_gain( bool replace, uint8_t cf_setting, uint8_t gain_setting )
{
	uint16_t val =0;
	val |= (replace << 8);			//  making 1 will replace ADC_RDY output with signal from timing engine.
									//  controlled by PROG_TG_STC and PROG_TG_ENDC regs.
	val |= (cf_setting << 3);		//  Control of C1 settings (3 bits -> 0-7)
	val |= (gain_setting << 0);		//  Control of R1 settings (3 bits -> 0-7)
									//  if separate TIA gains is not enabled, then these will control both (C1 & C2) and (R1 & R2)
	AFE_Reg_Write(TIA_GAINS1, val);
}

inline void set_led_currents( uint8_t led1_current, uint8_t led2_current, uint8_t led3_current)
{
	uint32_t val;
	val |= (led1_current << 0);		// LED 1 addrss space -> 0-5 bits
	val |= (led2_current << 6);		// LED 2 addrss space -> 6-11 bits
	val |= (led3_current << 12);	// LED 3 addrss space -> 12-17 bits
	AFE_Reg_Write(LED_CONFIG, val);
}

inline void set_clkout_div( bool enable, uint8_t div )
{
	uint16_t val = 0;
	val |= (enable << 9);	//	Enable clock output if enable = 1
	val |= (div << 1);		//	division value (1 - 7)
	AFE_Reg_Write(CLKOUT, val);
}

inline void set_int_clk_div( uint8_t div )
{
	AFE_Reg_Write(CLKDIV_PRF, div);		//	clock div 0->4Mhz, 1=2=3 -> do not use, 4-> 2Mhz, 5->1Mhz, 6->0.5Mhz, 7-> 0.25Mhz
}



void AFE_Reg_Write (int reg_address, uint32_t data)
{
  byte configData[3];
  configData[0]=(byte)(data >>16);
  configData[1]=(byte)(((data & 0x00FFFF) >>8));
  configData[2]=(byte)(((data & 0x0000FF)));
  I2C_write(AFE_ADDR, reg_address, configData, 3); 
}

inline uint32_t AFE_Reg_Read(int reg_address)
{
  byte configData[3];
  signed long retVal;
  I2C_read (AFE_ADDR, reg_address, configData, 3);
  retVal = configData[0];
  retVal = (retVal << 8) | configData[1];
  retVal = (retVal << 8) | configData[2];
  if (reg_address >= 0x2A && reg_address <= 0x2F)
  {
    if (retVal & 0x00200000)  // check if the ADC value is positive or negative
    {
      retVal &= 0x003FFFFF;   // convert it to a 22 bit value
      return (retVal^0xFFC00000);
    }
  }
  return retVal;
}

void I2C_write (int slave_address, int reg_address, byte configData[], int byteCount)
{
  signed long retVal;
  
  Wire.beginTransmission(slave_address);
  Wire.write(reg_address);
  // Serial.print(configData[0]);
  // Serial.print(",");
  // Serial.print(configData[1]);
  // Serial.print(",");
  // Serial.println(configData[2]);
  retVal = configData[0];
  retVal = (retVal << 8) | configData[1];
  retVal = (retVal << 8) | configData[2];
  // Serial.println(retVal);
  // Serial.println(reg_address);
  Wire.write(configData, 3);
  Wire.endTransmission();
}

inline void I2C_read(int slave_address, int reg_address, byte *read_Data, int byteCount)
{ 
  Wire.beginTransmission(slave_address);
  Wire.write(reg_address);
  Wire.endTransmission();
  Wire.requestFrom(slave_address, 3);
  while(Wire.available() && (byteCount != 0))
  {
    *read_Data++ = Wire.read();
    byteCount--;
  }
}