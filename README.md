# DialDRM
 "Interactive Keygen" for oldschool codewheel DRMs ... on your PS Vita

## Roadmap

* Make DialDRM as a System App so you can run it at the same time as other applications eg. ScummVM
* Add more wheels for more games (feel free to open an Issue if you want me to add a specific wheel)
* Improve general GUI and possibly add more functionality

## Image specifications

* **Wheels:** 525 x 525 px transparent PNG centered as well as possible
* **Game Logos:** 418 x n (maximum 525) px transparent PNG

## Build Instructions

Make sure to first install [libmathneon](https://github.com/Rinnegatamante/math-neon), [vitaGL](https://github.com/Rinnegatamante/vitaGL) and [dear ImGui](https://github.com/Rinnegatamante/imgui-vita).

When compiling VitaGL use: `make HAVE_SBRK=1 NO_DEBUG=1 install`.

Building DialDRM itself:
```
mkdir build
cd build
cmake .. && make
```
 
