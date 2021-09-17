/*
  user_config_override.h - user configuration overrides my_user_config.h for Tasmota

  Copyright (C) 2020  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

// force the compiler to show a warning to confirm that this file is included
#warning **** user_config_override.h: Using Settings from this File ****

/*****************************************************************************************************\
 * USAGE:
 *   To modify the stock configuration without changing the my_user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update my_user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/

/*
Examples :

// -- Master parameter control --------------------
#undef  CFG_HOLDER
#define CFG_HOLDER        4617                   // [Reset 1] Change this value to load SECTION1 configuration parameters to flash

// -- Setup your own Wifi settings  ---------------
#undef  STA_SSID1
#define STA_SSID1         "YourSSID"             // [Ssid1] Wifi SSID

#undef  STA_PASS1
#define STA_PASS1         "YourWifiPassword"     // [Password1] Wifi password

// -- Setup your own MQTT settings  ---------------
#undef  MQTT_HOST
#define MQTT_HOST         "your-mqtt-server.com" // [MqttHost]

#undef  MQTT_PORT
#define MQTT_PORT         1883                   // [MqttPort] MQTT port (10123 on CloudMQTT)

#undef  MQTT_USER
#define MQTT_USER         "YourMqttUser"         // [MqttUser] Optional user

#undef  MQTT_PASS
#define MQTT_PASS         "YourMqttPass"         // [MqttPassword] Optional password

// You might even pass some parameters from the command line ----------------------------
// Ie:  export PLATFORMIO_BUILD_FLAGS='-DUSE_CONFIG_OVERRIDE -DMY_IP="192.168.1.99" -DMY_GW="192.168.1.1" -DMY_DNS="192.168.1.1"'

#ifdef MY_IP
#undef  WIFI_IP_ADDRESS
#define WIFI_IP_ADDRESS   MY_IP                  // Set to 0.0.0.0 for using DHCP or enter a static IP address
#endif

#ifdef MY_GW
#undef  WIFI_GATEWAY
#define WIFI_GATEWAY      MY_GW                  // if not using DHCP set Gateway IP address
#endif

#ifdef MY_DNS
#undef  WIFI_DNS
#define WIFI_DNS          MY_DNS                 // If not using DHCP set DNS IP address (might be equal to WIFI_GATEWAY)
#endif

*/


#undef CFG_HOLDER
#define CFG_HOLDER        4617                   // [Reset 1] Change this value to load SECTION1 configuration parameters to flash

// optional: your SSID1 settings
// optional: your MQTT settings

#define USE_DISPLAY
#define USE_DISPLAY_SSD1306
#define USE_I2C
#undef USE_RULES
#define USE_SCRIPT

#undef TELE_PERIOD
#define TELE_PERIOD 15

#define USE_MHZ19
//#define USE_SENSEAIR    
//#define USE_SCD30

// CO2 branch github.com/arnold-n/Tasmota required for PRECONFIGURED_SCRIPT, START_SCRIPT_FROM_BOOT, and MHZ19B_ABC_DISABLE:
#define START_SCRIPT_FROM_BOOT
// by default, ABC enabled by *not* defining MHZ19B_ABC_DISABLE

