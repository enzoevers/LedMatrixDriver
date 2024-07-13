# LedMatrixDriver

<img src="./Coverage/coverage.svg">

---

- Documentation
  - [How to build and use the software.](./Doc/Software.md)
  - [Electronics of the LED matrix.](./Doc/SignalFlow.md)

---

This project was a good platform for the following learning points:
- Making use of a very big LED matrix display which otherwise would just take up space in the house.
- **Reverse engineering:** Before the software could be written, the protocol had to be reverse-engineered by probing the IC pins on the PCB and seeing what these ICs are doing. See [Electronics of the LED matrix.](./Doc/SignalFlow.md) for details.
- **Getting back into 'bare-metal' STM32 development:** I wanted to get back into bare-metal STM32 development, but without the big IDEs like [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) and their HAL. I wanted to read the datasheet and really figure out how to set the register up by hand from scratch. I ended up using the [ObKo/stm32-cmake](https://github.com/ObKo/stm32-cmake) project in my CMake infra for getting the correct compilers and linker scripts etc. From there I also decided to use [Common Microcontroller Software Interface Standard (CMSIS)](https://arm-software.github.io/CMSIS_6/latest/General/index.html) to set the registers without having to go through the trouble of finding the address of those registers. As for the IDE I used VSCode that was connected via SSH to an ubuntu machine (my main OS was Windows).
- **Using lcov for code coverage and showing this as a GitHub badge**
- **Applying Test Driven Design (TDD):** while writing the code in [`./Code/Libs`](./Code/Libs) I forced myself to use TDD from the start. This lead to libraries with just the code that is needed for the features that are used. It also resulted in a testable design of the code by forcing abstractions which also resulted in easily using mocks for testing. Note that while creating the code in [`./Code/Applications/Clock`](./Code/Applications/Clock) once the library code was functionally finished, I was applying TDD as well, but not as strict anymore. Simply because I wanted to move on to another hobby project. The fact that TDD was used also resulted in a lot of [open issues](https://github.com/enzoevers/LedMatrixDriver/issues) still to be done at some point. But I think that this is also the point of TDD. Implement now what is needed (with extendability and good design practices in mind of course) and only implement new features when needed.
