#!/usr/bin/python
import socket
import Queue
from time import time

RX_PORT=13333

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('', RX_PORT))

    while True:
        buf = sock.recv(2048)
        # print("%s|%.06f"%(buf, time()))
        t2 = time()
        buf = buf.split('|')
        t1 = float(buf[1])
        print("%s|delay:%.03fus"%(buf[0], (t2-t1)*1E6))
        pass
    pass

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print(e)
    finally:
        exit()