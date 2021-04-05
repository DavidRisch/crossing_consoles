\page connection-establishment Connection Establishment

\image html handshake.png

First, the client sends a ConnectionRequestMessage to the server to initiate the handshake and changes into the state CLIENT_CONNECTION_REQUEST_SENT.
The server replies with a ConnectionResponseMessage and changes into the state SERVER_CONNECTION_RESPONSE_SENT.
After receiving the message, the client sends an AcknowledgeMessage and changes into the state READY.
When the server receives the Acknowledge, it also changes into the state READY.

