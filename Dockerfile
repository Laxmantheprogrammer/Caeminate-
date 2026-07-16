FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# -----------------------------
# System dependencies
# -----------------------------
RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    wget \
    curl \
    python3 \
    python3-pip \
    python3-dev \
    libxerces-c-dev \
    libexpat1-dev \
    qtbase5-dev \
    qtbase5-dev-tools \
    libqt5opengl5-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxmu-dev \
    libxi-dev \
    freeglut3-dev \
    libssl-dev \
    libfftw3-dev \
    libtbb-dev \
    && rm -rf /var/lib/apt/lists/*


# -----------------------------
# Install Geant4 11.3.2
# -----------------------------

WORKDIR /opt

RUN git clone --depth 1 --branch v11.3.2 \
    https://github.com/Geant4/geant4.git

RUN mkdir /opt/geant4-build && \
    cd /opt/geant4-build && \
    cmake ../geant4 \
    -DCMAKE_INSTALL_PREFIX=/opt/geant4-install \
    -DGEANT4_INSTALL_DATA=ON \
    -DGEANT4_USE_QT=ON \
    -DGEANT4_USE_OPENGL_X11=ON \
    -DGEANT4_USE_SYSTEM_EXPAT=ON \
    -DGEANT4_USE_SYSTEM_ZLIB=ON \
    && make -j$(nproc) \
    && make install


ENV Geant4_DIR=/opt/geant4-install/lib/cmake/Geant4
ENV PATH=/opt/geant4-install/bin:$PATH


# -----------------------------
# Install ROOT
# -----------------------------

RUN git clone --depth 1 \
    https://github.com/root-project/root.git /opt/root

RUN mkdir /opt/root-build && \
    cd /opt/root-build && \
    cmake ../root \
    -DCMAKE_INSTALL_PREFIX=/opt/root-install \
    -Dgnuinstall=ON \
    -Droofit=ON \
    -Dtmva=ON \
    && make -j$(nproc) \
    && make install


ENV ROOTSYS=/opt/root-install
ENV PATH=/opt/root-install/bin:$PATH
ENV LD_LIBRARY_PATH=/opt/root-install/lib:$LD_LIBRARY_PATH


# -----------------------------
# Python ML stack
# -----------------------------

RUN pip3 install \
    numpy \
    pandas \
    matplotlib \
    uproot \
    awkward \
    scikit-learn \
    jupyter


# -----------------------------
# Caeminate
# -----------------------------

WORKDIR /caeminate

COPY . /caeminate


RUN mkdir build && \
    cd build && \
    cmake .. \
    -DGeant4_DIR=/opt/geant4-install/lib/cmake/Geant4 \
    && make -j$(nproc)


# Default location

WORKDIR /caeminate


CMD ["/bin/bash"]