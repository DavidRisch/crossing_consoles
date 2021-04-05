\page messages Messages

| Type  | Purpose | Sender | Receiver | Acknowledge
| :----------------- | :----------: |  :----------: | :---------: | :--------: |
Acknowledge | Show that a message has been received | All | All | No
Connection Request | Request a connection, step 1 of CE | Client | Server | No
Connection Reset | Close connection on purpose or due to timeout | All | All | Yes
Connection Response | Connection can be established | Server | Client | Yes
Keepalive | Prevent timeout | All | All | Yes
Not Acknowledge | Show problem with received message, request resend | All | All | No
Payload | Send information | All | All | Yes
