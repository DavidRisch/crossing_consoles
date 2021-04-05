\page message-structure Message Structure

| Content       | Escape Sequence (Flag) | Message Type | Message Sequence| Length of Payload | Payload | CRC | Escape Sequence (Flag)
| :------------- | :----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: | :-----------: |
| Content of Acknowledge Message     | \" | \" | \" | \"  | Acknowledged Message Sequence | \"  | \" 
| Size in Bytes | 1 | 1 | 2 | 0 or 2 | 0 - (2^16-1) | 4 | 1
