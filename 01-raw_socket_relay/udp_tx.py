#!/usr/bin/python
import socket
from time import time, sleep

TX_PORT=12222

def main():
    cnt = 0
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # sock.connect(('127.0.0.1', TX_PORT))
    while True:
        sleep(1.0)
        cnt += 1
        buf = "%6d|%.06f"%(cnt, time())
        print(buf)
        sock.sendto(buf, ('127.0.0.1', TX_PORT))
        pass
    pass

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print(e)
    finally:
        exit()