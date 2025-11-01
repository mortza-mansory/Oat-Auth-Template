FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV CPATH=/usr/local/include
ENV LIBRARY_PATH=/usr/local/lib
ENV LD_LIBRARY_PATH=/usr/local/lib

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential cmake git wget libssl-dev libsqlite3-dev \
    curl ca-certificates pkg-config libcurl4-openssl-dev nlohmann-json3-dev && \
    rm -rf /var/lib/apt/lists/*

# Install oatpp 1.3.0
WORKDIR /opt
RUN git clone --branch 1.3.0 https://github.com/oatpp/oatpp.git && \
    cd oatpp && \
    git checkout 1.3.0 && \
    git log -1 && \
    mkdir build && cd build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DOATPP_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc) || { echo "make failed"; exit 1; } && \
    make install || { echo "make install failed"; exit 1; } && \
    ldconfig && \
    # Debug: Check installed files and Oat++ version
    echo "Checking Oat++ include directory:" && \
    ls -l /usr/local/include/oatpp* || true && \
    ls -l /usr/local/include/oatpp*/oatpp || true && \
    echo "Checking Oat++ CMake directory:" && \
    ls -l /usr/local/lib/cmake/oatpp* || true && \
    echo "Checking for oatppConfig.cmake:" && \
    find /usr/local -name "oatppConfig.cmake" || true && \
    find /usr/local -name "oatpp-config.cmake" || true && \
    echo "Checking for specific headers (ConnectionProvider.hpp, component.hpp):" && \
    find /usr/local/include -name "ConnectionProvider.hpp" || true && \
    find /usr/local/include -name "component.hpp" || true && \
    echo "Checking Oat++ version in oatppConfig.cmake (if found):" && \
    find /usr/local -name "oatppConfig.cmake" -exec grep -i "VERSION" {} \; || true && \
    echo "Checking installed libraries:" && \
    ls -l /usr/local/lib/liboatpp* || true

# Install oatpp-swagger
RUN git clone --branch 1.3.0 https://github.com/oatpp/oatpp-swagger.git && \
    cd oatpp-swagger && \
    git checkout 1.3.0 && \
    git log -1 && \
    mkdir build && cd build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DOATPP_DIR=/usr/local/lib/cmake/oatpp-1.3.0 -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc) || { echo "oatpp-swagger make failed"; exit 1; } && \
    make install || { echo "oatpp-swagger make install failed"; exit 1; } && \
    ldconfig && \
    mkdir -p /usr/local/share/oatpp-swagger && \
    cp -r /opt/oatpp-swagger/res /usr/local/share/oatpp-swagger/ || echo "No res directory found" && \
    echo "Checking oatpp-swagger installation:" && \
    ls -l /usr/local/lib/liboatpp-swagger* || true && \
    ls -l /usr/local/lib/cmake/oatpp-swagger* || true && \
    ls -l /usr/local/share/oatpp-swagger/res || true

# Copy source
WORKDIR /app
COPY . /app

# Build with Oat++ 1.3.0 path
RUN mkdir build && cd build && \
    cmake -DOATPP_DIR=/usr/local/lib/cmake/oatpp-1.3.0 -DCMAKE_PREFIX_PATH=/usr/local -DCMAKE_VERBOSE_MAKEFILE=ON .. && \
    make -j$(nproc) || { echo "make failed"; exit 1; } && \
    # Debug: Output CMake logs
    echo "CMake Output Log:" && \
    cat CMakeFiles/CMakeOutput.log || true && \
    echo "CMake Error Log:" && \
    cat CMakeFiles/CMakeError.log || true

EXPOSE 8000
CMD ["./build/backend"]