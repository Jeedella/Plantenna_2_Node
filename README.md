# Smart Plant Monitoring System Zephyr-based Node

Repository that contains the sources for the node of the Smart Plant Monitoring System (SPMS) Fontys project. The program runs on [Zephyr RTOS](https://zephyrproject.org/) (version 2.4.0) and has been prototyped using the [nRF52 DK](https://www.nordicsemi.com/Software-and-Tools/Development-Kits/nRF52-DK) (nRF52832 SoC).

The main goal of the project is to make a proof of concept for a bluetooth mesh network of plant monitoring nodes. The data of all the nodes is collected by a [gateway](https://github.com/Jeedella/Plantenna_2_Gateway) and pushed to a [cloud database](https://github.com/Jeedella/Plantenna_2_Cloud) for potential further processing.

## How to run

1. Setup a Zephyr development environment (version 2.4.0) following the Ubuntu [getting started guide](https://docs.zephyrproject.org/latest/getting_started/index.html) and make sure to replace the ``west init`` step with:
    ````
    west init ~/zephyrproject --mr zephyr-v2.4.0
    ````
    This makes sure the correct version of zephyr is installed. 

    It is recommended to make use of an Ubuntu based machine, since the required Toolchains in the Nordic SDK are not supported on Windows or macOS. One can also virtualize Ubuntu ([WSL](https://ubuntu.com/wsl) or more preferably [Virtual box](https://www.virtualbox.org/)) to circumvent this issue. A short guide to setup Virtual box can be found [here](#Brief-Ubuntu-VM-setup-using-virtual-box).

    > The "west flash" command will not work in WSL due it not having USB passthrough. [Here](#-Workaround-for-programming-jlink-devices-in-WSL) is a workaround.

2. Change directory to your zephyr folder and clone this repository:

    ```text
    cd ~/zephyrproject/zephyr
    git clone https://github.com/Jeedella/Plantenna_2_Node.git
    ```

    > When wanting to build the program outside of the zephyr environment do not forget to set its base path in .bashrc:
    >
    > ```text
    > echo 'export ZEPHYR_BASE=~/zephyrproject/zephyr' >> ~/.bashrc
    > ```

3. If everything up to this point has been setup correctly, the program can be build and uploaded just like the zephyr samples. Since three bluetooth variants of the application exists (ble, mesh_server and mesh_client), the bluetooth type has to be specified first.

    ```text
    export SPMS_BT=<bluetooth_type>
    west build -p auto -b nrf52dk_nrf52832 Plantenna_2_Node/
    west flash
    ```

    > When using a different board, replace the nrf52dk_nrf52832 parameter with your own.

    **Notes:**

    * It is also possible to upload the program manually by drag and dropping the generated .hex file located in the `build/zephyr/` folder.
    * When using [WSL](https://ubuntu.com/wsl), Ubuntu files can be found in the folder `\\wsl$\Ubuntu`.

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
2. Create a new VM having with disk size of atleast 20GB.
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
