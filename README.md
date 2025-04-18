# abcc-example-stm32nucleo
This is a stand-alone example application using the Anybus CompactCom Driver API ([abcc-api](https://github.com/hms-networks/abcc-driver-api)) ported for the [Anybus CompactCom Adapter Board - Module to STM32 Nucleo™](https://www.hms-networks.com/p/024620-b-anybus-compactcom-adapter-board-module-to-stm32-nucleo) evaluation platform.
### Purpose
To enable easy evaluation and inspiration to [Anybus CompactCom](https://www.hms-networks.com/embedded-network-interfaces) prospects.

## Prerequisites
### Integrated Development Environment (IDE)
- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) is needed to run this example application.
### STM32 Nucleo H753ZI
- This example application is created for the [STM32 Nucleo H753ZI](https://www.st.com/en/evaluation-tools/nucleo-h753zi.html) board together with the [adapter board](https://www.hms-networks.com/p/024620-b-anybus-compactcom-adapter-board-module-to-stm32-nucleo) from HMS.
### Git
- Of course, you will need to have Git installed. See [this tutorial](https://github.com/git-guides/install-git) on how to install Git.

## Cloning
### Flag? What flag?
This repository contain submodules [abcc-driver-api](https://github.com/hms-networks/abcc-api), [abcc-driver](https://github.com/hms-networks/abcc-driver) and [abcc-abp](https://github.com/hms-networks/abcc-abp) that must be initialized. Therefore, pass the flag `--recurse-submodules` when cloning.

```
git clone --recurse-submodules https://github.com/hms-networks/abcc-example-starterkit.git
```
#### (In case you missed it...)
If you did not pass the flag `--recurse-submodules` when cloning, the following command can be run:
```
git submodule update --init --recursive
```

## Build and run
### Step 1. Setup the hardware
Follow the steps in the chapter called *Confuguration* in the [Anybus® CompactCom™ Adapter Board for STM32 Nucleo™ - Installation Guide](https://hmsnetworks.blob.core.windows.net/nlw/docs/default-source/products/anybus/manuals-and-guides---manuals/hms-scm-1202-236.pdf?sfvrsn=90594caf_4) to setup your hardware correctly.
### Step 2. Open the project
Find the **.project** file in the repository root and open it with STM32CubeIDE.
### Step 3. Build and run
Select *Run*->*Debug* to build and run the configuration called "Debug". Use the function keys to step through or run the debug application.
### Step 4. See debug print-out
Open *Window*->*Show view*->*SWV*->*SWV ITM Data Console*. From there, click on the wrench icon to open the Serial Wire Viewer settings and enable the 0th ITM Stimulus port (rightmost checkbox). Then, press the red circle "Start Trace" to view the debug print-out from the example application.
#### (Nothing is happening...)
Make sure that your hardware is set up correctly and that the switch (S4) on the adapter board is configured to either SPI or UART operation mode. See the [Anybus® CompactCom™ Adapter Board for STM32 Nucleo™ - Installation Guide](https://hmsnetworks.blob.core.windows.net/nlw/docs/default-source/products/anybus/manuals-and-guides---manuals/hms-scm-1202-236.pdf?sfvrsn=90594caf_4) for more details.

