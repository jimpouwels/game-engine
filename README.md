# Intro
This is my attempt to build a simple game engine. The game engine is equipped with:

- game-loop handling (incl. fps management and timing)
- drawing sprites
  - size
  - angle
  - scale
- animated sprites based on sprite groups
- keyboard handling
- basic geo calculation utilities

## Install

- clone the code
- install XCode
- open the project in XCode
- run it!

## Background

In order to work with this game engine it's good to have a good understanding of the inner workings. 

This game engine consists of two main proccesses:

1. Update loop
2. Rendering loop

The purpose of the update loop is to update the gamestate at a pace that is as high as possible. The reason this does not happen on the rendering loop is to make the game engine frame-rate independent. Frame rate dependency can cause issues in many areas, e.g. in collision detection. When the frame rate drops, sprites will make bigger 'jumps' on the screen for each frame, therefore potentially skipping the thresholds for collision detection which leads to unexpected behavior in your game.

The rendering loop has only one task: Getting the state of the game, and display it on the screen.

The game engine manages these two processes and you don't have to intervene with them. All you have to do is act upon events from both of these loops.

## Usage

1. Extend from the the `GameEngine` class.
2. Invoke the `start()` function to make the gameloop start

At this point, the game will be in a running state. You can add sprites to the scene by creating one in one of the following ways:

1. Create a new instance of the `AnimatedSprite` class. It will automatically register itself with the gameloop and it will be rendered in the next frame.
2. Create a new class that extends the `AnimatedSprite` class.
