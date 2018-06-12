# Raw Socket Test

Raw Socket is POSIX ABI compatible socket for network communication.
UDP Sender(12222) ==> Tx_Side ==> Rx_Side ==> UDP Receiver(13333)
Tx_Side: Raw_Socket_Rx (12222) + UDP_Tx (12306)
Rx_Side: UDP_Rx (12306) + Raw_Socket_Tx (13333)

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

###TODO
- [x] jitter caculation (ncat --> python)