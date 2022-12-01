# Continuous Game of Life
Project aiming to implement the Continuous Game of Life leveraging the power of Shaders.
You can find more information about the Continuous Game of Life and especially Lenia here: https://chakazul.github.io/lenia.html

## Finished
- Game of Life (with Compute and Fragment Shaders)
- Game of Life with a Kernel (Fragment Shader)
- Game of Life with a Kernel+Growth method (Fragment shader)
- Larger than Life (Fragment Shader)
- Continuous States (Fragment Shader)
- Continuous Space (Fragment Shader)
- Smooth Kernel (Fragment Shader)
- Smooth Growth / Lenia (Fragment Shader)

# Upcoming / Planned
- Using Fast Fourier Transformation to improve performance when calculating the next states.

## How to build
```bash
mkdir cmake-build
cd cmake-build
cmake ..
make
```

## How to run
You can either run the executable in `cmake-build` or run one of the scripts in the benchmarks folder
to create a benchmark. 
You might want to run `./max_resolution` beforehand to find the maximum output resolution your GPU supports.