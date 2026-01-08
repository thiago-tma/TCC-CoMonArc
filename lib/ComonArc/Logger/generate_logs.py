# Script to generate logs used in the target device and in the serial decoder.
# Purpose is to keep the messages in the target and in the host matched.
# Script gets messages from log_messages.py
# Existing subsystems and levels are infered from the messages
#   log_ids.h: header with message IDs, the existing subsystems and message levels
#   log_api.h: functions to call each message (prevents mistakes in sending messages)
#   log_db.py: messages in JSON for the host-side decoder to use

import yaml
import sys
from pathlib import Path

SCHEMA_FILE = Path("log_messages.yaml")
GEN_DIR = Path("include")
TOOLS_DIR = Path("tools")

GEN_DIR.mkdir(exist_ok=True)
TOOLS_DIR.mkdir(exist_ok=True)

# --- Argument sizes --------------------------------------------

ARG_TYPE_INFO = {
    "u8":  {"ctype": "uint8_t",  "size": 1, "signed": False},
    "i8":  {"ctype": "int8_t",   "size": 1, "signed": True},
    "u16": {"ctype": "uint16_t", "size": 2, "signed": False},
    "i16": {"ctype": "int16_t",  "size": 2, "signed": True},
    "u32": {"ctype": "uint32_t", "size": 4, "signed": False},
    "i32": {"ctype": "int32_t",  "size": 4, "signed": True},
    "f32": {"ctype": "float",    "size": 4, "signed": False},
}

# --- Load YAML -------------------------------------------------

with open(SCHEMA_FILE, "r") as f:
    schema = yaml.safe_load(f)

# ------------------------------------------------------------
# Validate global structure
# ------------------------------------------------------------

required_top_keys = {"subsystems", "levels", "args", "messages"}
missing = required_top_keys - schema.keys()
if missing:
    sys.exit(f"Missing top-level keys: {missing}")

subsystems = schema["subsystems"]
levels = schema["levels"]
messages = schema["messages"]
arg_types = schema["args"]

if not isinstance(subsystems, list) or not subsystems:
    sys.exit("'subsystems' must be a non-empty list")

if not isinstance(levels, list) or not levels:
    sys.exit("'levels' must be a non-empty list")

if not isinstance(messages, dict) or not messages:
    sys.exit("'messages' must be a non-empty mapping")

if not isinstance(arg_types, list) or not arg_types:
    sys.exit("'args' must be a non-empty list")

# ------------------------------------------------------------
# Validate messages
# ------------------------------------------------------------

for name, msg in messages.items():
    if "subsystem" not in msg:
        sys.exit(f"{name}: missing 'subsystem'")
    if msg["subsystem"] not in subsystems:
        sys.exit(
            f"{name}: invalid subsystem '{msg['subsystem']}' "
            f"(must be one of {subsystems})"
        )

    if "level" not in msg:
        sys.exit(f"{name}: missing 'level'")
    if msg["level"] not in levels:
        sys.exit(
            f"{name}: invalid level '{msg['level']}' "
            f"(must be one of {levels})"
        )

    if "args" not in msg or msg["args"] is None:
        msg["args"] = []
    if not isinstance(msg["args"], list):
        sys.exit(f"{name}: 'args' must be a list")
    for a in msg["args"]:
        if a not in arg_types:
            sys.exit(
                f"{name}: invalid arg type '{a}' "
                f"(must be one of {arg_types})"
            )

    if "format" not in msg:
        sys.exit(f"{name}: missing 'format'")

    # Assign the payload size for each message based on arguments
    payload_size = 0
    for a in msg["args"]:
        if a not in ARG_TYPE_INFO:
            sys.exit(f"{name}: unknown arg type '{a}'")
        payload_size += ARG_TYPE_INFO[a]["size"]
    msg["payload_size"] = payload_size


# Assign IDs deterministically
sorted_names = sorted(messages.keys())
for i, name in enumerate(sorted_names, start=0):    # Must start at zero to enable ID count in enum
    messages[name]["id"] = i

