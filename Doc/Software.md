# Setup

```bash
cd <root of this repo>
git submodule init
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

After `cmake ..` was called from the `Code/build` directory, the following command can be build to generate html documentation.

```bash
doxygen Doc/Doxyfile.doxygen
```

The `index.html` can then be found in `Code/build/Doc/html/index.html`
