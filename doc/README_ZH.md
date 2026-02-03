# Keyboard Tool

[ [English](README_EN.md) | 简体中文 ]

**警告**
---
**此库已使用cpp重写，迁移至项目[Keyboard tools](https://github.com/JaderoChan/keyboard_tools)，本仓库将不再维护，并且存在些许问题**
---

C语言多平台的键盘事件监听和发送库，起初来自[Global Hotkey](https://github.com/JaderoChan/global_hotkey)库中的`KeyboardHook`模块。

## 特性

- 使用C99
- 支持Windows、Linux、MacOs

## 如何编译？

1. 运行根目录下对应的`install`脚本，生成库和示例程序至`./install`目录下。
2. 使用`CMake`，例如：

    ```sh
    cmake -B ./build -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles"
    ```

## 编译选项

- `KBDT_BUILD_SHARED`

    是否生成动态库，默认为`OFF`。

- `KBDT_BUILD_WITH_UTILITY`

    是否编译`Utility`模块，其包含`keyboard_key`与`native key code`的映射函数，以及`keyboard_key`与字符串的互转函数。如果你已有**keycode**映射程序，则可以将此选项置`OFF`，默认为`ON`。

- `KBDT_BUILD_EXAMPLE`

    是否生成示例程序。

## 注意点

- 通常来讲，需要管理员权限才可以正常运行。
- 不要在`event_handler`所属线程中执行`end`函数。

## 依赖

[jwinarske/pthreads4w](https://github.com/jwinarske/pthreads4w) 用于在**MSVC**环境下通过cmake提供**pthread'**（进行了部分修改）
