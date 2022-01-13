# Mbed-Game
## Overview
This is the final project for ECE 2035 at Georgia Tech for Fall 2021 semester. All development was done in the online Mbed compiler.
Project objective was to create a C++ RPG Game with graphics designed to run on mbed LPC1768 microcontroller using a µLCD-144-G2(GFX) screen, buttons and accelerometer for controls.

Apologies for the lack of images. I took apart the system for parts before I took photos of gameplay. I was able to find one photo of the system here:

<img src="https://user-images.githubusercontent.com/71471706/149411685-4dbfb309-9935-464b-acce-6c51370b1ac6.jpg" width="300" height="300">

## Primary Elements
These are a list of major functional elements comprising the game.
1. Hash Table - Hash Table with external chaining collision resolution strategy with proper memory management stores game elements while preventing memory leaks.
2. Graphics - Graphics uses a grid of 11x11 elements for games tiles and updates the screen during movement.
3. Speech - Managed by separate graphics code to supply variable length speech bubbles as well as user prompts for choices and turned based combat.
4. Hardware - The code allows for use of a piezo speaker, 4 buttons, the µLCD, and an accelerometer.

## Primary Features
These are the list of game-play feature that the code supports and exhibits
- Running: Holding down button will make the character move faster.
- Dynamic sprites: The character sprite can change depending on direction faced. Currently not implemented due to limitations on the µLCD provided by the class
- Turn Based Combat: The speech tools allow for choice making during a combat.
- Inventory: Inventory is displayed on the bottom of the screen and can be kept track of in the character struct.
- Multiple Maps: Maps can be created and moved between using portals.
- Movement: Movement is controlled through an accelerometer; the player moves in the direction you tilt the game.

## Final Notes
Basic shell code (A few method headers and constructors) and µLCD library was provided by the instructor. All functional portions of the code was written by me. I received a 135/100 grade on this project. Please reach out if you have any questions at jadon.gro@gatech.edu.
