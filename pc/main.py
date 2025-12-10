# Simple TCP server that prints incoming data (line or chunk)
import socket

HOST, PORT = "0.0.0.0", 5000
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(1)
print(f"listening on {PORT} ...")
conn, addr = s.accept()
print("connected:", addr)

with conn:
    while True:
        data = conn.recv(1024)   # read bytes from client
        print(data.decode("utf-8", errors="ignore").rstrip())