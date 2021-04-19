mock = "lamp_1:0;lamp_2:0;lamp_3:0;lamp_4:0;air_1:1;air_2:0;sensor_pres_1:1;sensor_pres_2:1;sensor_door_kitchen:1;sensor_window_kitchen:1;sensor_door_living_room:1;sensor_window_living_room:1;sensor_window_room_1:1;sensor_window_room_2:1;"

import asyncio

from constants import (
    HOST_CENTRAL,
    PORT_CENTRAL
)
from menu import Menu

class Server:
    async def connection_handler(self, reader, writer):
        menu = Menu()
        tasks = asyncio.gather(menu.start())
        try:
            await tasks
        except Exception as err:
            print(err)
            tasks.cancel()
        
        writer.write(b'Bye World')
        writer.close()

    async def start(self):
        server = await asyncio.start_server(self.connection_handler, HOST_CENTRAL, PORT_CENTRAL)

        host, port, *_ = server.sockets[0].getsockname()
        print('Waiting for connections on %s:%s ...', host, port)

        async with server:
            await server.serve_forever()

if __name__ == '__main__':
    server = Server()
    asyncio.run(server.start())
