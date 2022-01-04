FROM alpine:latest

RUN apk add --no-cache \
    git \
    g++ \
    make \
    cmake 

RUN git clone https://github.com/JHungerford2021/pandemic_game_simulator

WORKDIR /pandemic_game_simulator

RUN cmake .; make