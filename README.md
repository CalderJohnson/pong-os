__Pong OS__

*Whats the best thing about computers to you? Is it the vast possibilities they create for social connection? Maybe the abundance of information, right at your fingertips? Perhaps still you enjoy their ability to crunch data and create new discoveries to improve our lives? Well if you answered with any of those, your opinion is wrong. Clearly their ultimate achievement is that you can play pong. Endlessly. That is true progress and freedom.*

Pong OS is a minimal operating system that boots up and runs the classic game pong directly on the cpu, handling all low level tasks itself, meant as an exploration into the development of operating systems and device drivers. It was written in C and x86 assembly and runs in 32 bit mode.

To run it yourself, you will need:
- NASM
- clang (or a gcc cross compiler, this will require you to update your makefile)
- lld
- qemu
- gnumake (or equivalent)

Then run the shell script and have fun :) (w to go up, s to go down)