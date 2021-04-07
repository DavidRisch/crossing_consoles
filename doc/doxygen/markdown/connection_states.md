\page connection-states Connection States

\image html connection_states.png

If a message is sent, the state is changed from READY to WAITING_FOR_ACKNOWLEDGE.  
After ACKNOWLEDGEs have been received for all sent messages, the state is changed to READY again. 
