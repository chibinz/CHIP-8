# CHIP-8
First attempt writing an emulator

Development of this emulator started in late 2018, but it was only made fully functional until early 2021. Back then I hoped to gain a foothold on emulation development by implementing this CHIP-8 emulator, but progress soon came into a cease due to lack of computer architecture knowledge and inadequate debugging skills. It took just a few days to fix the bugs previously commited once I picked up this project the second time. The way that CHIP-8 handles IO, dedicated instructions for drawing sprites and blocking for keypad input, drastically differs from real machines, which is usually done by MMIO. Nevertheless, I think it is a fun and worthwhile experience.

## Features
This emulator is created with cross-platform support in mind. Two binaries are generated at build, `libchip.a` and `chip`. The former is a static library that implements the core logic of the CHIP-8 interpreter. By default, the `console` struct is allocated on stack; no allocation is needed. Rom loading functionality is up to the user to implement to avoid dependency on the file system. Core functions are exposed in the `console.h` header file. The latter is an example frontend that handles IO and links against `libchip.a`. `chip` is tested on both Windows and Linux, but presumably adapting it to embedded devices would require minimal effort.

```C
console console_new();
void console_timer_tick(console *console);
void console_cpu_tick(console *console);
void console_tick(console *console);

...

void disassemble(u8 lo, u8 hi);
void disassemble_rom(u8 *rom, usize start, usize len);
```

## Building
```sh
meson build
meson compile -C build
```

## Usage
```sh
build/chip <path_to_rom.ch8>
```

## Dependencies
- `meson`
- `cmake`
`chip` relies on `MiniFB` to get a frame buffer, but you don't need to explicitly install it yourself. `meson` directly fetch source files from the `MiniFB` repository declared in `subprojects/minifb.wrap`. However, you still need `cmake` to build `MiniFB`. Prebuilt binaries are available at the github release section.

## References
- [Emulator 101](http://www.emulator101.com/chip-8-emulator.html)
- [Awesome CHIP-8](https://chip-8.github.io/links/)
- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [MiniFB](https://github.com/emoon/minifb)
- [chip8-test-rom](https://github.com/corax89/chip8-test-rom)
