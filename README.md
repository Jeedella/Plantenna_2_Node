# Project Plant Sensing System - nRF52-DK nodes
This README shows how to program the nRF52-DK using Segger Embedded Studdio or with a prewritten .hex-file.\
In addition, the current status of this project and the to-do list are added below.

## Program the nRF52-DK using Segger Embedded Studio
For the nRF52-DK, Segger Embedded Studio can be used to program it. For later developed nRF-DKs, something like Zephyr RTOS must be used to program it. Here, we will only look at Segger Embedded Studio.\
\
**Step 1. Install Segger Embedded Studio**\
Segger Embedded Studio can be installed here: https://www.segger.com/downloads/embedded-studio

**Step 2. Connecting to the J-Link**\
Create a new project or open an existing. Now, connect the nRF52-DK with a micro usb to usb cable to your laptop. In the menu above, click on "Target", then "Connect J-Link". On the bottom left a message should appear that says something simelar to: "J-Link connected". Also, in the menu above, at "Target" you should now see that you can disconnect from the J-Link (second option).

**Step 3. Run or debug the program**\
In the menu above, click "Build", then "Build and Run" for running to project. For debugging, click "Build and Debug".

## Program the nRF52-DK with a prewritten .hex-file
**Step 1. Connecting the nRF52-DK to your laptop**\
The nRF52-DK can be easily connected to your laptop/PC with a micro usb to usb cable. When you turn on the nRF52-DK, you should see a new drive (named J-Link) appear in your file explorer.

**Step 2. Program the nRF52-DK**\
Now, the nRF52-DK can be easily programmed by dragging and dropping a prewritten .hex-file on the J-Link drive.

## Current status of the project
**bluez meshctl with nRF52-DK nodes:**
- Looking at the Gateway repo, a demo was made with bluez meshctl on the Raspberry Pi 4 and 2 nRF52-DKs running Zephyr.hex. The Zephyr.hex and an additional Zephyr.elf are located in this repo in "zephyr_firmware.zip".

\
**Sensors:**
- <update needed>

\
**mesh network with only nRF52-DKs:**
- <update needed>

## To-do list
**General:**
- Update repo and README regarding Sensors and mesh network.

\
**bluez meshctl with nRF52-DK nodes:**
- Research into using Zephyr RTOS for creating BLE mesh nodes on the nRF52-DKs.

\
**Sensors:**
- <update needed>

\
**mesh network with only nRF52-DKs:**
- <update needed>
