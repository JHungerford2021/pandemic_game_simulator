FROM alpine:latest

RUN apk add --no-cache --virtual build_dependencies \
    git \
    g++ \
    make \
    cmake 

RUN git clone https://github.com/JHungerford2021/pandemic_game_simulator

WORKDIR /pandemic_game_simulator

RUN mkdir build
RUN cmake -S . -B build; build/make

RUN apk del build_dependencies