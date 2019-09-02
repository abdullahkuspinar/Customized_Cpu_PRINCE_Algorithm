# Customized-Cpu

For a given C code, a customized processor is synthesized. FELICS, 
Fair Evaluation of Lightweight Cryptographic Systems, PRINCE algorithm version 01 is chosen for this purpose. 
Hence the definition and the description of the PRINCE algorithm is due. 
PRINCE algorithm is the first algorithm of a kind that exists as a lightweight block cipherthat was intentionally developed
to be a minimizer of latency of a sort.  PRINCE algorithm is tried with ARM, MSP and, AVR before. This project’s aim is to show that 
PRINCE algorithm can be worked with a microcontroller which is customized for this work. 
To prove this point, PRINCE algorithm made applicable to VSCPU, which is designed for this project. 
After implementation part, by looking at the test results, the comparison is made between VSCPU and customized VSCPU
in terms of the number of instructions, slice registers, slice LUTs. Additionally, the platforms will be compared by the numbers of 
instructions.

Used Languages: C / Assembly Language / Verilog / Python
Used IDEs: Xilinx ISE Design Suite 14.7 / Sublime Text Editor 3
Used Devices: Xilinx FPGA Boards
Used Operation System: Linux Elementary
