import json
import random
import os

MAX_MASS = 500          # arbitrary units
NUM_LAYERS = 5          # maximum layers per configuration
NUM_CONFIGS = 3  # number of configurations to generate

materials = [
    ("G4_Pb", 11.34),
    ("G4_Al", 2.70),
    ("G4_Fe", 7.87)
]

def generate_layer():
    mat, density = random.choice(materials)
    thickness = random.uniform(1, 20)  # mm

    return {
        "material": mat,
        "thickness_mm": thickness,
        "density": density
    }

def generate_config():
    layers = []
    total_mass = 0

    for _ in range(NUM_LAYERS):
        layer = generate_layer()
        mass = layer["thickness_mm"] * layer["density"]

        if total_mass + mass > MAX_MASS:
            break

        total_mass += mass
        layers.append(layer)

    return {
        "layers": layers,
        "beam": {
            "particle": "gamma",
            "energy_keV": random.uniform(0.5, 5.0),
            "position_mm": [-50, 0, 0],
            "direction": [1, 0, 0]
        },
        "run": {
            "events": 10000,
            "output": "../output/Output"
        }
    }

# Directory containing this Python script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Build directory
build_dir = os.path.join(script_dir, "build")
os.makedirs(build_dir, exist_ok=True)

# Generate all configurations
configs = [generate_config() for _ in range(NUM_CONFIGS)]

# Save them into one file
config_path = os.path.join(build_dir, "config.json")

with open(config_path, "w") as f:
    json.dump(configs, f, indent=2)

print(f"Generated {NUM_CONFIGS} configurations in {config_path}")