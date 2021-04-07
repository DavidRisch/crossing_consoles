\page messages Messages

| Type  | Purpose | Sender | Receiver | Acknowledge
| :----------------- | :---------- |  :----------: | :---------: | :--------: |
Acknowledge | Show that a message has been received | All | All | No
Connection Request | Request a connection | Client | Server | No
Connection Reset | Close a connection on purpose | All | All | Yes
Connection Response | Show that a connection can be established | Server | Client | Yes
Keepalive | Prevent timeout | All | All | Yes
Not Acknowledge | Show problem with received message, request resend | All | All | No
Payload | Send information | All | All | Yes
