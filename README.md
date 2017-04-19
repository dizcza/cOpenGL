# cOpenGL

This is C89 implementation of OpenGL basics for STM32F429 Discovery boards.

`git clone git@github.com:dizcza/cOpenGL.git --recursive`

[Latest binary](https://drive.google.com/drive/folders/0B5LYlYDnS3oHMWpkVGVkTzVuZUU?usp=sharing)

### Features

* Does not require any additional libraries.
* Written in ANSI C89.
* Uses [depth buffer](https://en.wikipedia.org/wiki/Z-buffering) to solve the visibility problem (hidden 3D objects are not drawn).
* Can draw _any_ object as long as its [polygon mesh](https://en.wikipedia.org/wiki/Polygon_mesh) is provided (vertices positions and colors are required).
* Fast framebuffer clear (both color and depth) with DMA technique. (This is done after each redraw of the 3D scene)
* [Barycentric coordinates](https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates) for smooth color interpolation between vertices.
* Test coverage of OpenGL modules.

### Not implemented features

* Ray tracing.
* Lighting and shadows.
* Textures.
* Antialiasing.

### Demo (YouTube)

[![Demo YouTube](http://i63.tinypic.com/2v8iv4g.jpg)](https://youtu.be/da3nk9iFHSY)

### Used libraries

* [linmath.h](https://github.com/dizcza/linmath.h)
* BSP (Board Support Package)
* HAL (Hardware Abstraction Layer)


### Benchmark

Benhmark is measured by FPS (frames per second) on the scene with 3 cubes for different optimization flags with asserts turned off.

| Checkpoint  | -O0   | -Os   | -O1  | -O2  | -O3   |
|----------------------|---------|--------|---------|--------|---------|
| Apr 18, 2017 | 1.25 | 5.38 | 6.10 | 7.81 | 8.48 |

### Issues

* Software
	* FPS(#cubes=1) = 24, FPS(#cubes=2) = 14, FPS(#cubes=3) = 8 -- very fast performance degradation with respect to number of cubes in the scene. This should not happen.
	
* Hardware
	* Touchscreen is not working in interruption mode (`BSP_TS_IT*`).
	* Touchscreen refuses to initialize after a few hardware resets, if compiled with optimization flag `-On` for any `n>0`. For some reason,  `stmpe811_ReadID()` returns `0` instead of `0x0811`.

### Trololo

Here is a collection of encountered funny bugs.

_When you forget to clear FrameBuffer_

<img src="http://i64.tinypic.com/5b1cw6.jpg" alt="When you forget to clear FrameBuffer" width="300" height="400" >
