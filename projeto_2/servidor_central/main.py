import asyncio

from constants import (
    HOST_CENTRAL,
    PORT_CENTRAL,
    MAX_QUEUE_SIZE,
    MAX_BUFFER_SIZE
)
from menu import Menu
from csv_writer import CSV
from client import send_message


class Server:
    async def initial_state(self):
        print('Requesting initial state...')
        await send_message('INITIAL\0')

    async def commands_handler(self, commands_queue, states_queue, csv):
        toggle_mapping = {
            'lamp_1': 1,
            'lamp_2': 2,
            'lamp_3': 3,
            'lamp_4': 4,
            'air_1': 5,
            'air_2': 6,
        }
        while True:
            commands = await commands_queue.get()
            if commands is None:
                break

            for switch in commands:
                csv.write(f'T{switch}')
                command = f'T{str(toggle_mapping[switch])}\0'
                await send_message(command)

    async def states_handler(self, reader, writer, states_queue, csv):
        while True:
            try:
                payload = await reader.read(MAX_BUFFER_SIZE)
                decoded_states = payload.decode('utf-8')
                if decoded_states:
                    csv.write(decoded_states)

                await states_queue.put(decoded_states)
            except asyncio.IncompleteReadError:
                host, port, *_ = writer.get_extra_info('peername')

                print(f'Probably {host}:{port} was interrupted.', host, port,
                      'Dropping connection..')

                raise asyncio.CancelledError
            except Exception as err:
                print(err)
                raise err

    async def connection_handler(self, reader, writer):
        commands_queue = asyncio.Queue(MAX_QUEUE_SIZE)
        states_queue = asyncio.Queue(MAX_QUEUE_SIZE)
        csv = CSV()

        await self.initial_state()

        menu = Menu(commands_queue, states_queue)
        tasks = asyncio.gather(
            menu.start(),
            self.commands_handler(commands_queue, states_queue, csv),
            self.states_handler(reader, writer, states_queue, csv)
        )

        try:
            await tasks
        except Exception:
            print('Closed the menu')

        try:
            for task in asyncio.Task.all_tasks():
                task.cancel()

            print('All tasks finished')
        except Exception as err:
            print('Error when stopping tasks: ', err)

        print('Closing connections...')
        csv.close()
        writer.close()
        await writer.wait_closed()

    async def start(self):
        server = await asyncio.start_server(
            self.connection_handler,
            HOST_CENTRAL,
            PORT_CENTRAL
        )

        host, port, *_ = server.sockets[0].getsockname()
        print('Waiting for connections on %s:%s ...', host, port)

        async with server:
            await server.serve_forever()


if __name__ == '__main__':
    server = Server()
    asyncio.run(server.start())
