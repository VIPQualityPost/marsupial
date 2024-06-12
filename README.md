[![Hardware Design](https://github.com/VIPQualityPost/marsupial/actions/workflows/design.yml/badge.svg?branch=main)](https://github.com/VIPQualityPost/marsupial/actions/workflows/design.yml)
[![Assembly Documentation](https://github.com/VIPQualityPost/marsupial/actions/workflows/documentation.yml/badge.svg)](https://github.com/VIPQualityPost/marsupial/actions/workflows/documentation.yml)

# MARSUPIAL USB-PD + data hub
Marsupial (Motor And Regular Stuff USB PD Injector and Link) is a two port USB FS/HS hub which also is capable of injecting up to 60W per port (or 100W per port if I can implement USB PD EPR messages). The intended use for this is to be able to power and control motors over a single wire to both the motor and the PC, although it is fully USB PD compliant so you would be able to plug any other type of device into it. At the heart of the design is a programmable buck converter circuit which has a few predefined levels. In the future I would like to be able to implement Programmable Power Supply (PPS) functionality but at the moment this is out of scope of the project. It requires a large external 24V 120W power supply in order to power each port, and also features a daisy-chain connector on the back so that if you have multiple boards and a more powerful supply, you do not need to wire them all separately. The USB lines are brought out separately on an FFC connector so that the hub may be omitted and the signals manually handled off-board if desired.

![Render of PCB](/render.png)