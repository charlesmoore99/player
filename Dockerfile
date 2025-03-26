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

# Add your build steps here
RUN conan profile detect --force
RUN conan install . --output-folder=build --build=missing
RUN cmake . -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release
RUN make


FROM busybox:latest AS final
COPY --from=builder /app/player /app/player

WORKDIR /app
CMD ["/app/player"]