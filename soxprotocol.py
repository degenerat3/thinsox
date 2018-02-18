import asyncio


class SoxClientProtocol(asyncio.DatagramProtocol):
    def __init__(self, message, loop):
        self.message = message
        self.loop = loop
        self.transport = None

    def connection_made(self, transport):
        self.transport = transport
        self.translate_to_sox()
        print(self.message)
        self.transport.sendto(self.message.encode())

    def error_received(self, exc):
        if isinstance(exc, OSError):
            if exc.errno != 65:
                print('Error received:', exc)

    def connection_lost(self, exc):
        print("Socket closed, stop the event loop")
        # loop = asyncio.get_event_loop()
        # loop.stop()

    def translate_to_sox(self):
        self.message = "SOX"  + self.message

    def translate_from_sox(self):
        self.message = self.message[3:]

class SoxServerProtocol:
    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        username, message = self.translate_from_sox(data.decode())
        print('Received %r from %s' % (message, username))
        message = self.translate_to_sox(message)

    def connection_lost(self, exc):
        print("Socket closed, stop the event loop")
        # loop = asyncio.get_event_loop()
        # loop.stop()

    def translate_to_sox(self, message):
        return "SOX" + message

    def translate_from_sox(self, message):
        return message[3:8].replace(" ", ""), message[8:]