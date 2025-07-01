# 1. Install core C++ toolchain, Boost, Zlib, CMake, Python pip, and Google Test
sudo apt-get update
sudo apt-get install -y build-essential libboost-all-dev zlib1g-dev cmake python3-pip libgtest-dev  # installs compiler, Boost ≥1.70, Zlib, CMake, pip3, GTest :contentReference[oaicite:3]{index=3}
sudo apt-get install libasio-dev
vcpkg install crow


# 2. Build and install Google Test libraries
cd /usr/src/gtest
sudo cmake CMakeLists.txt    # configure GTest build :contentReference[oaicite:4]{index=4}
sudo make                    # build static libraries :contentReference[oaicite:5]{index=5}
sudo cp lib/*.a /usr/lib/     # copy .a files to system library path :contentReference[oaicite:6]{index=6}

# 3. Install pre-commit hooks manager
pip3 install pre-commit       # multi-language Git hook manager :contentReference[oaicite:7]{index=7}

# 4. Clone CrowCpp source
sudo git clone https://github.com/CrowCpp/Crow.git /usr/local/src/crow  # Crow source :contentReference[oaicite:8]{index=8}
sudo cp -r /usr/local/src/crow/include /usr/local/include

# 6. Verify installation with a test build
cat << 'EOF' > ~/crow_test.cpp
#include "crow.h"
int main() {
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")([](){ return "Hello world"; });
    app.port(18080).run();
}
EOF

g++ ~/crow_test.cpp -std=c++14 -lpthread -lboost_system -o ~/crow_test  # build test app :contentReference[oaicite:11]{index=11}
echo "CrowCpp setup complete; run ~/crow_test"
