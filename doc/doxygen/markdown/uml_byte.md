\page uml-byte Byte Layer

The Byte Layer provides a ByteStream and a ConnectionSimulator, which is used to send and receive bytes.

The following UML Diagrams illustrate the connection between the used classes in each sub-directory.  
For an overview of all connections between all classes see \ref uml-src.

| Connection Simulator |
| :---- |
| \image html src-communication-byte_layer-connection_simulator.png width=100% |

The class `ConnectionSimulatorFlaky` is only used for testing.

| Byte Stream |
| :---- |
| \image html src-communication-byte_layer-byte_stream.png  width=100% |

The classes `MockInputStream` and `MockBidirectionalStream` only exist for testing purposes.
