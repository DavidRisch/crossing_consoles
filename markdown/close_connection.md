\page close-connection Connection Closing

# Connection Reset Message

\image html connection_reset.png

If one of the communication partners wants to close the connection, he sends a ConnectionResetMessage to the other and changes his state to WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE. 
The partner sends an AcknowledgeMessage to confirm that the connection will be closed and changes into the state CLOSED. 
After receiving the AcknowledgeMessage, the first partner also changes into the state CLOSED. 
No further messages can be sent or received on this connection and both partners remove it from their list of connections.

# Timeout

\image html timeout.png

If one partner detects a timeout on a connection, he changes its state to CLOSED and removes it from his list of connections.
