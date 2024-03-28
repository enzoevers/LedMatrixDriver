#

# Setup

```bash
$ cd <root of this repo>
$ git submodule init
$ git submodule update
```

## For visual studio code
Go to the extensions and filter for `Recommended` to install the recommended extensions for this repository.

## Install CMAKE

[https://cmake.org/download/](https://cmake.org/download/)

## Install Ninja

Go to: [https://github.com/ninja-build/ninja/releases](https://github.com/ninja-build/ninja/releases), download the binary for your platform and app the path to the binary to your system PATH.

## Install clang-format

```bash
$ sudo apt install clang-format
```

# Using the scripts

## STM32F303xc

### Prerequisits

```bash
sudo apt install libnewlib-arm-none-eabi \
                 stlink-tools            \
                 openocd                 \
                 gdb-multiarch
```

The `stlink-tools` package comes from [https://github.com/stlink-org/stlink](https://github.com/stlink-org/stlink).

### Available scripts

- `$ ./Scripts/Stm32/Stm32f303xc/BuildStm32f303xc.sh`
- `$ [sudo] ./Scripts/Stm32/Stm32f303xc/CleanStm32f303xcBuild.sh`
- `$ ./Scripts/Stm32/Stm32f303xc/StLinkFlash.sh`
- `$ ./Scripts/Stm32/Stm32f303xc/StLinkDebug.sh`

### Debugging

> This is based on https://www.youtube.com/watch?v=_1u7IOnivnM

After running `./Scripts/Stm32/Stm32f303xc/StLinkDebug.sh`, type the following in the gdb window:

```bash
(gdb) target extended-remote localhost:3333
```

To restart the program, set a breakpoint at main and see the source code run (also see [https://openocd.org/doc/html/GDB-and-OpenOCD.html#Sample-GDB-session-startup](https://openocd.org/doc/html/GDB-and-OpenOCD.html#Sample-GDB-session-startup)):

```bash
(gdb) monitor reset halt
...
(gdb) load
...
(gdb) b main
...
(gdb) c
...
(geb) lay src
```

To close the source code viewer press `crtl+x+a`. To bring it back use the same command. For other layouts run `help lay`.

To continue the program type again `c`.

To break, press `ctrl+c`.

From here on see the GDB and OpenOCD documentation on how to see variables values etc.


## Desktop

### Available scripts

- `$ ./Scripts/Desktop/BuildDesktop.sh`
- `$ ./Scripts/Desktop/CleanDesktopBuild.sh`

## Testing

### Prerequisits

```bash
sudo apt install clang \
                 libstdc++-12-dev
```

> The reason for `libstdc++12-devmak` is described here: https://stackoverflow.com/questions/74543715/usr-bin-ld-cannot-find-lstdc-no-such-file-or-directory-on-running-flutte

### Available scripts

- `$ ./Scripts/Test/BuildTest.sh`
- `$ [sudo] ./Scripts/Test/CleanTestBuild.sh`
- `$ ./Scripts/Test/RunTest.sh`

When creating new tests, make sure to add the test to the [./Scripts/Test/RunTest.sh](./Scripts/Test/RunTest.sh) script.

## Documentation

### Prerequisits

The [Code/Doc/CMakeLists.txt](../Code/Doc/CMakeLists.txt) makes use of the Doxygen cmake package. To use it, install Doxygen as described here: https://www.doxygen.nl/manual/install.html

```bash
$ cd <somewhere other than this repo>
$ sudo apt install flex \
                   bison

$ export DOX_VERSION=1.10.0

$ wget https://www.doxygen.nl/files/doxygen-$DOX_VERSION.src.tar.gz

$ gunzip doxygen-$DOX_VERSION.src.tar.gz
$ tar xf doxygen-$DOX_VERSION.src.tar

$ cd doxygen-$DOX_VERSION
$ mkdir build
$ cd build

$ cmake -G "Unix Makefiles" ..
$ make -j12

$ [sudo] make -j12 install
```

### Available scripts

- `$ ./Scripts/Doc/BuildDoc.sh`
- `$ ./Scripts/Doc/CleanDocBuild.sh`

The `index.html` can then be found in [`Code/BuildDoc/Doc/html/index.html`](Code/BuildDoc/Doc/html/index.html).
