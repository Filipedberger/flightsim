# Project Plan for Third Person Flight Simulator

## 1. Project Overview
Create a third person flight simulator where the player can fly over a large terrain. The game will feature shaders, a skybox or skydome, collision detection with ground and sky obstacles, and light sources. The player will need to avoid obstacles and, if time permits, participate in a time-based point race.

## 2. Project Participants
- David Norman, davno223@student.liu.se
- Victor Eriksson, vicer124@student.liu.se
- Filip Edberger, filed881@student.liu.se

## 3. Project structure
The game will bi implemented with a object oriented structure and will run with a main loop and based on different states such as start state, different gamemode states, end state. The main loop will also handle the creation and curation of the window. 

Each state represents one part of the game and has three main components, namely handle, update and display. In each state, the game logic is represented and events that happen are handled for each object, such as movement or collosion. 


## 4. Project steps
### 4.1 Main loop and terrain
As a first step the main loop and the creation and drawing of the terrain will be implemented. This includes 
* Creating a start state
* Creating the handle, update and display functions 

* Creating a class for the ground 
* Creating the handle, update and display functions 
* Lighting of the ground (part of the display function)

* Creation of a skydome class 
* Creating the handle, update and display functions 
* Lighting of the skydome, (part of the display function)

* Display the start state

### 4.2 Add an airplane and implement movement controlls
This step includes:
* Creating a class for the airplane
* Creating the handle, update and display functions 
* Lighting of the airplane, (part of the display function)




# 5. Task delegation
## Projekt step 4.1
We will start this project by experimenting with potetial solutions for the first steps. This will be done in the code we have created durint the course labs. When we have found solutions to the first steps, we will merge it together into the intended structure for the project, the object oriented state based structure.

Filip will start experimenting with the main loop and the states.
Virro will start experimenting with the terrain class and the funcitons within.
David will start experimenting with the skydome and the light source.

