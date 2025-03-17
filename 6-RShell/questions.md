1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client detects the end of a command's output by looking for a special end of message marker, also known as the EOF character. TCP is a stream oriented protocol so data might arrive in segments (partial reads). As a result the client repeatedly calls the receive function, recv(), and accumulates the data until it sees that the last byte received equals the designated marker.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A network shell protocol should define clear boundaries using either delimiters or length fields. Without these boundaries, the receiver may combine multiple commands into one or split a single command across multiple reads, leading to parsing errors and misinterpretation.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintain session info between requests. The server keeps track of the clients state, so each subsequent request can be processed in context. On the other hand, in stateless protocols, every request is independent. No session info is maintained between requests. Each request must contain all necessary data for the server to process it.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

Even though UDP is reliable, it is used for advantages such as lower latency and simplicity. It is also used in real time applications such as video conferencing or online gaming. UDP also supports broadcasting for network services. It has speed and efficiency basically.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides the sockets API which has the details of network communication and allows applications to create, manage, and use network connections thru standard functions. Some standard functions would inclide connect() send() recv() etc. This abstraction simplifies network programming by hiding the complexities of functions and protocols.