// Script for MHZ19B:
#define PRECONFIGURED_SCRIPT ">D\n; CO2 sensor script for covid-CO2 project\n; (c) Arnold Niessen license CC BY-SA 4.0\n\n; configuration parameters:\n; g1 is CO2 threshold line\n; mx is max CO2 value in graph\n; g1+g2+mx determine LED blinking\n; Bmin,Bmax ADC values for battery display\n; tstep is graph update cycle: 30s/pixel = 1 hour cycle\n; h is stepsize dashed line\n\ng1=600\ng2=800\nmx=1000\ntstep=30\nh=4\nblnk=0\nbmin=640\nbmax=1024\n\n; other vars:\n; a,aa for battery image\n; x,x2 is graph position on graph (0..127)\n; t is time of most recent graph update (in seconds)\n; l is CO2 value\n; ft 1:first-time init; 2: wait for usefule readings\n\na=0\naa=0\nx=0\nx2=0\nt=0\nl=0\nft=1\ny=1\nftn=0\n\n>T\n; Select sensor: S8 or MHZ19B or SCD30\nl=MHZ19B#CarbonDioxide\n\n; first time: initialize display\nif ft==1 {\n=>Power on\n=>LedPower 0\n=>SetOption31 1\n=>DisplayMode 0\ny=(g1-400)/(mx/40-10)\nif y<0 {\ny=0\n}\nif y>=40 {\ny=40\n}\ny=40-y\nfor ftn 0 127 h\n=>DisplayText [C1x%0ftn%y%0y%v1]\nnext\nft=2\n}\n\n; do not print initial 0 or 410 values of sensor (for MHZ)\nif (((l>0) and (l!=410)) or (upsecs>120)) {\nft=0\n}\n\n; print CO2 ppm value as text\nif ft>0 {\n=>DisplayText [x-2y43C1f2] ---\n=>DisplayText [x66y41C1]ppm\nif (adc(7)>500) {\n; draw battery\n=>DisplayText [C1x120y42h7]\n=>DisplayText [C1x120y63h7]\n=>DisplayText [C1x120y43v20]\n=>DisplayText [C1x127y43v20]\n}\n} else {\nif l<1000 {\n=>DisplayText [x-2y43C1f2] %0l%\n} else {\n=>DisplayText [x-2y43C1f2]%0l%\n}\n=>DisplayText [x66y41C1]ppm\n\na=adc(7)\nif a>500 {\na=18*(a-bmin)/(bmax-bmin)\nif (a<1) {\na=1\n}\nif a>18 {\na=18\n}\naa=62-a\nfor ftn 122 125 1\n=>DisplayText [C1x%0ftn%y%0aa%v%0a%]\nnext\na=18-a\nif a>0 {\nfor ftn 122 125 1\n=>DisplayText [C0x%0ftn%y44v%0a%]\nnext\n}\n}\n\n; blink LED if CO2 above thresholds g1/g2\nblnk+=1\nif blnk>2 {\nblnk=0\n}\nif ((l<=g1) or ((l<=g2) and (blnk<2)) or ((l<=mx) and (blnk<1))) {\n=>LedPower 0\n} else {\n=>LedPower 1\n}\n\n; update graph every tstep seconds\nif upsecs>t {\nt+=tstep\n\n;draw pixel for CO2 value\ny=(l-400)/(mx/40-10)\nif y<0 {\ny=0\n}\nif y>=40 {\ny=40\n}\ny=40-y\n=>DisplayText [C1x%0x%y%0y%v1]\n; extend threshold line\nif x%h==0 {\ny=(g1-400)/(mx/40-10)\nif (y<0) {\ny=0\n}\nif y>=40 {\ny=40\n}\ny=40-y\n=>DisplayText [C1x%0x%y%0y%v1]\n}\n\n; clear up oldest data point\nx2=x+8\nif x2>127 {\nx2=x2-128\n}\n=>DisplayText [C0x%0x2%y0v41]\nx=x+1\nif x>=128 {\nx=0\n}\n}\n}"

