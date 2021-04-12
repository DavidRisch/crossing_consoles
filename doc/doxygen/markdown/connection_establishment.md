\page connection-establishment Connection Establishment

\image html handshake.png

First, the client sends a ConnectionRequestMessage to the server to initiate the handshake and changes into the state
CLIENT_CONNECTION_REQUEST_SENT.  
The server replies with a ConnectionResponseMessage and changes into the state SERVER_CONNECTION_RESPONSE_SENT.  
The client responds with an AcknowledgeMessage and changes into the state READY.   
After receiving the Acknowledge of the client, the server changes into the state READY as well.

