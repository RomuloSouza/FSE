import asyncio

from prompt_toolkit import Application
from prompt_toolkit.application import get_app
from prompt_toolkit.buffer import Buffer
from prompt_toolkit.layout.containers import VSplit, Window, HSplit
from prompt_toolkit.layout.controls import BufferControl, FormattedTextControl
from prompt_toolkit.layout.layout import Layout
from prompt_toolkit.key_binding import KeyBindings
from prompt_toolkit.widgets import CheckboxList, Frame, Label
from prompt_toolkit.formatted_text import HTML

from constants import MAX_QUEUE_SIZE

class Menu:
    class CheckboxListNoScroll(CheckboxList):
        show_scrollbar = False

    def __init__(self):
        self.msg = 'teste'
        self.kb = KeyBindings()
        self.is_running = True
        self.queue = asyncio.Queue(MAX_QUEUE_SIZE)
        self.environment = {
            'temperature': 0,
            'humidity': 0
        }
        self.switches_structure = {
            'lamp_1': {
                'name': 'Lâmpada 01 (Cozinha)',
                'value': 0
            },
            'lamp_2': {
                'name': 'Lâmpada 02 (Sala)',
                'value': 0
            },
            'lamp_3': {
                'name': 'Lâmpada 03 (Quarto 01)',
                'value': 0
            },
            'lamp_4': {
                'name': 'Lâmpada 04 (Quarto 02)',
                'value': 0
            },
            'air_1': {
                'name': 'Ar-Condicionado 01 (Quarto 01)',
                'value': 0
            },
            'air_2': {
                'name': 'Ar-Condicionado 02 (Quarto 02)',
                'value': 0
            },
        }

        self.switches = self.get_switches()
        self.update_switches()

        self.sensors_structure = {
            'sensor_pres_1': {
                'name': 'Sensor de Presença 01 (Sala)',
                'value': 0
            },
            'sensor_pres_2': {
                'name': 'Sensor de Presença 02 (Cozinha)',
                'value': 0
            },
            'sensor_door_kitchen': {
                'name': 'Sensor Abertura 01 (Porta Cozinha)',
                'value': 0
            },
            'sensor_window_kitchen': {
                'name': 'Sensor Abertura 02 (Janela Cozinha)',
                'value': 0
            },
            'sensor_door_living_room': {
                'name': 'Sensor Abertura 03 (Porta Sala)',
                'value': 0
            },
            'sensor_window_living_room': {
                'name': 'Sensor Abertura 04 (Janela Sala)',
                'value': 0
            },
            'sensor_window_room_1': {
                'name': 'Sensor Abertura 05 (Janela Quarto 01)',
                'value': 0
            },
            'sensor_window_room_2': {
                'name': 'Sensor Abertura 06 (Janela Quarto 02)',
                'value': 0
            },
        }
        self.sensors = self.get_sensors()
        self.update_sensors()

        @self.kb.add('c-c')
        def exit_(event):
            """
            Pressing Ctrl-C will exit the user interface.

            Setting a return value means: quit the event loop that drives the user
            interface and return this value from the `Application.run()` call.
            """
            event.app.exit()

        @self.kb.add('d')
        async def vish(event):
            """TESTE
            """
            if self.switches_structure['lamp_1']['value']:
                switch = {
                    'lamp_1': 0
                }
                self.sensors_structure['sensor_pres_1']['value'] = 0
            else:
                switch = {
                    'lamp_1': 1
                }
                self.sensors_structure['sensor_pres_1']['value'] = 1
            await self.queue.put(switch)

    def get_switches(self):
        switches_list = [
            [key, info['name']] for key, info in self.switches_structure.items()
        ]

        switches = self.CheckboxListNoScroll(switches_list)

        return switches

    def update_switches(self):
        def get_text_colored(key):
            value = self.switches_structure[key]['value']
            name = self.switches_structure[key]['name']
            color = 'green' if value else 'red'

            return HTML(f'<{color}>{name}</{color}>')

        for idx, (key, _) in enumerate(self.switches.values):
            self.switches.values[idx][1] = get_text_colored(key)

    def get_sensors(self):
        sensors = [
            Window(FormattedTextControl(info['name'])) for key, info in self.sensors_structure.items()
        ]

        return sensors

    def update_sensors(self):
        def get_text_colored(key):
            value = self.sensors_structure[key]['value']
            name = self.sensors_structure[key]['name']

            color = 'green' if value else 'red'
            return HTML(f'<{color}>{name}</{color}>')

        for idx, (key, _) in enumerate(self.sensors_structure.items()):
            self.sensors[idx].content.text = get_text_colored(key)


    async def update(self):
        while self.is_running:
            switches = await self.queue.get()

            for key, value in switches.items():
                self.switches_structure[key]['value'] = value

            self.update_switches()
            self.update_sensors()

            get_app().invalidate()
    
    async def start(self):
        root_aux = VSplit([
            Frame(
                title="Central Menu",
                body=HSplit([
                    Label(HTML("\n<b>Switches:</b>"), width=20),
                    HSplit([self.switches]),

                    Label(HTML("\n<b>Sensors:</b>"), width=20),
                    HSplit(self.sensors),
                ], padding=1)
            ), 
            # Frame(
            #     title="Environment",
            #     body=HSplit([
            #         VSplit([
            #             Label(HTML("\n<b>Temperature: </b>"), width=10),
            #         ])
            #     ], padding=1)
            # )
        ], width=50)

        layout = Layout(root_aux)

        app = Application(layout=layout, full_screen=True, key_bindings=self.kb)
        await asyncio.gather(
            self.update(),
            app.run_async(),
        )