#! /usr/bin/env python3

import argparse
import asyncio

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--host',
                        type=str,
                        default='0.0.0.0',
                        help='The interface that you would like to bind to',
                        )

    parser.add_argument('--port',
                        type=str,
                        default='8000',
                        help='The port that you would like to listen on',
                        )

    return parser.parse_args()

class ThinSox():
    host = ""
    port = ""
    loop = asyncio.get_event_loop()
    reader = None
    writer = None
    server = None

    def __init__(self, host, port):
        self.host = host
        self.port = port

    async def get_connection(self):
        return await asyncio.open_connection(host=self.host, port=int(self.port), loop=self.loop)

    def run(self):
        try:
            self.read_messages()
            self.loop.run_forever()
        except KeyboardInterrupt:
            pass
        finally:
            self.server.close()
            self.loop.run_until_complete(self.server.wait_closed())
            self.loop.close()
            return

    def read_messages(self):
        self.server = self.loop.run_until_complete(
            asyncio.start_server(
                self.get_message,
                self.host,
                int(self.port),
                loop=self.loop)
        )

    async def send_message(self, message):
        pass

    async def get_message(self, reader, writer):
        while True:
            data = await reader.read(100)
            message = data.decode()
            print(message)
            if 'STOP' in message:
                break
        writer.close()

if __name__ == '__main__':
    args = get_args()
    thinsox = ThinSox(args.host, args.port)
    thinsox.run()
