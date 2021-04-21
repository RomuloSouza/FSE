# mock = "lamp_1:0;lamp_2:0;lamp_3:0;lamp_4:0;air_1:1;air_2:0;sensor_pres_1:1;sensor_pres_2:1;sensor_door_kitchen:1;sensor_window_kitchen:1;sensor_door_living_room:1;sensor_window_living_room:1;sensor_window_room_1:1;sensor_window_room_2:1;"

import asyncio

from client import send_message
from constants import (
    HOST_CENTRAL,
    PORT_CENTRAL,
    MAX_QUEUE_SIZE,
    MAX_BUFFER_SIZE
)
from menu import Menu

class Server:
    async def initial_state(self, writer=None):
        print('Requesting initial state...')
        await send_message('INITIAL\0')

    async def commands_handler(self, commands_queue, states_queue):
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
                command = f'T{str(toggle_mapping[switch])}\0'
                await send_message(command)

    async def states_handler(self, reader, writer, states_queue):
        while True:
            try:
                payload = await reader.read(MAX_BUFFER_SIZE)
                decoded_states = payload.decode('utf-8')
                await states_queue.put(decoded_states)
            except asyncio.IncompleteReadError:
                host, port, *_ = writer.get_extra_info('peername')

                print("Probably %s:%s was interrupted. Dropping connection..",
                       host, port)

                raise asyncio.CancelledError
            except Exception as err:
                print(err)
                raise err

    async def connection_handler(self, reader, writer):
        commands_queue = asyncio.Queue(MAX_QUEUE_SIZE)
        states_queue = asyncio.Queue(MAX_QUEUE_SIZE)

        await self.initial_state()

        menu = Menu(commands_queue, states_queue)
        tasks = asyncio.gather(
            menu.start(),
            self.commands_handler(commands_queue, states_queue),
            self.states_handler(reader, writer, states_queue)
        )

        try:
            await tasks
        except Exception as err:
            print('Closed the menu', err)

        try:
            for task in asyncio.Task.all_tasks():
                task.cancel()

            print('All tasks finished')
        except Exception as err:
            print('Error when cancelling tasks: ', err)

        print('Closing connections...')
        writer.close()
        await writer.wait_closed()
        print('Closed server connection to distributed')

    async def start(self):
        server = await asyncio.start_server(self.connection_handler, HOST_CENTRAL, PORT_CENTRAL)

        host, port, *_ = server.sockets[0].getsockname()
        print('Waiting for connections on %s:%s ...', host, port)

        async with server:
            await server.serve_forever()

if __name__ == '__main__':
    server = Server()
    asyncio.run(server.start())
