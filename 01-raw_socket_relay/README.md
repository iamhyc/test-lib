# Raw Socket Test

Raw Socket is POSIX ABI compatible socket for network communication.

**Introduction**:
> Raw Socket Port Relay Test. The meseared program should work as 
> intermeddiate, listen over "port A", send over "port B". The test
> unit has two part as UDP Tx and UDP Rx. "ncat" script is integrated.

**Dependency**:
> ncat; python-2.7;

### Usage
* make
* make ncat-tx
* make ncat-rx