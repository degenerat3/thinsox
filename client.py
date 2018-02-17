#! /usr/bin/env python3

import argparse
from thinsox import ThinSox

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

if __name__ == '__main__':
    args = get_args()
    thinsox = ThinSox()

    if args.host:
        thinsox.set_host(args.host)
    if args.port:
        thinsox.set_port(args.port)

    try:
        username = input('Enter a username: ')
        username = username[:5]
        thinsox.set_username(username)
        while True:
            message = input('Enter a message: ')
            thinsox.send_message(message)
    except KeyboardInterrupt:
        pass