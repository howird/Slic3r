FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get -y update && \
    apt-get -y install git wget curl make g++ unzip cmake python3 python3-dev python-is-python3 libboost-all-dev vim

COPY . /slic3r
WORKDIR "/slic3r"

# RUN mkdir build
# WORKDIR "/slic3r/build"
# RUN cmake .. && \
#     cmake --build .

# RUN wget https://raw.githubusercontent.com/KrisRoofe/curaengine-dockerfile/master/herringbone-gear-large.stl && \
#     wget https://upload.wikimedia.org/wikipedia/commons/3/36/3D_model_of_a_Cube.stl