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
        START_BYTE = 0xaa
        MIN_LENGHT = 5

        print(type(data))
        print(repr(data))
        print(data)

        if (data.encode("Latin-1","replace")[0] == 0xAA):
            print("Recognized  0xAA!")
        return ""

        try:
            # Convert string into bytes
            self._buffer.extend(data.encode("latin1"))
        except Exception as e:
            print(f"An error occurred: {e}")

        output = ""

        print(self._buffer)

        # Identify beggining of a message
        messageStart = self._buffer.find(START_BYTE)
        if messageStart == -1:
            self._buffer.clear()
            return ""
        
        print("OK")
        return ""

        frame = self._buffer[messageStart:]
        if (len(frame) < MIN_LENGHT):
            # Wait for the rest of the frame
            return ""
        
        # Collect parameters and convert to int
        origin       =  frame[1]
        level        =  frame[2]
        msg_id       =  frame[3]
        payload_size =  frame[4]

        # Wait for the rest of the message
        if (len(self._buffer) < 5 + payload_size):
            return ""
        
        message = LOG_MESSAGES.get(msg_id, None)
        if (message is None):
            print("No message found")
            self._buffer.clear()
            return ""
        messageText = message.get(format, "No text within message found")
        print(messageText)

        #output =  f"[{LOG_SUBSYSTEMS.get(origin,'?')}][{LOG_LEVELS.get(level,'?')}:] {LOG_MESSAGES.get(msg_id,{}).get('format','?')}"
        self._buffer.clear()

        return output

#            
#
#            total_len = 4 + payload_size
#            if len(self._buffer) < total_len:
#                break
#
#            payload = self._buffer[4:total_len]
#
#            # Consume buffer now (safe)
#            del self._buffer[:total_len]
#
#            # -------------------------------------------------
#            # ID = 0 → raw string passthrough
#            # -------------------------------------------------
#            if msg_id == 0:
#                try:
#                    text = payload.decode(errors="replace")
#                except Exception:
#                    text = "<INVALID STRING>"
#                output.extend((text + "\n").encode())
#                print("Raw Message Receiver\n")
#                break
#
#            # -------------------------------------------------
#            # Unknown message ID
#            # -------------------------------------------------
#            if msg_id not in LOG_DB:
#                output.extend(
#                    f"[UNKNOWN MSG id={msg_id} subsys={origin} level={level}]\n"
#                    .encode()
#                )
#                print("Error: msgId not in LOG_DB\n")
#                continue
#
#            entry = LOG_DB[msg_id]
#            arg_types = entry["args"]
#
#            # -------------------------------------------------
#            # Decode payload
#            # -------------------------------------------------
#            try:
#                values = self._decode_args(payload, arg_types)
#                text = entry["format"].format(*values)
#            except Exception as e:
#                print("Error: Unable to decode\n")
#                text = f"[DECODE ERROR id={msg_id}] {e}"
#
#            # Optional: prepend subsystem/level
#            output.extend((text + "\n").encode())
#
#        return bytes(output)

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