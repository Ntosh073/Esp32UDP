#Reciever
import socket

ip="192.168.0.129"
port=33333

sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sock.bind((ip, port))

print(f'Start listening ti {ip}:{port}')

while True:
    data, addr=sock.recvfrom(1024)
    print(f"recieved message: {data}")