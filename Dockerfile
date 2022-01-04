FROM alpine:latest AS builder

RUN apk add --no-cache git
    && apk add --no-cache --virtual build_dependencies \
    g++ \
    make \
    cmake
    && git clone https://github.com/JHungerford2021/pandemic_game_simulator
    && cd /pandemic_game_simulator
    && mkdir build
    && cmake -S . -B build 
    && make --directory build
    && apk del build_dependencies
    && apk --purge del apk-tools