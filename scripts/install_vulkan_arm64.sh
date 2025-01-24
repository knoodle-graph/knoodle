#!/bin/bash

sudo apt update -y
sudo apt install -y libglm-dev cmake libxcb-dri3-0 libxcb-present0 libpciaccess0 \
libpng-dev libxcb-keysyms1-dev libxcb-dri3-dev libx11-dev g++ gcc \
libwayland-dev libxrandr-dev libxcb-randr0-dev libxcb-ewmh-dev \
git python-is-python3 bison libx11-xcb-dev liblz4-dev libzstd-dev \
ocaml-core ninja-build pkg-config libxml2-dev wayland-protocols python3-jsonschema \
clang-format

wget -O - https://sdk.lunarg.com/sdk/download/1.4.304.0/linux/vulkansdk-linux-x86_64-1.4.304.0.tar.xz | tar xJf -

cd 1.4.304.0

sudo apt install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu

export C=aarch64-linux-gnu-gcc
export CXX=aarch64-linux-gnu-g++

./vulkansdk -j $(nproc) vulkan-headers vulkan-loader vulkan-validationlayers spirv-headers dxc
