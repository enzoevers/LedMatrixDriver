#!/bin/bash

set -e

# function cleanup()
# {
#     echo
#     echo "//===================="
#     echo "// Clean tests"
#     echo "//===================="
#     echo 

#     sudo Scripts/Test/CleanTestBuild.sh
# }

# trap cleanup EXIT

echo
echo "//===================="
echo "// Install submodules"
echo "//===================="
echo 

git submodule init
git submodule update

echo
echo "//===================="
echo "// Generic installs"
echo "//===================="
echo

sudo apt install -y clang-format
sudo apt install -y gcc-12 g++-12 \
                    libstdc++-12-dev \
                    lcov llvm

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100

wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 17
rm llvm.sh

wget -qO- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.7/install.sh | bash
export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"  # This loads nvm
[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"  # This loads nvm bash_completion

nvm install 21.1.0
nvm use 21.1.0

python3 -m pip install fontTools pillow argparse

echo
echo "//===================="
echo "// Build tests"
echo "//===================="
echo 

Scripts/Test/BuildTest.sh "build"

echo
echo "//===================="
echo "// Run tests"
echo "//===================="
echo 

Scripts/Test/RunTest.sh
