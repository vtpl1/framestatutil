FROM ubuntu:24.04

RUN apt update \
    && DEBIAN_FRONTEND=noninteractive apt install -y \
    ca-certificates \
    software-properties-common \
    build-essential \
    wget curl git gdb ninja-build \
    lsb-release \
    pkg-config zip unzip tar iputils-ping ccache nasm \
    bison cpio autoconf-archive autotools-dev automake libtool gperf \
    gpg-agent flex graphviz \
    python3-tk python3-pip python3-setuptools \
    python3-dev python3-venv python3-jinja2 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

    RUN wget -O cmake.sh https://github.com/Kitware/CMake/releases/download/v3.28.5/cmake-3.28.5-linux-x86_64.sh && \
    sh cmake.sh --prefix=/usr/local/ --exclude-subdir && rm -rf cmake.sh

ARG LLVM_VERSION=19
RUN wget https://apt.llvm.org/llvm.sh && chmod +x llvm.sh && ./llvm.sh ${LLVM_VERSION} all && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN apt-get update \
    && apt-get install -y libx11-dev libxi-dev \
    libxft-dev libxext-dev libgtk-3-dev \
    libc6 libc6-i386 libc6-x32 \
    libfreetype6 libxext6 libxi6 libxtst6 \
    libxrender1 \
    bison \
    nasm \
    bison cpio autoconf-archive autotools-dev automake libtool gperf \
    flex \
    libltdl-dev \
    libxtst-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

ENV SHELL=/bin/bash

ARG USERNAME=ubuntu
# ARG USER_UID=1000
# ARG USER_GID=$USER_UID

# # Create the user
# RUN groupadd --gid $USER_GID $USERNAME && \
#     useradd --uid $USER_UID --gid $USER_GID -m $USERNAME

# RUN groupmod --gid $USER_GID $USERNAME && \
#     usermod --uid $USER_UID --gid $USER_GID $USERNAME && \
#     chown -R $USER_UID:$USER_GID /home/$USERNAME

USER $USERNAME
# RUN python3 -m pip install --upgrade pip
# RUN python3 -m pip install --no-cache-dir loguru meson jinja2 bump2version