// Script for S8:
//#define PRECONFIGURED_SCRIPT ">D\n; CO2 sensor script for covid-CO2 project\n; (c) Arnold Niessen license CC BY-SA 4.0\n\n; configuration parameters:\n; g1 is CO2 threshold line\n; mx is max CO2 value in graph\n; g1+g2+mx determine LED blinking\n; Bmin,Bmax ADC values for battery display\n; tstep is graph update cycle: 30s/pixel = 1 hour cycle\n; h is stepsize dashed line\n\ng1=600\ng2=800\nmx=1000\ntstep=30\nh=4\nblnk=0\nbmin=640\nbmax=1024\n\n; other vars:\n; a,aa for battery image\n; x,x2 is graph position on graph (0..127)\n; t is time of most recent graph update (in seconds)\n; l is CO2 value\n; ft 1:first-time init; 2: wait for usefule readings\n\na=0\naa=0\nx=0\nx2=0\nt=0\nl=0\nft=1\ny=1\nftn=0\n\n>T\n; Select sensor: S8 or MHZ19B or SCD30\nl=S8#CarbonDioxide\n\n; first time: initialize display\nif ft==1 {\n=>Power on\n=>LedPower 0\n=>SetOption31 1\n=>DisplayMode 0\ny=(g1-400)/(mx/40-10)\nif y<0 {\ny=0\n}\nif y>=40 {\ny=40\n}\ny=40-y\nfor ftn 0 127 h\n=>DisplayText [C1x%0ftn%y%0y%v1]\nnext\nft=2\n}\n\n; do not print initial 0 or 410 values of sensor (for MHZ)\nif (((l>0) and (l!=410)) or (upsecs>120)) {\nft=0\n}\n\n; print CO2 ppm value as text\nif ft>0 {\n=>DisplayText [x-2y43C1f2] ---\n=>DisplayText [x66y41C1]ppm\nif (adc(7)>500) {\n; draw battery\n=>DisplayText [C1x120y42h7]\n=>DisplayText [C1x120y63h7]\n=>DisplayText [C1x120y43v20]\n=>DisplayText [C1x127y43v20]\n}\n} else {\nif l<1000 {\n=>DisplayText [x-2y43C1f2] %0l%\n} else {\n=>DisplayText [x-2y43C1f2]%0l%\n}\n=>DisplayText [x66y41C1]ppm\n\na=adc(7)\nif a>500 {\na=18*(a-bmin)/(bmax-bmin)\nif (a<1) {\na=1\n}\nif a>18 {\na=18\n}\naa=62-a\nfor ftn 122 125 1\n=>DisplayText [C1x%0ftn%y%0aa%v%0a%]\nnext\na=18-a\nif a>0 {\nfor ftn 122 125 1\n=>DisplayText [C0x%0ftn%y44v%0a%]\nnext\n}\n}\n\n; blink LED if CO2 above thresholds g1/g2\nblnk+=1\nif blnk>2 {\nblnk=0\n}\nif ((l<=g1) or ((l<=g2) and (blnk<2)) or ((l<=mx) and (blnk<1))) {\n=>LedPower 0\n} else {\n=>LedPower 1\n}\n\n; update graph every tstep seconds\nif upsecs>t {\nt+=tstep\n\n;draw pixel for CO2 value\ny=(l-400)/(mx/40-10)\nif y<0 {\ny=0\n}\nif y>=40 {\ny=40\n}\ny=40-y\n=>DisplayText [C1x%0x%y%0y%v1]\n; extend threshold line\nif x%h==0 {\ny=(g1-400)/(mx/40-10)\nif (y<0) {\ny=0\n}\nif y>=40 {\ny=40\n}\ny=40-y\n=>DisplayText [C1x%0x%y%0y%v1]\n}\n\n; clear up oldest data point\nx2=x+8\nif x2>127 {\nx2=x2-128\n}\n=>DisplayText [C0x%0x2%y0v41]\nx=x+1\nif x>=128 {\nx=0\n}\n}\n}"

// Script for SCD-30:
//#define PRECONFIGURED_SCRIPT ">D\n; CO2 sensor script for covid-CO2 project\n; (c) Arnold Niessen license CC BY-SA 4.0\n\n; configuration parameters:\n; g1 is CO2 threshold line\n; mx is max CO2 value in graph\n; g1+g2+mx determine LED blinking\n; Bmin,Bmax ADC values for battery display\n; tstep is graph update cycle: 30s/pixel = 1 hour cycle\n; h is stepsize dashed line\n\ng1=600\ng2=800\nmx=1000\ntstep=30\nh=4\nblnk=0\nbmin=640\nbmax=1024\n\n; other vars:\n; a,aa for battery image\n; x,x2 is graph position on graph (0..127)\n; t is time of most recent graph update (in seconds)\n; l is CO2 value\n; ft 1:first-time init; 2: wait for usefule readings\n\na=0\naa=0\nx=0\nx2=0\nt=0\nl=0\nft=1\ny=1\nftn=0\n\n>T\n; Select sensor: S8 or MHZ19B or SCD30\nl=SCD30#CarbonDioxide\n\n; first time: initialize display\nif ft==1 {\n=>Power on\n=>LedPower 0\n=>SetOption31 1\n=>DisplayMode 0\ny=(g1-400)/(mx/40-10)\nif y<0 {\ny=0\n}\nif y>=40 {\ny=40\n}\ny=40-y\nfor ftn 0 127 h\n=>DisplayText [C1x%0ftn%y%0y%v1]\nnext\nft=2\n}\n\n; do not print initial 0 or 410 values of sensor (for MHZ)\nif (((l>0) and (l!=410)) or (upsecs>120)) {\nft=0\n}\n\n; print CO2 ppm value as text\nif ft>0 {\n=>DisplayText [x-2y43C1f2] ---\n=>DisplayText [x66y41C1]ppm\nif (adc(7)>500) {\n; draw battery\n=>DisplayText [C1x120y42h7]\n=>DisplayText [C1x120y63h7]\n=>DisplayText [C1x120y43v20]\n=>DisplayText [C1x127y43v20]\n}\n} else {\nif l<1000 {\n=>DisplayText [x-2y43C1f2] %0l%\n} else {\n=>DisplayText [x-2y43C1f2]%0l%\n}\n=>DisplayText [x66y41C1]ppm\n\na=adc(7)\nif a>500 {\na=18*(a-bmin)/(bmax-bmin)\nif (a<1) {\na=1\n}\nif a>18 {\na=18\n}\naa=62-a\nfor ftn 122 125 1\n=>DisplayText [C1x%0ftn%y%0aa%v%0a%]\nnext\na=18-a\nif a>0 {\nfor ftn 122 125 1\n=>DisplayText [C0x%0ftn%y44v%0a%]\nnext\n}\n}\n\n; blink LED if CO2 above thresholds g1/g2\nblnk+=1\nif blnk>2 {\nblnk=0\n}\nif ((l<=g1) or ((l<=g2) and (blnk<2)) or ((l<=mx) and (blnk<1))) {\n=>LedPower 0\n} else {\n=>LedPower 1\n}\n\n; update graph every tstep seconds\nif upsecs>t {\nt+=tstep\n\n;draw pixel for CO2 value\ny=(l-400)/(mx/40-10)\nif y<0 {\ny=0\n}\nif y>=40 {\ny=40\n}\ny=40-y\n=>DisplayText [C1x%0x%y%0y%v1]\n; extend threshold line\nif x%h==0 {\ny=(g1-400)/(mx/40-10)\nif (y<0) {\ny=0\n}\nif y>=40 {\ny=40\n}\ny=40-y\n=>DisplayText [C1x%0x%y%0y%v1]\n}\n\n; clear up oldest data point\nx2=x+8\nif x2>127 {\nx2=x2-128\n}\n=>DisplayText [C0x%0x2%y0v41]\nx=x+1\nif x>=128 {\nx=0\n}\n}\n}"

