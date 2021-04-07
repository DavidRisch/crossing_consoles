\page game-world-rules Gameplay Rules  
Collection of decisions and conventions implemented in the game.

# Gameplay

## Rules

### Movement

* Players can move in four directions (Left, Right, Up, Down)
* Players can not walk through walls.
* Players can not walk through other players.
* Players are spawned randomly in the world.
* Dead players can not invoke any changes to the world.
* Projectiles pass through dead players.
* Players can not walk through dead players.
* The direction can be changed by "moving" in one of the four directions. *(not implemented yet!)*

### Health

* If a player's health is zero, the player is dead.
* Dead players will be respawned after a certain amount of time *(not implemented yet!)*

* A player's health has a fixed upper limit (e.g. 100).

### Score

* Points can be obtained by hitting **other** players.
* Maximal score limit is 2^16 points.

### Fight

* The player attacks into the direction they are currently facing.
* The inflicted damage depends on the player's weapon.
* If projectiles collide, they are removed from the world.

### World's Coordinate System

The world's coordinate system is orientated as follows:

<img src="coordinate_system.png" align="left"/><br/>

* x-axis to the right
* y-axis to the bottom 
