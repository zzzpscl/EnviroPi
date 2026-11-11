
import json

LIMITS = {
    "t": (0, 50),
    "h": (5, 95),
    "p": (900, 1100),
    "co2": (400, 5000),
    "gas": (1, 1022)
}

def parse(line):
    d = json.loads(line)
    return d

def validate(d):
    for key, (min, max) in LIMITS.items():
        if key not in d:
            raise ValueError(f"Feld fehlt: {key}.")
        if d[key] is None:
            continue
        if not isinstance(d[key], (int, float)):
            raise ValueError(f"Feld {key}: Wert ist keine Zahl.")