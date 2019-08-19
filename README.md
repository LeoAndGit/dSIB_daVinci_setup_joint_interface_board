[![Release](https://img.shields.io/badge/Release-Ver1.1.0-blue.svg)](https://github.com/LeoAndGit/dSIB_daVinci_setup_joint_interface_board/releases)

dSIB_daVinci_setup_joint_interface_board
===========================
This preject is to design a control system for da Vinci Surgical setup joint. It has three parts: a interface board, Arduino code and a python program to test it.

****
Author: Leo

Commercial use is not allowed, if not consulted me.
****

Interface board
---------------------------
This board is a four layer PCB to connect da Vinci to your PC. It is easy to use IDC connector on the board to deal with so many pins on da Vinci. I choose two GST90A48-P1M power supplies(48V 1.87A) to provide enough energy for 6 brakes inside da Vinci.

Use this board is really simple. Built in CH340 USB to serial converter makes it possible to program your STM controller without any other device. AD7173 also has wonderful performance and is easy to design the circuit. It has a 2.5V internal reference so the maximum input is 2.5V.

To uplode new program, just change the switch on left side to BOOT mode and use serial in Arduino IDE.

***************************

BUGs on daVinciBoard-01:
- Change C44 into a 0ohm resister. Connect AIN16 pin to GND.
- Remove R6 R7 for the pull up voltage is too high. I enable the internal buffer and this buffer cannot support a high voltage input.

***************************

- PCB 
<img src="/image/PCB.png" width="350px" />

- Photo of board
<img src="/image/board_img.jpg" width="350px" />

- Photo of board and da Vinci
<img src="/image/board_img2.jpg" width="350px" />

Arduino code
---------------------------
About the whole control system, you can get some information on my another project: https://github.com/LeoAndGit/Arduino_Simple_AT_Command This project uses the same architecture to communicate and control.

Arduino STM32 library: https://github.com/rogerclarkmelbourne/Arduino_STM32

You will still need this AD7173 library modified by myself: https://github.com/LeoAndGit/AD7173-Arduino/tree/new-feature

***************************

V1.10 code feature:
- Support control every independent brake
- Support 12 channels of analog reading
- Support multiple boards working together by different addresses

A red LED shows 3.3V logic power is online and a green LED tells you this MCU is working now. If you see two green LEDs, that means there are at least one brake released.

How to control this board: Send AT command to it with baud of 115200. 

- **Send**: AT\r\n  
  **Reply**: OK\r\n  
  **Description**: for testing  



 - **Send**: AT+VERSION\r\n  
   **Reply:** VERSION\r\n OK\r\n  
   **Description**: get version


 - **Send**: AT+ADDRESS\r\n  
   **Reply**: ADDRESS\r\n OK\r\n  
   **Description**: get address 



- **Send**: AT+FREEALL\r\n  
  **Reply**: OK\r\n  
  **Description**: release all brakes



- **Send**: AT+LOCKALL\r\n  
  **Reply**: OK\r\n  
  **Description**: lock all brakes



- **Send**: AT+FREE=X\r\n  
  **Reply**: OK\r\n  
  **Description**: release joint X. X should be 1, 2, 3, 4, 5, 6.



- **Send**: AT+READALL\r\n  
  **Reply**: DATA + OK\r\n  
  **Description**: read the value of every channel and send them back to PC. DATA part will be DDDDDD0X\r\n. DDDDDD is the 24bit AD data(you should turn string into HEX when you receive these data) and 0X is STATE. X shows the channel number. If error happens in ADC, 0 in DATA will become other value like 4. DDDDDD0X\r\n will repeat 12 times to send all channels and the first channel is not fixed.

**BUG**: Because reading process starts randomly, it is possible to skip first channel. This can be solved on STM or just adding a verification procedure in your program to check if you get all the channel data.

Testing program
---------------------------
- Click link then it will search for COM and auto connect to first one.
- Click start to update data every 0.1s.

***************************

- Photo of GUI 
<img src="/image/GUI.png" width="500px" />

