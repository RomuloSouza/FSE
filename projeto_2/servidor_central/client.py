import asyncio
from constants import HOST_DISTRIBUTED, PORT_DISTRIBUTED


async def send_message(message):

    _, writer = await asyncio.open_connection(
        HOST_DISTRIBUTED,
        PORT_DISTRIBUTED
    )

    writer.write(message.encode())

    await writer.drain()

    writer.close()
    await writer.wait_closed()