# ------------------------------------------------------------
# Generate log_ids.h
# ------------------------------------------------------------

with open(GEN_DIR / "log_ids.h", "w") as f:
    f.write("// AUTO-GENERATED — DO NOT EDIT\n\n")
    f.write("#ifndef D_LOGS_IDS_H\n")
    f.write("#define D_LOGS_IDS_H\n\n")

    f.write("typedef enum {\n")
    for name in sorted_names:
        f.write(f"    {name} = {messages[name]['id']},\n")
    f.write("    LOG_MESSAGEID_COUNT\n")
    f.write("} Log_MessageId_t;\n\n")

    f.write("typedef enum {\n")
    for s in subsystems:
        f.write(f"    LOG_SUBSYS_{s},\n")
    f.write("    LOG_SUBSYSTEM_COUNT\n")
    f.write("} Log_Subsystem_t;\n\n")

    f.write("typedef enum {\n")
    for l in levels:
        f.write(f"    LOG_LEVEL_{l},\n")
    f.write("    LOG_LEVEL_COUNT\n")
    f.write("} Log_Level_t;\n")

    f.write("#endif /* D_LOGS_IDS_H */")

# --- Generate log_api.h ---------------------------------------

log_api_h = GEN_DIR / "log_api.h"

with open(log_api_h, "w") as f:
    f.write("// AUTO-GENERATED FILE — DO NOT EDIT\n\n")
    f.write("#ifndef D_LOG_API_H\n")
    f.write("#define D_LOG_API_H\n\n")
    f.write("#include <stdint.h>\n")
    f.write("#include \"log_ids.h\"\n")
    f.write("#include \"Logger.h\"\n\n")

    for name in sorted_names:
        msg = messages[name]
        args = msg["args"]

        # Function signature
        params = ", ".join(
            f"{ARG_TYPE_INFO[a]['ctype']} arg{i}"
            for i, a in enumerate(args)
        )

        f.write(f"static inline void {name.lower()}({params})\n")
        f.write("{\n")

        if msg["payload_size"] > 0:
            f.write(f"    uint8_t payload[{msg['payload_size']}];\n")
            offset = 0

            for i, a in enumerate(args):
                size = ARG_TYPE_INFO[a]["size"]

                if size == 1:
                    f.write(f"    payload[{offset}] = (uint8_t)arg{i};\n")

                else:
                    for b in range(size):
                        f.write(
                            f"    payload[{offset + b}] = "
                            f"(uint8_t)(arg{i} >> {8 * b});\n"
                        )

                offset += size

            f.write("\n")
            f.write("    Logger_Log(\n")
            f.write(f"        LOG_SUBSYS_{msg['subsystem']},\n")
            f.write(f"        LOG_LEVEL_{msg['level']},\n")
            f.write(f"        {name}, payload, sizeof(payload));\n")

        else:
            f.write("    Logger_Log(\n")
            f.write(f"        LOG_SUBSYS_{msg['subsystem']},\n")
            f.write(f"        LOG_LEVEL_{msg['level']},\n")
            f.write(f"        {name}, NULL, 0);\n")

        f.write("}\n\n")

    f.write("#endif /* D_LOG_API_H */")

# --- Generate decoder database --------------------------------

log_db_py = TOOLS_DIR / "log_db.py"

with open(log_db_py, "w") as f:
    f.write("# AUTO-GENERATED FILE — DO NOT EDIT\n\n")
    f.write("LOG_DB = {\n")
    for name in sorted_names:
        msg = messages[name]
        f.write(f"    {msg['id']}: {{\n")
        f.write(f"        'name': '{name}',\n")
        f.write(f"        'subsystem': '{msg['subsystem']}',\n")
        f.write(f"        'level': '{msg['level']}',\n")
        f.write(f"        'args': {msg['args']},\n")
        f.write(f"        'format': \"{msg['format']}\",\n")
        f.write("    },\n")
    f.write("}\n")