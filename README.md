# Smart Plant Monitoring System Zephyr-based Node

Repository that contains the sources for the node of the Smart Plant Monitoring System (SPMS) Fontys project. The program runs on [Zephyr RTOS](https://zephyrproject.org/) and has been prototyped using the [nRF52 DK](https://www.nordicsemi.com/Software-and-Tools/Development-Kits/nRF52-DK) (nRF52832 SoC).

The main goal of the project is to make a proof of concept for a bluetooth mesh network of plant monitoring nodes. The data of all the nodes is collected by a [gateway](https://github.com/Jeedella/Plantenna_2_Gateway) and pushed to a [cloud database](https://github.com/Jeedella/Plantenna_2_Cloud) for potential further processing.

## How to run

1. Setup a Zephyr development environment by following the [getting started guide](https://docs.zephyrproject.org/latest/getting_started/index.html) and make sure the blinky sample builds without issues. It is recommended to make use of an Ubuntu based machine, since the required Toolchains in the Nordic SDK are not supported on Windows or macOS. One can also emulate Ubuntu ([WSL](https://ubuntu.com/wsl) or [Virtual Machine](https://www.virtualbox.org/)) to circumvent this issue.

    > The "west flash" command will not work in WSL due it not having USB passthrough. [Here](#-Workaround-for-programming-jlink-devices-in-WSL) is a workaround.

2. Change directory to your zephyr folder and clone this repository:

    ```text
    cd ~/zephyrproject/zephyr
    git clone https://github.com/Tijntj3/Plantenna_2_Node.git
    ```

    > When wanting to build the program outside of the zephyr environment do not forget to set its base path in .bashrc:
    >
    > ```text
    > echo 'export ZEPHYR_BASE=~/zephyrproject/zephyr' >> ~/.bashrc
    > ```

3. If everything up to this point has been setup correctly, the program can be build and uploaded just like the zephyr samples:

    ```text
    west build -p auto -b nrf52dk_nrf52832 Plantenna_2_Node/
    west flash
    ```

    > When using a different board, replace the nrf52dk_nrf52832 parameter with your own.

    **Notes:**

    * It is also possible to upload the program manually by drag and dropping the generated .hex file located in the `build/zephyr/` folder.

## Workaround for programming jlink devices in WSL

Alt_flash is an alternative tool/script for programming jlink devices. This is possible since WSL allows mounting windows drives and Jlink devices just so happens to be seen as one. Before trying to set it up make sure the [how-to-run](##-How-to-run) steps have been completed, except flashing the board of course.

### How to setup

```text
cd ~/zephyrproject/zephyr
sudo cp Plantenna_2_Node/alt_flash /usr/bin/
source ~/.bashrc
```

If all has been successfully, the board can now be programmed by running alt_flash and giving it the drive letter of the jlink device. For example when the jlink device mounted to (G:), it can be programmed as:

```text
alt_flash G
```

## Bluetooth low energy profile

The SPMS node its Bluetooth Low Energy (BLE) profile is based on the portable profiles used by the [myAir devices](https://hackmd.io/@sookah/myAir) and thus follows the same structure. A different profile ID has been added: `0x07`, since the SPMS node uses an airflow sensor instead.

### Advertising

| Service UUID | Profile ID | Description |
| :----: | :----: | :----: |
| 1A310701-63B2-0795-204F-1DDA0100D29D | 0x07 | Portable Airflow sensor |

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
| Airflow (mm/s) | 10 | 2 | SPMS | uint16 | - |
