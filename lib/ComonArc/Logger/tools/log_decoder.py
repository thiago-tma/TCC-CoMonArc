from platformio.public import DeviceMonitorFilterBase
from tools.log_db import LOG_DB
import struct

class LogDecoder(DeviceMonitorFilterBase):
    NAME = "log_decoder"

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._buffer = bytearray()

    # ---------------------------------------------------------
    # RX entry point (byte stream)
    # ---------------------------------------------------------
    def rx(self, data):
        self._buffer.extend(data)
        output = bytearray()

        while True:
            # Need at least header: 4 bytes
            if len(self._buffer) < 4:
                break

            origin = self._buffer[0]
            level = self._buffer[1]
            msg_id = self._buffer[2]
            payload_size = self._buffer[3]

            total_len = 4 + payload_size
            if len(self._buffer) < total_len:
                break

            payload = self._buffer[4:total_len]

            # Consume buffer now (safe)
            del self._buffer[:total_len]

            # -------------------------------------------------
            # ID = 0 → raw string passthrough
            # -------------------------------------------------
            if msg_id == 0:
                try:
                    text = payload.decode(errors="replace")
                except Exception:
                    text = "<INVALID STRING>"
                output.extend((text + "\n").encode())
                continue

            # -------------------------------------------------
            # Unknown message ID
            # -------------------------------------------------
            if msg_id not in LOG_DB:
                output.extend(
                    f"[UNKNOWN MSG id={msg_id} subsys={origin} level={level}]\n"
                    .encode()
                )
                continue

            entry = LOG_DB[msg_id]
            arg_types = entry["args"]

            # -------------------------------------------------
            # Decode payload
            # -------------------------------------------------
            try:
                values = self._decode_args(payload, arg_types)
                text = entry["format"].format(*values)
            except Exception as e:
                text = f"[DECODE ERROR id={msg_id}] {e}"

            # Optional: prepend subsystem/level
            output.extend((text + "\n").encode())

        return bytes(output)

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