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
    def __init__(self):
        self.kb = KeyBindings()
        self.running = 1
        self.queue = asyncio.Queue(MAX_QUEUE_SIZE)
        self.msg = FormattedTextControl('testando')
        self.count = 1

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
            """
            Pressing Ctrl-C will exit the user interface.

            Setting a return value means: quit the event loop that drives the user
            interface and return this value from the `Application.run()` call.
            """
            s = 'nova string porra!' + str(self.count)
            await self.queue.put(s)
            self.count += 1

    async def update(self):
        while self.running:
            new_state = await self.queue.get()
            self.msg.text = new_state
            get_app().invalidate()
    
    async def start(self):
        buffer1 = Buffer()  # Editable buffer.
        root_aux = HSplit([
            Window(self.msg, width=30)
        ], padding=1)
        root_container = HSplit([
            # A vertical line in the middle. We explicitly specify the width, to
            # make sure that the layout engine will not try to divide the whole
            # width by three for all these windows. The window will simply fill its
            # content by repeating this character.
            Window(height=1, content=FormattedTextControl(text=self.msg)),

            Window(height=1, char='-'),

            # One window that holds the BufferControl with the default buffer on
            # the bottom.
            Window(content=BufferControl(buffer=buffer1)),
            # Display the text 'Hello world' on the right.
        ])

        layout = Layout(root_aux)

        app = Application(layout=layout, full_screen=True, key_bindings=self.kb)
        await asyncio.gather(
            self.update(),
            app.run_async(),
        )