# OrbitSim
Authors: ```Paramvir Lobana```, ```Jian Jiao``` \
Program written for the course: ```Space Propulsion``` ```Winter 2024``` ```Concordia University, Montreal```

## Description
Physics Based Orbital Mechanics Engine.The objective of this project is to program and simple self contained orbit simulation software for simulating the motion of satellites. The program is based on C++ language with SFML library to produce a user friendly interface.

The software enables user to manually control the orbit of the satellite by manually initiate prograde and retrograde burns or initiate Hohmann transfer between circular orbits. The software provide instant feedback on the radius and velocity of the satellite and it's orbit condition through the vis-viva equation.

This software is based on simple two body assumption and uses equations covered in the lectures; at the core, the software is an physics based real time simulation. The position of the velocity of the satellite is governed by kinematics and is calculated after each frame. This allows the trajectory of the satellite to be altered by user input instantaneously. However, this method also introduces errors during the calculation and will be discussed in detail in the error sections.


The program executable can be downloaded from:
[OrbitSim](src/x64/Release/)

Complete report can be found here: [Complete Report](report/OrbitSim.pdf)

## Usage
When the program is called, the following instructions are presented in a separate console window to help the user.
```bash
======================================================================================
INSTRUCTIONS
--------------------------------------------------------------------------------------
Mode 1
Up arrow key to increase the orbit radius.
Down arrow key to decrease the orbit radius.
Press T to initiate orbit transfer in Mode 1
--------------------------------------------------------------------------------------
Mode 2
Left arrow key
Right arrow key
Press the spacebar key to normalize the orbit in Model 2
--------------------------------------------------------------------------------------
Ctrl + scroll wheel for zooming in and zooming out.
======================================================================================
```

The GUI interface is presented below:

![Alt text](report/figures/transfer_state.png?raw=true "GUI")