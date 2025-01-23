#!/bin/bash

wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo tee /etc/apt/trusted.gpg.d/lunarg.asc
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-$(lsb_release -sc).list http://packages.lunarg.com/vulkan/lunarg-vulkan-$(lsb_release -sc).list
sudo apt update -y
sudo apt install vulkan-sdk
