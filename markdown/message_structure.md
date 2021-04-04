\page message-structure Message Structure

| Content       | Escape Sequence (Flag) | Message Type | Message Sequence| Length of Payload | Payload | CRC | Escape Sequence (Flag)
| :------------- | :----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: |
| Content of Acknowledge Message     | see above|see above |see above |see above  | Acknowledged Message Sequence |see above  | see above
| Size in Bytes | 4 | 1 | 2 | 2 or 0 | 0 - (2^16-1) | 8 | 4

Current implementation of message structure in the
[Communication Protocol](https://github.com/DavidRisch/crossing_consoles/blob/master/src/communication/ProtocolDefinition.h)
.