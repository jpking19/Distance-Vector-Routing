Distance Vector Routing Protocol

Jason King

Implementation of a distributed asynchronous distance vector(DV) routing protocol. This project was completed using C over a network emulator already provided by the professor of the Computer Networks class. My contribution was the sending and receiving IP-level messages for routing data over a network.

## Running the emulator

First, "make all"

Then, run "p3"

A default node configuration will initialize and then send packets containing their distance tables to each other. The simulation will end when every node has a converged distance table, which accurately contains the shortest route to every other node in the configuration.
