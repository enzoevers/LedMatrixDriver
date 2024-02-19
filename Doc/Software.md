#

# Setup

```bash
cd <root of this repo>
git submodule init
```

# Using the scripts

## STM32

### Available scripts

- ./Scripts/Stm32/BuildStm32.sh
- [sudo] ./Scripts/Stm32/CleanStm32Build.sh

## Desktop

### Available scripts

- ./Scripts/Desktop/BuildDesktop.sh
- ./Scripts/Desktop/CleanDesktopBuild.sh

## Testing

### Prerequisits

```bash
sudo apt install clang \
                 libstdc++-12-dev
```

> The reason for `libstdc++12-devmak` is described here: https://stackoverflow.com/questions/74543715/usr-bin-ld-cannot-find-lstdc-no-such-file-or-directory-on-running-flutte

### Available scripts

- ./Scripts/Test/BuildTest.sh
- [sudo] ./Scripts/Test/CleanTestBuild.sh
- ./Scripts/Test/RunTest.sh

## Documentation

### Prerequisits

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

### Available scripts

- ./Scripts/Doc/BuildDoc.sh
- ./Scripts/Doc/CleanDocBuild.sh

The `index.html` can then be found in `Code/BuildDoc/Doc/html/index.html`
