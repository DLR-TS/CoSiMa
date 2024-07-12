FROM ubuntu:22.04 AS cosima_builder
LABEL maintainer="frank.baumgarten@dlr.de"

ENV DEBIAN_FRONTEND=noninteractive MAKEFLAGS="-j$(nproc)"
RUN apt-get update && apt-get install -y cmake build-essential pip git libxerces-c-dev && rm -rf /var/lib/apt/lists/*
RUN pip install conan==1.59.0

RUN git clone https://github.com/eclipse-sumo/sumo.git
WORKDIR /sumo
ENV SUMO_HOME=/sumo
RUN cmake -B build .
RUN cmake --build build -j $(nproc)

WORKDIR /
RUN mkdir cosima && mkdir cosima/build
WORKDIR /cosima/build
COPY . /cosima/

RUN cmake .. -DBUILD_SHARED_LIBS=false -DCMAKE_BUILD_TYPE=Release
RUN cmake --build . --target CoSimulationManager -j $(nproc)

FROM ubuntu:22.04
RUN apt-get update && apt-get install -y libxerces-c3.2 && rm -rf /var/lib/apt/lists/*
COPY --from=cosima_builder /cosima/build/bin/CoSimulationManager .
COPY --from=cosima_builder /sumo/bin/libsumocpp.so /usr/lib
CMD ./CoSimulationManager -v config.yml

