import json
import random
import os

# ============================================================
# USER SETTINGS
# ============================================================

# Number of output files
NUM_FILES = 1

# Configurations per file
CONFIGS_PER_FILE = 1

# Maximum number of layers in a shield
MAX_LAYERS = 5

# Maximum total mass (arbitrary units)
MAX_MASS = 500

# Thickness settings (mm)
MIN_THICKNESS = 1.0
MAX_THICKNESS = 3.0
THICKNESS_RESOLUTION = 0.5

# Beam energy settings (keV)
MIN_ENERGY = 100
MAX_ENERGY = 10000
ENERGY_RESOLUTION = 250

# Number of events per simulation
NUM_EVENTS = 10000

# Output file prefix
OUTPUT_FILE = "../output/Output"

BEAM_POSITION = [-50, 0, 0]
BEAM_DIRECTION = [0, 0, 1]

# Materials available for shielding
MATERIALS = {
    "G4_AIR": {
        "density": 0.001225
    },
    "G4_WATER": {
        "density": 1.000
    },
    "G4_POLYETHYLENE": {
        "density": 0.94
    },
    "G4_Al": {
        "density": 2.70
    },
    "G4_B": {
        "density": 2.34
    },
    "G4_Be": {
        "density": 1.85
    },
    "G4_C": {
        "density": 2.267
    },
    "G4_Cu": {
        "density": 8.96
    },
    "G4_Fe": {
        "density": 7.87
    },
    "G4_Ag": {
        "density": 10.49
    },
    "G4_Au": {
        "density": 19.32
    },
    "G4_Pb": {
        "density": 11.34
    },
    "G4_Ta": {
        "density": 16.69
    },
    "G4_Ti": {
        "density": 4.50
    },
    "G4_W": {
        "density": 19.25
    }
}

# Particles available for simulation
PARTICLES = [
    "gamma",
    "e-",
    "e+",
    "proton",
    "neutron",
    "alpha",
    "mu-",
    "mu+",
    "pi-",
    "pi+",
    "kaon-",
    "kaon+",
    "deuteron",
    "triton",
    "He3",
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

        mass = (
            layer["thickness_mm"]
            * MATERIALS[layer["material"]]["density"]
        )

        if total_mass + mass > MAX_MASS:
            break

        total_mass += mass

        layers.append(layer)

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

# Stores every configuration generated so far
unique_configs = set()

for file_num in range(NUM_FILES):

    configs = []

    while len(configs) < CONFIGS_PER_FILE:

        config = generate_config()

        # Convert to a deterministic string for uniqueness checking
        key = json.dumps(config, sort_keys=True)

        if key in unique_configs:
            continue

        unique_configs.add(key)
        configs.append(config)

    config_path = os.path.join(
        build_dir,
        f"config_{file_num}.json" #COME HERE BRUH COME HERE
    )

    with open(config_path, "w") as f:
        json.dump(configs, f, indent=2)

print(
    f"\nDone! Generated {NUM_FILES * CONFIGS_PER_FILE} unique configurations across {NUM_FILES} files."
)
