# libemoji &nbsp;[![Build Status](https://travis-ci.org/emoji-gen/libemoji.svg?branch=master)](https://travis-ci.org/emoji-gen/libemoji) [![wercker status](https://app.wercker.com/status/de905f978179bb9ca837a8b14d0cdf07/s/master "wercker status")](https://app.wercker.com/project/byKey/de905f978179bb9ca837a8b14d0cdf07)

:tada: Ultimate Emoji Generator library using Skia

<br>
<img src="example/emoji.png" width="128" height="128" alt="libemoji">

## System requirements

- CMake
- Python 2.7
- C11 Compiler
- C++14 Compiler

### macOS

```
$ brew install cmake
```

### Ubuntu 16.04 TLS

```
$ sudo apt-get update
$ sudo apt-get install git cmake g++ python libfontconfig1-dev \
    libx11-dev libxcomposite-dev libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev -y
```

## How to build

```
$ git submodule update --init
$ cmake .
$ make
```

## License
MIT &copy; Emoji Generator
