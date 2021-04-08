\page uml-messages Message Layer

The following UML Diagrams illustrate the connection between the used classes in each sub-directory.  
For an overview of all connections between all classes see \ref uml-src.

| Messages |
| :---- |
| \image html src-communication-message_layer-message.png width=100% |

All types of messages inherit from the abstract base class `Message`.

| Messages Stream |
| :---- |
| \image html src-communication-message_layer-message_stream.png width=100% |

A CRC Checksum is calculated by the `CRCHandler` (see below) and checked after each de- and encoding of
the `MessageCoder` in order to detect invalid messages.  
If the checksum, calculated from the received message, does not match the checksum included in the sent message, a
`CRCIncorrectException` is thrown.  
The relation between the `CRC Handler` and `MessageCoder` is displayed in full detail here: \ref uml-src .

| CRC Handler |
| :---- |
| \image html src-communication-message_layer-crc.png |
