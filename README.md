# Caeminate v01

## What is Caeminate?

This project is called **Caeminate**. The inspiration for the name comes from the Latin word **Caelus**, meaning *the sky*, and the English word **Laminate**, meaning something composed of multiple layers.

The goal of this project is to use **Monte Carlo simulation methods**, which use probabilistic sampling to simulate the interactions of different types of radiation with different materials. Since these interactions are inherently probabilistic, Monte Carlo methods are well suited for modelling them. The long-term objective is to use the generated data to discover materials that can shield specific types of radiation—such as neutrons, protons, and gamma rays—more effectively than the materials currently used in rockets and satellites.

## Motivation

I chose this idea for two main reasons.

First, radiation shielding for astronauts and spacecraft electronics is still an **active area of research**. We still do not know the optimal materials for protecting against different kinds of radiation, and this remains one of the factors limiting how long astronauts and electronics can safely operate in space. This is the first research problem I have worked on.

Second, when I first heard about this problem, I felt that it was something within my reach—something I could realistically attempt to work on myself (even though I did not do 99% of the work myself).

## Caemite

The theoretical end goal of Caeminate was **Caemite**. Unlike a laminate, Caemite would (theoretically) be a single material capable of blocking all forms of radiation almost perfectly. I now know that this is not possible with our current understanding of physics, but it was still the theoretical goal that inspired the project.

## `maybe_caeminate.cc`

The main file is called `maybe_caeminate.cc` because, when I first started this project, I did not know whether I could build something like this. So, instead of naming it something like `caeminate_v1`, I called it `maybe_caeminate`.

## About this version

I did almost everything with AI because, due to the science fair at my school where I planned to present this project, I did not have enough time to learn C++ to the level that would have been required.

If you see variables that are created but never used, files that could have been much smaller, or code that is far from optimized, just remember that this is the very first version of the project. At the time, I did not know C++, ROOT, Linux, Git, CMake, Python, or machine learning—essentially all of the technologies used here.

Just consider me a beginner and this project the roughest draft you have ever seen.

There are many limitations in this code, probably more than I can even imagine, but isn't that what a v01 is supposed to be?

I never intended this version to be perfect, and I don't expect any version to be perfect, but I do expect to add new things and understand them better in future versions.

Honestly, I do not understand most of the code in this project in depth. If you ask me what a particular section of the code does, I will most of the time not be able to answer. However, I do have a general idea of how the project works, and in future versions, I intend to write the code myself, using only as much AI assistance as actually makes sense.

## What each file does

I don't know what each file **does**. What I do understand is:

`include/` contains the C++ header files.  
`src/` contains the C++ source files.

`DetectorConstruction.cc` and `DetectorConstruction.hh` define the simulation geometry, including the world volume, materials, and logical volumes.

`physics.cc` and `physics.hh` define the Geant4 physics list, allowing the user to choose which physical processes and interactions are included in the simulation.

`SensitiveDetector.cc` and `SensitiveDetector.hh` implement the sensitive detectors used in the simulation. They collect the relevant simulation data and write it to ROOT output files.

`action.cc`, `action.hh`, `run.cc`, and `run.hh` implement the Geant4 user actions, including particle generation, run management, and other simulation callbacks.

There are other files as well, but I have no idea what they do.

## How it works

I don't know how it **works**. But I do know some parts of it.

This version implements a basic data generation pipeline.

First, I randomly generate many different material configurations using `genmat.py`. Running this script creates a `config.json` file inside the `build` directory.

These generated configurations are then simulated using Geant4, which models their interactions with different types of radiation at different energies. Geant4 produces one `.root` output file for each simulated configuration, and these files are stored inside the `output` folder.

After that, `root2csv.py` converts the generated `.root` files into a single `.csv` dataset.

**This repository only generates the dataset and does not train the machine learning model.**

## Usage

To use this project, follow these steps (or at least, this is how I use it).

**Note: THIS IS FOR WITHOUT VISUALIZATION:**

0. Configure Geant4 and ROOT on your machine. I am using a WSL Ubuntu terminal, but you may use whatever you want if you know what you are doing.

1. In the `maybe_caeminate` directory, make another directory called `output` (note that it is fully lowercase).

2. Go to the `build` directory (make it inside `maybe_caeminate` if it does not exist) and run in the terminal:

   * `cmake ..`
   * `make -j$(nproc)`

3. Edit `genmat.py` by setting the number of configurations and modifying any other parameters according to your needs.

4. Then, in the terminal, run (inside the `maybe_caeminate` directory):

   * `python3 genmat.py`

5. Verify that the generated configuration files are present inside the `build` folder.

6. Go to the `build` directory if you are not already there.

7. Run:

   * `./maybe_caeminate`

   (Also, if you ever accidentally stop the simulations, it is okay. There is a `completed.txt` file that notes how many configurations have been simulated, and it allows this project to skip the ones which are already simulated.)

8. After the simulation finishes, the generated `.root` files should appear inside the `output` folder.

9. Return to the `maybe_caeminate` directory and run:

   * `python3 root2csv.py`

This will generate the final CSV dataset.

Also, if you want visualization:

1. Search for the `fCheckOverlaps` boolean and set it to `true`.

2. Search for the `VISUALIZATION` bool in `maybe_caeminate.cc` and set it to `true` as well.

Future versions will improve as I add better ML, physics, and programming.

## Small thing

Now, I know this is a very hilarious code, but you have to do it at some point in your life. You can only start with bad code, but you can start with good ideas and vision.

If you believe that you need some refund for the time you wasted while trying to read the code and for the monstrosity your eyes have witnessed, please note that I am not giving you that.

## Credit

ChatGPT,  
Me (Laxman),  
My friend Ayush, username: `nurseryworker`   