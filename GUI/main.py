#! /usr/bin/env python
import kivy
kivy.require('1.10.0')

from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button

from kivy.core.window import Window

class MyApp(App):

    def build(self):

        layout = FloatLayout()

        text_area = Label(
            text="FUCK",
            color=[1, 0, 0, 1],
            font_size='20sp',
            size_hint=(1, .9),
            pos_hint={'x': 0, 'y': 0}
        )
        textinput = TextInput(
            text='',
            size_hint=(.9, .1),
            pos_hint={'x': 0, 'y': 0}
        )

        send_button = Button(
            text='Send',
            size_hint=(.1, .1),
            pos_hint={'x': .9, 'y': 0}
        )

        layout.add_widget(text_area)
        layout.add_widget(textinput)
        layout.add_widget(send_button)

        return layout

if __name__ == '__main__':
    # Window.system_size = (600, 500)
    MyApp().run()