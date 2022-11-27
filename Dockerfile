FROM ubuntu:22.04

RUN apt update && apt install -y python3.11 libpython3.11-dev curl libhyperscan-dev libhyperscan5 build-essential gnuplot jq libboost-regex-dev
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y --default-toolchain 1.65.0
RUN curl https://bootstrap.pypa.io/get-pip.py | python3.11 -
RUN python3.11 -m pip install cython pandas
