\page game-client-server Communication of Game Server and Game Clients

The GameServer and connected Game Client use `PayloadMessage`s to exchange information about the Game.

All Game Logic is managed on the GameServer.  
The GameClient only sends information about the player's keyboard input and meta information like the player's id while
the GameServer processes the client's input and applies the provided `GameLogic` as seen in \ref uml-game.

The type of the requested change to the world by the server or the client is marked using `ChangeType`s which
defintiions are listed below.

| ChangeType | Sender | Definition
| :------------- | :---------- | :---------- |
| SET_NAME | Client| Set the player's name|
| SET_COLOR | Client | Set the player's color |
| MOVE_UP <br> MOVE_DOWN <br> MOVE_RIGHT <br> MOVE_LEFT | Client | Move the player one field towards the indicated direction|
| USE_ITEM | Client | Use the item currently held by the player|
| SET_OWN_ID | Server | Assign an ID to the player|
| SET_WORLD | Server | An updated version of the world can be set in the client's viewport|
| UPDATE_WORLD | Server | An updated version of the world (static elements excluded) can be set in the client's viewport|
