FROM ubuntu:22.04 AS cosima_builder
LABEL maintainer="frank.baumgarten@dlr.de"

ENV DEBIAN_FRONTEND=noninteractive MAKEFLAGS="-j$(nproc)"
RUN apt-get update && apt-get install -y cmake build-essential pip git && rm -rf /var/lib/apt/lists/*
RUN pip install conan==1.59.0

RUN mkdir cosima && mkdir cosima/build
WORKDIR /cosima/build
COPY . /cosima/

RUN cmake .. -DBUILD_SHARED_LIBS=false -DCMAKE_BUILD_TYPE=Release
RUN cmake --build . --target CoSimulationManager

FROM ubuntu:22.04
COPY --from=cosima_builder /cosima/build/bin/CoSimulationManager .
CMD ./CoSimulationManager -v config.yml
