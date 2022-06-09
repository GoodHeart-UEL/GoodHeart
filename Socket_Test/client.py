import socket
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('localhost', 3333)
print(sys.stderr, 'connecting to %s port %s' % server_address)
sock.connect(server_address)

try:
    while True:
        data = input("Digite o que deseja enviar:")
        sock.sendall(data.encode())
        data = sock.recv(100)
        print(f"Recebido: %s"%data.decode())
finally:
    sock.close()