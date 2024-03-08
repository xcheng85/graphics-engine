# graphics-engine

## submodule
```shell
git submodule add https://github.com/glfw/glfw.git external/glfw
```

## glfw

## glm

```shell
cd /home/xiao/Downloads/glm-1.0.1

cmake \
    -DGLM_BUILD_TESTS=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -B build .
cmake --build build -- all
sudo cmake --build build -- install

```