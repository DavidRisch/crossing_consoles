\page network-model Network Model

| Layer  | Functionality
| :-------------: | :---------- |
| Connection Layer | <ul><li>Communication Statistics</li><li> Provide TCP Services:<ul><li>3-Way-Handshake</li><li>Communication Reset</li><li>Timeout</li><li>Send and Receive Messages</li></ul></li></ul>
| Message Layer | <ul><li> Message De-/Encoding to/from Bytes, including CRC calculation and escape sequence handling.</li><li> Generate In-/Output Message Stream</li></ul>
| Byte Layer | <ul><li> Open/Close sockets and create Bytestream</li><li> Listen/Write to Bytestream of socket</li></ul>  