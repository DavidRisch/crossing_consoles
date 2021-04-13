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
* `SPACE` Use current item
* `ESC` Close the game

Press `Y` to show every player's score, health, package loss and ping in-game.

Press `X` to show detailed statistics for your connection.

### Items

Currently, there are four types of items:

* Gun `¬` : Long range item which spawns projectiles that cause damage.
* Sword `Ϯ` : Short range item which causes damage directly.
* Heart `♥` : Increases the player's health by one when picked up.
* Points `10` : Increases the player's score by a certain amount.

'Hearts' and 'Points' get used immediately when picked up, 'Guns' and 'Swords' can be carried around by a player. If a
player picks up a new weapon in the world, their old weapon gets discarded.

### Scoring

A player can increase their score either by causing damage to another player using the 'Gun' or the 'Sword' item or by
collecting the 'Points' item in the world. For the 'Gun', the player's score is only increased if they shoot another
player, not themself. The player's score is displayed on the top left of the game viewport.

### Health

A player is dead if their health reaches zero. If a player is hit by a projectile from a 'Gun' or hit by a 'Sword',
their health is decreased by an amount specified by the weapon. The health bar in the top right shows the remaining
lives in form of hearts ♥ , every player starts with 8 lives.
