#Reciever
from socket import *
socket.socket

ip="127.0.0.1"
port=55555

sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sock.bind((ip, port))

print(f'Start listening ti {ip}:{port}')

while True:
    data, addr=sock.socket.recvfrom(1024)
    print(f"recieved message: {data}")