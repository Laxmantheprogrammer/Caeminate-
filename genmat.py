import json
import random
import os

# ============================================================
# USER SETTINGS
# ============================================================

# Number of configurations to generate
NUM_CONFIGS = 10

# Maximum number of layers in a shield
MAX_LAYERS = 5

# Maximum total mass (arbitrary units)
MAX_MASS = 500

# Thickness settings (mm)
MIN_THICKNESS = 1.0
MAX_THICKNESS = 20.0
THICKNESS_RESOLUTION = 0.5

# Beam energy settings (keV)
MIN_ENERGY = 0.5
MAX_ENERGY = 5.0
ENERGY_RESOLUTION = 0.5

# Number of events per simulation
NUM_EVENTS = 10000

# Output file
OUTPUT_FILE = "../output/Output"
BEAM_POSITION = [-50, 0, 0]
BEAM_DIRECTION = [0, 0, 1]

# Materials available for shielding
MATERIALS = {
    "G4_Pb": {
        "density": 11.34
    },
    "G4_Al": {
        "density": 2.70
    },
    "G4_Fe": {
        "density": 7.87
    }
}

# Particles available for simulation
PARTICLES = [
    "gamma",
    "e-",
    "e+",
    "proton",
    "neutron",
    "alpha"
]

# ============================================================
# HELPER FUNCTIONS
# ============================================================

def random_thickness():
    steps = int((MAX_THICKNESS - MIN_THICKNESS) / THICKNESS_RESOLUTION)

    return (
        MIN_THICKNESS
        + random.randint(0, steps) * THICKNESS_RESOLUTION
    )


def random_energy():
    steps = int((MAX_ENERGY - MIN_ENERGY) / ENERGY_RESOLUTION)

    return (
        MIN_ENERGY
        + random.randint(0, steps) * ENERGY_RESOLUTION
    )


def generate_layer():

    material = random.choice(list(MATERIALS.keys()))

    density = MATERIALS[material]["density"]

    thickness = random_thickness()

    return {
        "material": material,
        "thickness_mm": thickness,
    }


def generate_config():

    layers = []

    total_mass = 0

    num_layers = random.randint(1, MAX_LAYERS)

    for _ in range(num_layers):

        layer = generate_layer()

        mass = layer["thickness_mm"] * layer["density"]

        if total_mass + mass > MAX_MASS:
            break

        total_mass += mass

        layers.append(layer)

        layers = layers[:MAX_LAYERS]

    while len(layers) < MAX_LAYERS:
        layers.append({
            "material": "None",
            "thickness_mm": 0.0,
        })

    return {

        "layers": layers,

        "beam": {

            "particle": random.choice(PARTICLES),

            "energy_keV": random_energy(),

            "position_mm": BEAM_POSITION,

            "direction": BEAM_DIRECTION

        },

        "run": {

            "events": NUM_EVENTS,

            "output": OUTPUT_FILE

        }

    }


# ============================================================
# MAIN
# ============================================================

script_dir = os.path.dirname(os.path.abspath(__file__))

build_dir = os.path.join(script_dir, "build")

os.makedirs(build_dir, exist_ok=True)

configs = [generate_config() for _ in range(NUM_CONFIGS)]

config_path = os.path.join(build_dir, "config.json")

with open(config_path, "w") as f:

    json.dump(configs, f, indent=2)

print(f"Generated {NUM_CONFIGS} configurations in {config_path}")