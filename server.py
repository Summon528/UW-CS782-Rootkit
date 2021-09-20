import socketserver

HOST, PORT = "0.0.0.0", 43933

class MyTCPHandler(socketserver.BaseRequestHandler):
    def handle(self):
        print("{} connected".format(self.client_address[0]))
        while True:
            self.data = self.request.recv(128)
            print(self.data[:self.data.find(b'\x00')].decode("ascii"), end='', flush=True)

if __name__ == "__main__":
    socketserver.ThreadingTCPServer.allow_reuse_address = True
    with socketserver.ThreadingTCPServer((HOST, PORT), MyTCPHandler) as server:
        server.serve_forever()