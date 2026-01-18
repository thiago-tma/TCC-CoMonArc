from platformio.public import DeviceMonitorFilterBase


class Demo(DeviceMonitorFilterBase):
    NAME = "filterComonArc3"

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        print("My custom filter is loaded")

    def rx(self, text):
        return f"Received: {text}\n"

    def tx(self, text):
        return text