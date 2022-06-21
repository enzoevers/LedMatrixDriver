# LedMatrixDriver

systemd: https://www.raspberrypi.org/documentation/linux/usage/systemd.md
         https://raspberrypi.stackexchange.com/questions/54922/systemd-unit-dependencies-to-start-wiringpi-gpio-application
         https://stackoverflow.com/questions/45776003/fixing-a-systemd-service-203-exec-failure-no-such-file-or-directory
         https://askubuntu.com/questions/676007/how-do-i-make-my-systemd-service-run-via-specific-user-and-start-on-boot

In this project I reversed engineered a big led matrix which is used on the front of public transport busses. After reverse engineering this and documenting it in a schematic I started writing the low-level driver code and user libraries. The user library includes different fonts to use. The result is that a user can simply give a string to the library function and the text will appear on the led matrix.