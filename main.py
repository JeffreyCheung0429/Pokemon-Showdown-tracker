from utils import filterRooms, getRoomInfo, parseSocketInput, saveLog

import asyncio
import websockets
import logging
import configparser
import os

logging.basicConfig(level=logging.INFO)

class Client:
    def __init__(self, url: str, target: list[str] = [], format: str = ""):
        self.url: str = url
        self.target: list[str] = target
        self.format = format
        self.socket: websockets.ClientConnection = None
        self.rooms: dict[str, str] = {}
        self.leftRooms: list[str] = []

    async def send(self, message: str):
        await self.socket.send(message)
        logging.info(f"Sent: {message}")

    # Periodically send a command to get the list of rooms
    async def listenToRoomList(self, interval: int = 15):
        while True:
            await self.send(f"|/cmd roomlist {self.format}")
            await asyncio.sleep(interval)  # Interval of 60 seconds

    # Receive and process messages from the websocket
    async def receiver(self):
        while True:
            output = await self.socket.recv()

            if not output.startswith("|queryresponse|roomlist|"):
                logging.info(f"Received: {output}")
            else:
                logging.info(f"Received: (very long roomlist)")

            if output.startswith("|"):
                parsed = parseSocketInput(output)
                if type(parsed) == dict and "rooms" in parsed:
                    filteredRooms = filterRooms(parsed["rooms"], self.target)
                    await self.join(filteredRooms)

            elif output.startswith(">"):
                roomId, message = getRoomInfo(output)

                if roomId in self.leftRooms:
                    continue

                if roomId not in self.rooms:
                    self.rooms[roomId] = message
                else:
                    self.rooms[roomId] += message

                if "|win" in message:
                    await saveLog(roomId, self.rooms[roomId])
                    await self.leave(roomId)

            else:
                raise ValueError(f"Unexpected output: {output}")

    # Join the filtered rooms
    async def join(self, rooms: dict[str, dict]):
        for room in rooms:
            if room not in self.rooms and room not in self.leftRooms:
                await self.send(f"|/join {room}")

    # Leave the room
    async def leave(self, room: str):
        await self.send(f"|/noreply /leave {room}")
        self.leftRooms.append(room)

    # Run the client by connecting to the websocket and starting the listeners
    async def run(self):
        self.socket = await websockets.connect(self.url)
        await asyncio.gather(
            self.listenToRoomList(),
            self.receiver()
        )

if __name__ == "__main__":
    # Read the configuration file
    config_path = 'config.ini'
    config = configparser.ConfigParser()
    if not os.path.exists(config_path):
        config['settings'] = {'target': '', 'format': '', 'url': 'wss://sim3.psim.us/showdown/websocket'}
        with open(config_path, 'w') as config_file:
            config.write(config_file)
    else:
        config.read(config_path)

    targatPlayers = [i.strip().rstrip() for i in config.get('settings', 'target', fallback=[]).split(',')]
    formatRestriction = config.get('settings', 'format', fallback="")

    # Create a client instance and run it
    url = config.get('settings', 'url', fallback="wss://sim3.psim.us/showdown/websocket")
    client = Client(url, targatPlayers, formatRestriction)
    asyncio.run(client.run())
