# Keyboard Tool

[ English | [简体中文](doc/README_ZH.md) ]

Warnning
---
**The library has been using CPP rewritten, migrated to project the [Keyboard tools](https://github.com/JaderoChan/keyboard_tools), this repository no longer maintenance, and exists some bugs**
---

A cross-platform keyboard event listening and sending library written in C. Originally derived from the `KeyboardHook` module in the [Global Hotkey](https://github.com/JaderoChan/global_hotkey) library.  

## Features

- Written in C99
- Supports Windows, Linux, and macOS

## How to Build?

1. Run the `install` script in the root directory to install the library and example programs to `./install` directory.
2. Use `CMake`, for example:

    ```sh
    cmake -B ./build -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles"
    ```

## Build Options

- `KBDT_BUILD_SHARED`

    Whether to build a dynamic library. Defaults to `OFF`.

- `KBDT_BUILD_WITH_UTILITY`

    Whether to compile the `Utility` module, which includes functions for mapping `keyboard_key` to `native key code`, as well as conversion functions between `keyboard_key` and strings. If you already have a **keycode** mapping utility, you can set this option to `OFF`. Defaults to `ON`.

- `KBDT_BUILD_EXAMPLE`

    Whether to build example programs.

## Notes

- Generally, administrator privileges are required for proper execution.
- Do not call the `end` function within the thread where the `event_handler` is running.

## Dependences

[jwinarske/pthreads4w](https://github.com/jwinarske/pthreads4w) Used to provide **pthread** via cmake in the **MSVC** environment (partially modified)
