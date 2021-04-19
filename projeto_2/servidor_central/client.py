import socket
import sys

HOST = '192.168.0.4'     # The remote host
# HOST = '192.168.0.52'     # The remote host
PORT = 10109              # The same port as used by the server
s = None
for res in socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC, socket.SOCK_STREAM):
    af, socktype, proto, canonname, sa = res
    try:
        s = socket.socket(af, socktype, proto)
    except OSError as msg:
        s = None
        continue
    try:
        s.connect(sa)
    except OSError as msg:
        s.close()
        s = None
        continue
    break
if s is None:
    print('could not open socket')
    sys.exit(1)
with s:
    s.sendall(b'S')
    data = s.recv(512)

print('Received', repr(data))
decoded = data.decode('utf-8')
print('Decoded', decoded)
