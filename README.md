# demo-Smart-Pressure-detector

Basic Information:

A Demo of Qualcomm MDM9206,for the promotion of Qualcomm technology.

Maintainer: Kou Zhiwu,JiangYuan,Wang Jie,Yan Shaohui,YangRong.

Start date: January 21st, 2019

Function Description:

The pressure value is detected using the fsr402 pressure sensor, and the LED lights up when the threshold is outside the set range.

Document Introduction:

===> Gizwits: it mainly include Gizwits cloud protocol related files.

===> Utils: It mainly includes some common data processing algorithms.

===> demo-Smart-Motion-detector-picture: include some pictures about this demo.

===> driver: mainly include some driver code, about gpio, adxl345, spi etc.

===> gagent: file containing the Gizwits Cloud Device Connection Protocol Library.

===> main: code start runing from this directory.

===> objs: Store some link files.

===> sdk: Store head files.

===> target: Store image file.

===> demo-Smart-Motion-detector.doc: detailed description of the demo;

===> llvm_build.bat: Script to compile source code.

Usage Instructions:

1. Downloading code from github according to the repository in “https://github.com/ThunderSoft-XA/demo-Smart-Pressure-detector” sheet.
2. Compile the code and flash the image to Gokit4 development kit.
3. Connect the Fsr402 sensor to the ADC interface of the Gokit development board.
4. Connect one pin of the led to the D9 pin of the development board, and the other pin is connected to the vcc.
5. USB data cable to connect PC and Gokit development board.
6. Open the serial debugging assistant.
7. Put your finger on the pressure receiving surface of the sensor, you can see that the data has changed. 
8. When pressure values change beyond the set threshold,the LED will be lit.


