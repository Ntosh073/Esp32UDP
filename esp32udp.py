#Sender
import socket

ip="192.168.0.128"
port=33447
msg=b"Hello"

print(f'Sending {msg} to {ip}:{port}')

while True:
    sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    sock.sendto(msg,(ip, port))