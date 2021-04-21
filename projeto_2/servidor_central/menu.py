import asyncio

from prompt_toolkit import Application
from prompt_toolkit.application import get_app
from prompt_toolkit.layout.containers import VSplit, Window, HSplit
from prompt_toolkit.layout.controls import FormattedTextControl
from prompt_toolkit.layout.layout import Layout
from prompt_toolkit.key_binding import KeyBindings
from prompt_toolkit.widgets import CheckboxList, Frame, Label
from prompt_toolkit.formatted_text import HTML


class Menu:
    class CheckboxListNoScroll(CheckboxList):
        show_scrollbar = False

    def __init__(self, commands_queue, states_queue):
        self.kb = KeyBindings()
        self.is_running = True
        self.commands_queue = commands_queue
        self.states_queue = states_queue

        self.temperature = 0
        self.humidity = 0
        self.environment = self.get_environment()

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

        self.update_switches()
        self.update_sensors()

        @self.kb.add('q')
        def exit_(event):
            """
            Pressing Q will exit the user interface.
            """
            self.stop()

        @self.kb.add('s')
        def send(event):
            # TOGGLE EVENT
            selected_switches = self.switches.current_values
            asyncio.create_task(self.commands_queue.put(selected_switches))

    def bind_state(self, current_state):
        """
        State format: <key_1>:<value_1>;<key_2>:<value2>; ...
        """
        current_state = current_state.split(';')
        states = filter(None, current_state)
        for state in states:
            key, value = state.split(':')
            if key == 'temperature':
                self.temperature = float(value)
            elif key == 'humidity':
                self.humidity = float(value)
            elif key in self.switches_structure:
                self.switches_structure[key]['value'] = int(value)
            elif key in self.sensors_structure:
                self.sensors_structure[key]['value'] = int(value)
            else:
                print('Error when binding initial state of switches/sensors')

    def get_environment(self):
        environ_html = HTML(
            f"<foo>Temperature:</foo> {self.temperature:0.4f}\n"
            f"<foo>Humidity:</foo> {self.humidity:0.4f}\n"
        )
        environ = Window(content=FormattedTextControl(environ_html))

        return environ

    def update_environment(self):
        environ_html = HTML(
            f"<foo>Temperature:</foo> {self.temperature:0.4f}\n"
            f"<foo>Humidity:</foo> {self.humidity:0.4f}\n"
        )

        self.environment.content.text = environ_html

    def get_switches(self):
        switches_list = [
            [key, info['name']]
            for key, info in self.switches_structure.items()
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
            Window(FormattedTextControl(info['name']))
            for key, info in self.sensors_structure.items()
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
            new_state = await self.states_queue.get()

            if new_state:
                self.bind_state(new_state)
            elif not self.is_running:
                print('Closing program state received')
            else:
                print('INVALID NEW STATE')

            self.update_environment()
            self.update_switches()
            self.update_sensors()

            get_app().invalidate()

    def stop(self):
        self.is_running = False
        asyncio.create_task(self.commands_queue.put(None))
        asyncio.create_task(self.states_queue.put(None))

        app = get_app()
        if app.is_running:
            app.exit()

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
            Frame(
                title="Environment",
                body=HSplit([
                    VSplit([self.environment])
                ], padding=1),
            )
        ], width=50)

        layout = Layout(root_aux)

        app = Application(
            layout=layout,
            full_screen=True,
            key_bindings=self.kb
        )
        await asyncio.gather(
            self.update(),
            app.run_async(),
        )