// Connections used in USER_TEMPLATE:
//  288 = LED1
//  608 = SCL
//  640 = SDA
// 1408 = MHZ Rx
// 1440 = MHZ Tx
// 1600 = Sair Rx
// 1632 = SAir Tx
// 4704 = ADC Analog
//
// GPIO: 0    1   2   3   4   5   9  10  12  13  14   15   16  17
// PCB:                   D2  D1         D6      D5   D8   D0  ADC
// to:                    sensor         SDA     SCL  LED1 RST bat

// MHZ19B on pins D1 and D2:
#define USER_TEMPLATE "{\"NAME\":\"CO2\",\"GPIO\":[1,1,1,1,1440,1408,1,1,640,1,608,288,1,4704],\"FLAG\":0,\"BASE\":18}"

// S8 on pins D1 and D2:
//#define USER_TEMPLATE "{\"NAME\":\"CO2\",\"GPIO\":[1,1,1,1,1600,1632,1,1,640,1,608,288,1,4704],\"FLAG\":0,\"BASE\":18}"

// SCD30 on pins SCL/SDA together with OLED
//#define USER_TEMPLATE "{\"NAME\":\"CO2\",\"GPIO\":[1,1,1,1,1,1,1,1,640,1,608,288,1,4704],\"FLAG\":0,\"BASE\":18}"

#define MODULE USER_MODULE

// mimimize size
#undef USE_EMULATION_HUE
#undef USE_EMULATION_WEMO
#undef USE_SUNRISE
#undef ROTARY_V1
#undef USE_SONOFF_RF
#undef USE_RF_FLASH
#undef USE_SONOFF_SC

#undef USE_TUYA_MCU                             // Add support for Tuya Serial MCU
#undef USE_ARMTRONIX_DIMMERS                    // Add support for Armtronix Dimmers (+1k4 code)
#undef USE_PS_16_DZ                             // Add support for PS-16-DZ Dimmer (+2k code)
#undef USE_SONOFF_IFAN                          // Add support for Sonoff iFan02 and iFan03 (+2k code)
#undef USE_BUZZER                               // Add support for a buzzer (+0k6 code)
#undef USE_ARILUX_RF                            // Add support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))
#undef USE_SHUTTER                              // Add Shutter support for up to 4 shutter with different motortypes (+11k code)
#undef USE_EXS_DIMMER                           // Add support for ES-Store WiFi Dimmer (+1k5 code)
#undef USE_DEVICE_GROUPS                        // Add support for device groups (+5k5 code)
  #undef USE_DEVICE_GROUPS_SEND                 // Add support for the DevGroupSend command (+0k6 code)
