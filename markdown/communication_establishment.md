\page communication-establishment Communication Establishment

Communication establishment is done using the following handshake:

<table>
<tr>
<th> Client Side </th>
<th> Server Side </th>
</tr>
<tr>
<td>
<br />
<br />

Send ConnectionRequestMessage <br />
Change state to: CLIENT_CONNECTION_REQUEST_SENT
<br />
<br />
<br />
<br />
Send AcknowledgeMessage <br />
Change state to: READY
<br />
<br />
<br />
<br />
</td>
<td>
Status: SERVER_WAITING_FOR_FIRST
<br />
<br />
<br />
<br />
<br />
Send ConnectionResponseMessage <br />
Change state to: SERVER_CONNECTION_RESPONSE_SENT
<br />
<br />
<br />
Validate Acknowledge <br />
Change state to: READY

</td>
</tr>
</table>