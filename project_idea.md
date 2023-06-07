# Project Interior Design

A game where players can can create and design interior spaces that are shared with other players.

# Gameplay

### Goals

- Simplicity - Give the player tools where they would expect them. Ensure hot navigation paths are short
- Charming - Use animation, art, and sound design to make the interface feel warm

The gameplay can be split up into 2 distinct modes

- Progression Management
- Room Building

## Room Building

Room building revolves around a simple loop

1. Create a new `Room`
    - This requires a room name. This will be used for saving the room until it is submitted
2. Edit the `Room`
    - Most of the game will be spent building and designing rooms.
    - The tools used to build rooms will be similar to
      game engine tools such as placement, content browsers, and inspection panels.
3. Submit the `Room`
    - Requires more information including a description, title, and tags

### Wall Placement

Walls will be placed on a grid using splines. They will have many properties including what material is on them, and
what crown and base molding is used.

#### Windows

Windows will be carved out of placed walls. They can have their frame changed, window tint, and glass type. They can
also have curtains placed around them

### Decorations

Decorations will be placed on a grid based on the wall and floor. They can also be rotated.

#### Abstract Furniture

Instead of having categories in a simple content browser limit the player to a couple of abstract furniture types. Using
this the player layout a room without thinking about style whatsoever and then select what furniture they would like to
be in its place.

#### Example Types of Furniture

* Sofa
    * Loveseat
    * Divan
    * Sectional
* Bed
    * Full
    * Queen
    * King
* Chair
    * Dining
    * Armchair
    * Recliner

Each type of furniture will have its own ways of modification. An example being what material a sofa uses, whats the
frame around a bed, what comforter the bed is using, etc.