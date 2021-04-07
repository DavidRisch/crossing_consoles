\page close-connection Connection Closing

# Connection Reset Message

\image html connection_reset.png

If one of the communication partners wants to close the connection, a ConnectionResetMessage is sent to the associated
connection partner and the state is changed to WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE.  
As soon as the ConnectionResetMessage is received, an AcknowledgeMessage is sent to the sender of the
ConnectionResetMessage and the connection state is changed to CLOSED.   
The partner in the state WAITING_FOR_CONNECTION_RESET_ACKNOWLEDGE changes to the state CLOSED after receiving the
AcknowledgeMessage. the first partner also changes into the state CLOSED.  
As soon as a communication partner changes its state to CLOSED, the connection is removed from their connection list.

# Timeout

\image html timeout.png

If a timeout is detected, the connection state is changed to CLOSED and removed from the list of connections, regardless
of the previous connection state.
