\page game-player-controls Player   
Game control and Player information.

## Player

Each player can choose their displayed color in the console and set a name at the start of the game.

### Controls

The player's movement is controlled by the following keys:

* `W` Move player 1 field up
* `A` Move player 1 field to the left
* `S` Move player 1 field down
* `D` Move player 1 field to the right
* `SPACE` Use current item *(standard item: weapon)*
* `ESC` Closes the game

Press `Y` to show every player's score, health, package loss and ping in-game.

### Scoring

If a player is hit by a projectile which was not spawned by their own weapon, the player's score increases.  
The player's score is displayed on the top left of the game viewport.

### Health

A player is dead if their health reaches zero.  
If a player is hit by a projectile, their health is decreased by an amount specified by the weapon that spawned the
projectile.

The health bar in the top right shows the remaining lives in form of hearts ♥ , every player starts with 8 lives.
