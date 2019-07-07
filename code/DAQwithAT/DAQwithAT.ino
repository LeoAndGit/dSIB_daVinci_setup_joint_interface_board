
/* AT daVinci control Board
Version: 1.0
For: daVinciBoard-01
Author: Leo
Copyright 2019, github/LeoAndGit>
*/

#include <AD7173.h>

#define LED0 PB0
#define LED1 PB1
#define ADC_SS PA4
#define ADC_MISO PA6

#define J1_BRAKE_PWR PB3
#define J4_BRAKE_PWR PB4
#define J3_BRAKE_PWR PB5
#define J6_BRAKE_PWR PB6
#define J2_BRAKE_PWR PB7
#define J5_BRAKE_PWR PB8

#define ENABLED_CH 12

String inputString = "";         // a string to hold incoming data
/* ADC conversion data and STATUS register */
byte data[ENABLED_CH][4];

void setup() {
  afio_cfg_debug_ports(AFIO_DEBUG_NONE);// Stop the debug function
  // initialize the digital pin as an output.
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(ADC_SS, OUTPUT);
  pinMode(J1_BRAKE_PWR, OUTPUT);
  pinMode(J2_BRAKE_PWR, OUTPUT);
  pinMode(J3_BRAKE_PWR, OUTPUT);
  pinMode(J4_BRAKE_PWR, OUTPUT);
  pinMode(J5_BRAKE_PWR, OUTPUT);
  pinMode(J6_BRAKE_PWR, OUTPUT);
  digitalWrite(ADC_SS, LOW); //enable this device

  // initialize serial.
  Serial.begin(115200); 
  Serial.setTimeout(1000); 

  inputString.reserve(80); // allows you to allocate a buffer in memory for manipulating Strings
  
  /* initialize SPI connection to the ADC */
  AD7173.init();
  
  /* reset the ADC registers to default */
  AD7173.reset();
  
  /* set ADC input channel configuration */
  /* enable channel 0 and channel 1 and connect each to 2 analog inputs for bipolar input */
  /* CH0 - CH15 */
  /* true/false to enable/disable channel */
  /* SETUP0 - SETUP7 */
  /* AIN0 - AIN16 */
  AD7173.set_channel_config(CH0, true, SETUP0, AIN2, AIN16);
  AD7173.set_channel_config(CH1, true, SETUP0, AIN3, AIN16);
  AD7173.set_channel_config(CH2, true, SETUP0, AIN4, AIN16);
  AD7173.set_channel_config(CH3, true, SETUP0, AIN5, AIN16);
  AD7173.set_channel_config(CH4, true, SETUP0, AIN6, AIN16);
  AD7173.set_channel_config(CH5, true, SETUP0, AIN7, AIN16);
  AD7173.set_channel_config(CH6, true, SETUP0, AIN8, AIN16);
  AD7173.set_channel_config(CH7, true, SETUP0, AIN9, AIN16);
  AD7173.set_channel_config(CH8, true, SETUP0, AIN10, AIN16);
  AD7173.set_channel_config(CH9, true, SETUP0, AIN11, AIN16);
  AD7173.set_channel_config(CH10, true, SETUP0, AIN12, AIN16);
  AD7173.set_channel_config(CH11, true, SETUP0, AIN13, AIN16);
  
  /* set the ADC SETUP0 coding mode to BIPLOAR output */
  /* SETUP0 - SETUP7 */
  /* BIPOLAR, UNIPOLAR */
  AD7173.set_setup_config(SETUP0, UNIPOLAR, AIN_BUF_ENABLE, REF_INT);
  
  /* set ADC OFFSET0 offset value */
  /* OFFSET0 - OFFSET7 */
  //AD7173.set_offset_config(OFFSET0, 0);
  
  /* set the ADC FILTER0 ac_rejection to false and samplingrate to 1007 Hz */
  /* FILTER0 - FILTER7 */
  /* SPS_1, SPS_2, SPS_5, SPS_10, SPS_16, SPS_20, SPS_49, SPS_59, SPS_100, SPS_200 */
  /* SPS_381, SPS_503, SPS_1007, SPS_2597, SPS_5208, SPS_10417, SPS_15625, SPS_31250 */
  AD7173.set_filter_config(FILTER0, SPS_1007);

  /* set the ADC data and clock mode */
  /* CONTINUOUS_CONVERSION_MODE, SINGLE_CONVERSION_MODE */
  /* in SINGLE_CONVERSION_MODE after all setup channels are sampled the ADC goes into STANDBY_MODE */
  /* to exit STANDBY_MODE use this same function to go into CONTINUOUS or SINGLE_CONVERSION_MODE */
  /* INTERNAL_CLOCK, INTERNAL_CLOCK_OUTPUT, EXTERNAL_CLOCK_INPUT, EXTERNAL_CRYSTAL */
  AD7173.set_adc_mode_config(CONTINUOUS_CONVERSION_MODE, INTERNAL_CLOCK, REF_ENABLE);

  /* enable/disable CONTINUOUS_READ_MODE and appending STATUS register to data */
  /* to exit CONTINUOUS_READ_MODE use AD7173.reset(); */
  /* AD7173.reset(); returns all registers to default state, so everything has to be setup again */
  AD7173.set_interface_mode_config(false, true);

  /* wait for ADC */
  delay(10);

  digitalWrite(LED0,HIGH);// finish setup
  Serial.println("AT command Read Example:");
  }

