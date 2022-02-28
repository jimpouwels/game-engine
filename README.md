# Intro
This is my attempt to build a simple game engine. The game engine is equipped with:

- game-loop handling (incl. fps management and timing)
- drawing sprites
  - size
  - angle
  - scale
- graphics based on sprite groups
- shapes
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

The purpose of the update loop is to update the game's state at a pace that is as high as possible. The reason this does not happen on the rendering loop is to make the game engine frame-rate independent. Frame rate dependency can cause issues in many areas, e.g. in collision detection. When the frame rate drops, sprites will make bigger 'jumps' on the screen for each frame, therefore potentially skipping the thresholds for collision detection which leads to unexpected behavior in your game.

The rendering loop has only one task: Getting the state of the game, and display it on the screen.

The game engine manages these two processes and you don't have to intervene with them. All you have to do is act upon events from both of these loops (`onUpdate`, `onFrame`).

## Usage

### Create a Game
1. Extend from the the `GameEngine` class.
2. Invoke the `start()` function to make the gameloop start

At this point, the game will be in a running state. 

### Graphics
You can add sprites to the scene by creating a Graphic, which is done by creating a new class that extends from `jimp::Graphic`.

#### Registration
You can make a Graphic part of the game loop by registering it. This happens automatically when instantiating your class that extends `Graphic`. However, to prevent partially initialized graphics to be made part of the game loop, make sure you call `markAsInitialized()` at the end of your class' constructor.

**Note**
Don't register sprites or sounds with the game engine yourself. The `AnimatedSprite` class will do that for you.
In other words, **don't do this**:

```
SpaceShip* spaceship = new SpaceShip();
gameEngine->registerGraphic(spaceship);
```

#### Graphics
The model for a `Graphic` is as follows: 

- `Graphic`
  - `Animation[]`
    - `Sprite[]`

In essence, a Graphic is a complex sprite that represents a single character, object or shape on the screen that can be rendered in the form of different animations which each constists of a collection of sprites rendered in order. Example:

- Graphic: running cartoon
  - Animation1
    - name: "running left"
    - sprites: 5 sprites that represent running to the left
  - Animation2
    - name: "running right"
    - sprites: 5 sprites that represent running to the right

It's up to the game developer to switch between animations at the right time (as part of handling an event, such as `doOnUpdate()`). However, the game engine will handle switching between sprites within an animation. You have to tell the sprite how much time will be between each sprite within the animation, and the game engine will handle that.

Code example of a space ship:

```
SpaceShip::SpaceShip() : jimp::Graphic(jimp::Vector2D { .x = 400, .y = 400 }, SCALE, 0, 0.05F) {
    addSprite("idle", "spaceship.png");
    addSprite("throttling", "spaceship-thrust1.png");
    addSprite("throttling", "spaceship-thrust2.png");
    markAsInitialized();
}
```

In this example, we created a Graphic `SpaceShip`. It has two animations, being `idle` and `throttling`. The first animation you add a sprite for will be the active animation, in this case `idle`. 

**Do note that at the end of the constructor of a Graphic, it has to be marked as initialized by calling `markAsInitialized()`. This is important for the game engine to know that the Graphic is ready to be rendered to the screen.**

You can switch between `throttling` and `idle` like this:

```
void Ship::onKeyboardUp(jimp::KeyState keyState) {
    if (keyState == jimp::KeyState::PRESSED) {
        setCurrentAnimation("throttling");
    } else {
        setCurrentAnimation("default");
    }
}
```

When the active animation becomes `throttling`, the game engine will start switching between sprite `spaceship-thrust1.png` and `spaceship-thrust2.png` every 0.05 seconds (`spriteSwapIntervalInSeconds`). This value has to be passed to the `Graphic` constructor. Keep in mind that bringing the `spriteSwapIntervalInSeconds` value below the duration of a frame (dependent on fps), there will no longer be a noticable difference in swapping speed.

#### doOnUpdate and doOnFrame
For each class extending from Graphic, you have to implement the following functions:

- `doOnUpdate(float elapsedTime)`: Perform all the logic that is required to update the state of the Graphic (e.g. move the sprite across the screen or switch to a different animation).
- `doOnFrame(float elapsedTime)`: Perform all the logic that is required to render the Graphic. For simple cases, you don't have to do anything here.

#### Concurrency
Updating the state of an Graphic and rendering it takes place on two different threads. Keep the following in mind:

- A Graphic will never be rendered while it's in updating state. This means, that as long as the `doOnUpdate` function is running, that sprite will not get rendered until that function completes.
- A Graphic will never be updated while it's in rendering state. This means, that as long as the `doOnUpdate` function is running, that sprite will not get updated until that function completes.

### Sprite

### Timing
// Utilities
// Elapsed time

### Movement and accelaration

## Points of attention

### Separation of update and render
