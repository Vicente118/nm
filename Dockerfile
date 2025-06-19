FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y \
    gcc \
    binutils \
    make \
    nasm \
    vim \
    file \
    gdb \
    valgrind \
    strace \
    man-db \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make

CMD ["/bin/bash"]