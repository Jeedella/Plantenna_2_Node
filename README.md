# Smart Plant Monitoring System Zephyr-based Node

Repository that contains the sources for the node of the Smart Plant Monitoring System (SPMS) Fontys project. The program runs on [Zephyr RTOS](https://zephyrproject.org/) (version 2.7.99) and has been prototyped using the [nRF52 DK](https://www.nordicsemi.com/Software-and-Tools/Development-Kits/nRF52-DK) (nRF52832 SoC).

The main goal of the project is to make a proof of concept for a bluetooth mesh network of plant monitoring nodes. The data of all the nodes is collected by a [gateway](https://github.com/Jeedella/Plantenna_2_Gateway) and pushed to a [cloud database](https://github.com/Jeedella/Plantenna_2_Cloud) for potential further processing.

## How to run
To setup Zephyr development environment (version 2.7.99) their own guide can be followed [getting started guide](https://docs.zephyrproject.org/latest/getting_started/index.html). The guide is complete but the final configurations are a bit vague. In the list below all the points mention in the starting guide are given with a bit more context:
1. First follow the Zephyr installation guide to the point of [Install a Toolchain](https://docs.zephyrproject.org/latest/getting_started/index.html#:~:text=scripts%5Crequirements.txt-,Install%20a%20Toolchain,-A%20toolchain%20provides). Some notes of what is install
   > Chocolatey is a software installer for packages as is used often in Linux based machines, but for Windows.

2.  For this project the [GNU Arm Embedded toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) was chosen as this is a very popular and well maintained compiler. This might not be the optimal and you are free to chose any other. If it is supported by Zephyr it will work and will follow the same process.
    - Please install the toolchain in an easy to reach path (e.g C:\GNU) as this will make you life much easier later on.
3. Now that the compiler is installed west needs to be able to find and use it. This will be done by setting global variable in Windows that point to the correct folder. To do this the open cmd.exe and set the following commands:
    - ```setx ZEPHYR_TOOLCHAIN_VARIANT gnuarmemb```
    - ```setx GNUARMEMB_TOOLCHAIN_PATH <Your path to the toolchain>```
    - If you run into issues CMake is not finding the path try reinstalling CMake and in the worst case everything.
4.  The project can now be pulled from git using the following commands:
    ```text
    cd %HOMEPATH%/zephyrproject/zephyr
    git clone https://github.com/Jeedella/Plantenna_2_Node.git
    ```
    Git can directly be used or you can use [Github Desktop](https://desktop.github.com/) or [Sourcetree](https://www.sourcetreeapp.com/). These are Git GUI's and give a better visual understanding of all the changes and commits in a project.  
    > Be aware that west does not work outside of the zephyrproject folder without changing its directory. 
5. The project is installed and to verify that it works the blinky project can be build and flashed. This is done by:
    ``` cmd
    west build -p auto -b nrf52dk_nrf52832 /samples/basic/blinky
    west flash
    ```
    If this builds and flashes without any error's the setup until has worked. Now you can try it with the Plantenna code yourself, but you need to give the build extra variables to program either the server or the node. This is done by:
    ```
    west build -p auto -b nrf52dk_nrf52832 Plantenna_2_Node/ -- -D SPMS_BT=<mesh_server/mesh_node/ble>
    ```
    These are variables that are passed on to the CMake list to switch between the different code bases. 
    > You can set a default board with : ``` west config build.board nrf52dk_nrf52832```
6. If at any point CMake can no longer find your GNU Toolchain try resetting the global variables or reinstall Zephyr again.
7. 
## Bluetooth Mesh sensor model
The SPMS node contains code for the sensor client model and sensor server model. The sensor setup server model has not yet been implemented. The status regarding the sensor model is shown below:

| Message | Opcode | Send by | Status |
| :-----: | :----: | :-----: | :----: |
| Sensor Descriptor Get | 0x82 0x30 | Client | |
| Sensor Descriptor Status | 0x51 | Server | |
| Sensor Get | 0x82 0x31 | Client | Implemented, both with and without property ID present |
| Sensor Status | 0x52 | Server | Implemented, both one specific sensor or all sensors |
| Sensor Column Get | 0x82 0x32 | Client | Not yet implemented |
| Sensor Column Status | 0x53 | Server | Not yet implemented |
| Sensor Series Get | 0x82 0x33 | Client | Not yet implemented |
| Sensor Series Status | 0x54 | Server | Not yet implemented |

## Bluetooth low energy profile

The SPMS node its Bluetooth Low Energy (BLE) profile is based on the portable profiles used by the [myAir devices](https://hackmd.io/@sookah/myAir) and thus follows the same structure. A different profile ID has been added: `0x07`, since the SPMS node uses an airflow sensor instead.

### UUID's

| Type | UUID | Profile ID | Description |
| :----: | :----: | :----: | :----: |
| Device | 1A310001-63B2-0795-204F-1DDA0100D29D | 0x00 | Portable Airflow sensor |
| Service | 1A31FF01-63B2-0795-204F-1DDA0100D29D | 0xFF | Available tasks |
| Characteristic | 1A31FF02-63B2-0795-204F-1DDA0100D29D | 0xFF | Available tasks |
| Service | 1A310701-63B2-0795-204F-1DDA0100D29D | 0x07 | Portable Airflow sensor |
| Characteristic | 1A310702-63B2-0795-204F-1DDA0100D29D | 0x07 | Portable Airflow sensor |

### Broadcasting

| Data | Element | Size | Type | Data Type | Factor |
| :----: | :----: | :----: | :----: | :----: | :----: |
| Manufacturer specific size (0x1D) | 0 | 1 | Company | uint8 | - |
| AD type manufacturer specific (0xFF) | 1 | 1 | Company | uint8 | - |
| Company ID (tbd, 0xFF) | 2-3 | 2 | Company | uint16 | - |
| Temperature (°C) | 4-5 | 2 | MyAir | int16 | x100 |
| Humidity (%RH) | 6-7 | 2 | MyAir | int16 | x100 |
| Pressure (hPa) | 8-9 | 2 | MyAir | int16 | - |
| Battery voltage (mV) | 10 | 1 | MyAir | uint8 | /20 |
| Status register | 11 | 1 | MyAir | uint8 | - |
| Airflow (mm/s) | 12-13 | 2 | SPMS | uint16 | - |

## Brief Ubuntu VM setup using virtual box

The brief steps that need to be taken in order to setup an Ubuntu VM for setting up the zephyr build environment and programming the nRF can be found below. It has been tested to work with Virtual box version 6.1.18, Ubuntu server version 20.04.1 and the nRF command-line-tools version 10.12.1.
1. Install [Virtual box](https://www.virtualbox.org/) and download the [Ubuntu server ISO](https://ubuntu.com/download/server).
2. Create a new VM having with disk size of around 20GB.
3. Change the network adaptor settings of the VM to bridged to allow for ssh access later on.
4. Insert the Ubuntu ISO in the VM's optical drive and start the VM.
5. Follow the Ubuntu installation leaving all options on default. When asked for installing openSSH, check the box to install it.
6. Close the VM and eject the Ubuntu ISO, as it is now installed, and start the VM again. Make sure there are no errors during startup.
7. After logging in, update Ubuntu using:
    ````
    sudo apt update && sudo apt upgrade
    ````
8. Install net-tools and libncurses5 since these are required for the next steps:
    ````
    sudo apt install net-tools libncurses5 srecord
    ````
9. Check the VM's ip address using
    ````
    ifconfig
    ````
    and login to the VM using an ssh client on the host machine (for example [putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)) to verify that up to this point all has been configured correctly.
10. Download the linux 64 version of the [nRF command line tools](https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRF-Command-Line-Tools/Download#infotabs) on the host machine and copy it to the VM using an scp client (for example [winscp](https://winscp.net/eng/download.php)).
11. Extract and install the command line tools with:
    ````
    tar -xvf nRF-Command-Line-Tools_<verion>_Linux-amd64.tar
    sudo dpkg --install JLink_Linux_<version>_x86_64.deb
    sudo dpkg --install nRF-Command-Line-Tools_<version>_Linux-amd64.deb
    ````
12. Verify the installing with:
    ````
    nrfjprog --version
    ````
    And remove the downloaded files:
    ````
    rm -r *
    ````
13. Forward the Jlink USB device to the virtual machine. This can be done via the toolbar under Devices->USB->SEGGER Jlink. To verify this run:
    ````
    lsusb
    ````
    If the jlink device is show, then the VM has been setup correctly and the [how-to-run](#how-to-run) can be resumed.

## Workaround for programming jlink devices in WSL

Alt_flash is an script for "programming" jlink devices. This is possible since WSL allows mounting windows drives and Jlink devices just so happens to be seen as one. However this method has proven to be very inconsistent, and it is recomended to setup a virtual machine using [Virtual box](https://www.virtualbox.org/) instead. 

Before trying to set it up make sure the [how-to-run](#how-to-run) steps have been completed, except flashing the board of course.

### How to setup

```text
cd ~/zephyrproject/zephyr
sudo cp Plantenna_2_Node/alt_flash_tool/alt_flash /usr/bin/
source ~/.bashrc
```

If all has been successfully run, the board can now be programmed by running alt_flash and giving it the drive letter of the jlink device. For example when the jlink device mounted to (G:), it can be programmed as:

```text
alt_flash G
```
