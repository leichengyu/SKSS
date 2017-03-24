# -*- coding:utf-8 -*-

# test if clients and fds are destroyed when client closed

import socket
import sys

if len(sys.argv) != 3:
    print("Usage: python connections.py host port")

host = sys.argv[1]
port = int(sys.argv[2])

while True:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    s.close()

