from platformio.public import DeviceMonitorFilterBase
import struct

import sys
from pathlib import Path

sys.path.append(str(Path(__file__).parent))

from log_db import LOG_MESSAGES,LOG_SUBSYSTEMS,LOG_LEVELS

class LogDecoder(DeviceMonitorFilterBase):
    NAME = "comonarc_log_decoder"



    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._buffer = bytearray()
        print("comonarc_log_decoder is loaded\n")


    # ---------------------------------------------------------
    # RX entry point (byte stream)
    # ---------------------------------------------------------
    def rx(self, data):
        START_BYTE = 0xAA
        MIN_LENGHT = 3
        output = []

        try:
            # Convert string into bytes
            self._buffer.extend(data.encode("latin1"))
        except Exception as e:
            print(f"An error occurred: {e}")
            self._buffer.clear()
            return ""

        while(True):

            # Identify beggining of a message
            messageStart = self._buffer.find(START_BYTE)
            if messageStart == -1:
                self._buffer.clear()
                break

            frame = self._buffer[messageStart:]
            if (len(frame) < MIN_LENGHT):
                # Wait for the rest of the frame
                break

            # Collect parameters
            msg_id       =  frame[1]
            payload_size =  frame[2]

            # Wait for the rest of the message
            if (len(frame) < MIN_LENGHT + payload_size):
                break

            # Collect payload and define the end of the message in the buffer (preserve next message bytes after clear)
            messageEnd = messageStart+MIN_LENGHT+payload_size
            payload = frame[MIN_LENGHT:MIN_LENGHT+payload_size]
        
            # If messageID = 0, payload is raw text
            if msg_id == 0:
                output.append(f"[RAW MESSAGE][RAW MESSAGE]: {payload.decode('ascii', 'replace')}\n")
                del self._buffer[:messageEnd]
                break

            # Find messageID match in the local database
            message = LOG_MESSAGES.get(msg_id, None)
            if (message is None):
                print(f"Message ID doesn't have a match (messageID = {msg_id})\n")
                del self._buffer[:messageEnd]
                break

            # -------------------------------------------------
            # Decode payload
            # -------------------------------------------------
            try:
                arg_types = message["args"]
                values = self._decode_args(payload, arg_types)
                text = message["format"].format(*values)
            except Exception as e:
                print("Error: Unable to decode\n")
                text = f"[DECODE ERROR id={msg_id}] {e}"

            output.append(f"[{message.get('subsystem', '?')}][{message.get('level', '?')}]: {text}\n")
            del self._buffer[:messageEnd]

        separator = ""
        result = separator.join(output)

        return result

    # ---------------------------------------------------------
    # Helpers
    # ---------------------------------------------------------
    def _decode_args(self, payload, arg_types):
        values = []
        offset = 0

        for a in arg_types:
            if a == "u8":
                values.append(payload[offset])
                offset += 1

            elif a == "i8":
                values.append(struct.unpack_from("<b", payload, offset)[0])
                offset += 1

            elif a == "u16":
                values.append(struct.unpack_from("<H", payload, offset)[0])
                offset += 2

            elif a == "i16":
                values.append(struct.unpack_from("<h", payload, offset)[0])
                offset += 2

            elif a == "u32":
                values.append(struct.unpack_from("<I", payload, offset)[0])
                offset += 4

            elif a == "i32":
                values.append(struct.unpack_from("<i", payload, offset)[0])
                offset += 4

            elif a == "f32":
                values.append(struct.unpack_from("<f", payload, offset)[0])
                offset += 4

            else:
                raise ValueError(f"Unsupported arg type '{a}'")

        if offset != len(payload):
            raise ValueError(
                f"Payload size mismatch (used {offset}, got {len(payload)})"
            )

        return values