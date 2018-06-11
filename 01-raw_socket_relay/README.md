# Raw Socket Test

Raw Socket is POSIX ABI compatible socket for network communication.

**Introduction**:
> Raw Socket Port Relay Test. The measured program sample are enclosed
> as `tx_side.c` and `rx_side.c`, which function as relay from `port A`
> to `port B` over localhost.
> The test unit use `ncat` to provide UDP socket creation and monitor.

**Dependency**:
> ncat; python-2.7;

### Usage
* (Optional) `make` to build the sample program
* First, run the measured program
* Then, `make ncat-rx`
* Finally, `make ncat-tx` and input text in the console