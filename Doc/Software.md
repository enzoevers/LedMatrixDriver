# Prerequisits

```bash
sudo apt install clang \
                 libstdc++-12-dev
```

> The reason for `libstdc++12-devmak` is described here: https://stackoverflow.com/questions/74543715/usr-bin-ld-cannot-find-lstdc-no-such-file-or-directory-on-running-flutte

# Setup

```bash
cd <root of this repo>
git submodule init
```

```bash
cd Code

mkdir build
cd build
```

## STM32

```bash
cmake .. -DUSE_STM32=ON -DUSE_STM32F303XC=ON
make -j8
```

## Desktop

```bash
cmake .. -DUSE_DESKTOP=ON
make -j8
```

# Testing

```bash
CC=clang CXX=clang++ cmake .. -DTEST_ON_PC=ON
make -j8

./HAL/test/TestHAL
```

# Doxygen

## Install Doxygen

> NOTE: The project can also be built without Doxygen. Just no documentation will be generated in this case.

The [Code/Doc/CMakeLists.txt](../Code/Doc/CMakeLists.txt) makes use of the Doxygen cmake package. To use it, install Doxygen as described here: https://www.doxygen.nl/manual/install.html

```bash
export DOX_VERSION=1.10.0

wget https://www.doxygen.nl/files/doxygen-$DOX_VERSION.src.tar.gz

gunzip doxygen-$DOX_VERSION.src.tar.gz
tar xf doxygen-$DOX_VERSION.src.tar

cd doxygen-$DOX_VERSION
mkdir build
cd build

cmake -G "Unix Makefiles" ..
make

make install
```

## Generating documentation

To configure the doxygen files, use the `GENERATE_DOCS` property.

```bash
cmake .. -DGENERATE_DOCS=ON <any other properties>
```

After `cmake ..` was called from the `Code/build` directory, the following command can be build to generate html documentation.

```bash
doxygen Doc/Doxyfile.doxygen
```

The `index.html` can then be found in `Code/build/Doc/html/index.html`
