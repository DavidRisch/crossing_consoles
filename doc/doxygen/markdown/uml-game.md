\page uml-game Application Layer (Game)

The application layer contains the game **Crossing Consoles**.

The following UML Diagrams illustrate the connection between the used classes in each sub-directory of the `game`
namespace.  
For an overview of all connections between all classes of the namespace see \ref uml-src.

| Terminal |
| :---- |
| \image html src-game-terminal.png |

The `MockTerminal` is used for testing. Only the `RealTerminal` is used for the GamePlay.

| Networking |
| :---- |
| \image html src-game-networking.png width=100% |

Game information that is sent between Server and Client is de-/serialized and transferred using a `PayloadMessage` as
shown in \ref uml-messages.  
The `ChangeType` indicates which further action needs to take place on the Server or Client Side.

| Visual |
| :---- |
| \image html src-game-visual.png width=100% |

The output of the terminal is generated using the classes of the `visual` directory.

| Common |
| :---- |
| \image html src-game-common.png |

`Common` holds classes that are used by multiple `game` classes and describe common attributes of the world or of
elements in the world.

| World |
| :---- |
| \image html src-game-world.png width=100% |






