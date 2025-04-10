FROM ubuntu:latest AS builder

# Install necessary build tools and Conan
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install --break-system-packages conan

# Verify Conan version
RUN conan --version

# Set working directory
WORKDIR /app

# Copy your project files
COPY ./CMakeLists.txt ./conanfile.txt /app
COPY ./src /app/src
COPY ./tests /app/tests

# Add your build steps here
RUN conan profile detect --force
RUN conan install . --build=missing
WORKDIR /app/build
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
RUN  cmake --build .


FROM busybox:latest AS final

EXPOSE 8080

COPY --from=builder /app/build/player /app/player

WORKDIR /app
CMD ["/app/player"]
