# Continuous Game of Life
Project aiming to implement the Continuous Game of Life leveraging the power of Shaders.

## Done
- Game of Life (with Compute and Fragment Shaders)
- Game of Life with a Kernel (Fragment Shader)
- Game of Life with a Kernel+Growth method (Fragment shader)
- Larger than Life (Fragment Shader)

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
You can run `./max_resolution` to find the maximum output resolution your GPU supports.