void loop() {
// dealing with AT command
if (Serial.available() > 0) {
  inputString = String(Serial.readStringUntil(10));//search for \n
  	if(inputString.endsWith(String('\r'))){
  		if (inputString=="AT\r"){
  			Serial.println("OK");
  		} 

  		else if(inputString=="AT+FREEALL\r"){
        // release all brakes
        digitalWrite(J1_BRAKE_PWR, HIGH);
        delay(10);
        digitalWrite(J2_BRAKE_PWR, HIGH);
        delay(10);
        digitalWrite(J3_BRAKE_PWR, HIGH);
        delay(10);
        digitalWrite(J4_BRAKE_PWR, HIGH);
        delay(10);
        digitalWrite(J5_BRAKE_PWR, HIGH);
        delay(10);
        digitalWrite(J6_BRAKE_PWR, HIGH);
        digitalWrite(LED1, HIGH);
  			Serial.println("OK");
  		} 

  		else if(inputString=="AT+LOCKALL\r"){
        // lock all brakes
        digitalWrite(J1_BRAKE_PWR, LOW);
        delay(10);
        digitalWrite(J2_BRAKE_PWR, LOW);
        delay(10);
        digitalWrite(J3_BRAKE_PWR, LOW);
        delay(10);
        digitalWrite(J4_BRAKE_PWR, LOW);
        delay(10);
        digitalWrite(J5_BRAKE_PWR, LOW);
        delay(10);
        digitalWrite(J6_BRAKE_PWR, LOW);
        digitalWrite(LED1, LOW);
  			Serial.println("OK");
  		}

      else if(inputString.substring(0,8)=="AT+FREE="){
        int input =  inputString.substring(8,9).toInt();
        switch (input) {
          case 1:
            digitalWrite(J1_BRAKE_PWR, HIGH);
            digitalWrite(LED1, HIGH);
            Serial.println("OK");
            break;
          case 2:
            digitalWrite(J2_BRAKE_PWR, HIGH);
            digitalWrite(LED1, HIGH);
            Serial.println("OK");
            break;
          case 3:
            digitalWrite(J3_BRAKE_PWR, HIGH);
            digitalWrite(LED1, HIGH);
            Serial.println("OK");
            break;
          case 4:
            digitalWrite(J4_BRAKE_PWR, HIGH);
            digitalWrite(LED1, HIGH);
            Serial.println("OK");
            break;
          case 5:
            digitalWrite(J5_BRAKE_PWR, HIGH);
            digitalWrite(LED1, HIGH);
            Serial.println("OK");
            break;
          case 6:
            digitalWrite(J6_BRAKE_PWR, HIGH);
            digitalWrite(LED1, HIGH);
            Serial.println("OK");
            break;
          default:
            Serial.println("Error value");
            break;
        }
      }

      else if(inputString=="AT+READALL\r"){
        // read all channels
        /* when ADC conversion is finished */
        for (int i = 0; i <= (ENABLED_CH-1); i++) {
          while (digitalRead(ADC_MISO) == HIGH) { // wait for data
          }
          /* get ADC conversion result */
          AD7173.get_data(data[i], true);
        }
        
        // print data
        for (int i = 0; i <= (ENABLED_CH-1); i++) {
          char format[10];
          for (int j = 0; j <= 3; j++) {
              sprintf(format, "%.2X", data[i][j]);
              Serial.print(format);
            }
          Serial.println();
        }
        
        Serial.println("OK");
      }

  		else if(inputString=="AT+VERSION\r"){
  			Serial.println("AT daVinci control Board V1.0");
  			Serial.println("OK");
  		} 

  		else{
  			Serial.println("Error command");
  		}
  	}
  	else{
  		Serial.println("Error end check");
  	}
  
 }

}