#undef USE_PWM_DIMMER                           // Add support for MJ-SD01/acenx/NTONPOWER PWM dimmers (+2k3 code, DGR=0k7)
  #undef USE_PWM_DIMMER_REMOTE                  // Add support for remote switches to PWM Dimmer (requires USE_DEVICE_GROUPS) (+0k6 code)
#undef USE_SONOFF_D1                            // Add support for Sonoff D1 Dimmer (+0k7 code)
#undef USE_SHELLY_DIMMER                        // Add support for Shelly Dimmer (+3k code)
  #undef SHELLY_CMDS                            // Add command to send co-processor commands (+0k3 code)
  #undef SHELLY_FW_UPGRADE                      // Add firmware upgrade option for co-processor (+3k4 code)
  #undef SHELLY_VOLTAGE_MON                     // Add support for reading voltage and current measurment (-0k0 code)
#undef USE_WS2812                               // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
#undef USE_MY92X1                               // Add support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#undef USE_SM16716                              // Add support for SM16716 RGB LED controller (+0k7 code)
#undef USE_SM2135                               // Add support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#undef USE_SONOFF_L1                            // Add support for Sonoff L1 led control
#undef USE_ELECTRIQ_MOODL                       // Add support for ElectriQ iQ-wifiMOODL RGBW LED controller (+0k3 code)
#undef USE_LIGHT_PALETTE                        // Add support for color palette (+0k7 code)
#undef USE_DGR_LIGHT_SEQUENCE                   // Add support for device group light sequencing (requires USE_DEVICE_GROUPS) (+0k2 code)
#undef USE_COUNTER                              // Enable inputs as counter (+0k8 code)
#undef USE_DS18x20                              // Add support for DS18x20 sensors with id sort, single scan and read retry (+2k6 code)
    #undef USE_VEML6070_SHOW_RAW                // VEML6070, shows the raw value of UV-A
    #undef USE_APDS9960_GESTURE                   // Enable APDS9960 Gesture feature (+2k code)
    #undef USE_APDS9960_PROXIMITY                 // Enable APDS9960 Proximity feature (>50 code)
    #undef USE_APDS9960_COLOR                     // Enable APDS9960 Color feature (+0.8k code)
  #undef USE_ADE7953                            // [I2cDriver7] Enable ADE7953 Energy monitor as used on Shelly 2.5 (I2C address 0x38) (+1k5)
    #undef USE_DISPLAY_MODES1TO5                // Enable display mode 1 to 5 in addition to mode 0
    #undef USE_DISPLAY_LCD                      // [DisplayModel 1] [I2cDriver3] Enable Lcd display (I2C addresses 0x27 and 0x3F) (+6k code)
    #undef USE_DISPLAY_MATRIX                   // [DisplayModel 3] [I2cDriver5] Enable 8x8 Matrix display (I2C adresseses see below) (+11k code)
    #undef USE_DISPLAY_SEVENSEG                 // [DisplayModel 11] [I2cDriver47] Enable sevenseg display (I2C 0x70-0x77) (<+11k code)
#undef USE_SERIAL_BRIDGE                        // Add support for software Serial Bridge (+0k8 code)
#undef USE_ENERGY_MARGIN_DETECTION              // Add support for Energy Margin detection (+1k6 code)
  #undef USE_ENERGY_POWER_LIMIT                 // Add additional support for Energy Power Limit detection (+1k2 code)
#undef USE_PZEM004T                             // Add support for PZEM004T Energy monitor (+2k code)
#undef USE_PZEM_AC                              // Add support for PZEM014,016 Energy monitor (+1k1 code)
#undef USE_PZEM_DC                              // Add support for PZEM003,017 Energy monitor (+1k1 code)
#undef USE_MCP39F501                            // Add support for MCP39F501 Energy monitor as used in Shelly 2 (+3k1 code)
#undef USE_BL0940                               // Add support for BL0940 Energy monitor as used in Blitzwolf SHP-10 (+1k6 code)
#undef USE_DHT                                  // Add support for DHT11, AM2301 (DHT21, DHT22, AM2302, AM2321) and SI7021 Temperature and Humidity sensor (1k6 code)
#undef USE_IR_REMOTE                            // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k3 code, 0k3 mem, 48 iram)
  #undef USE_IR_RECEIVE                         // Support for IR receiver (+7k2 code, 264 iram)
  #undef USE_ZIGBEE_ZNP                         // Enable ZNP protocol, needed for CC2530 based devices
  #undef USE_ZBBRIDGE_TLS                       // TLS support for zbbridge

#endif  // _USER_CONFIG_OVERRIDE_H_
