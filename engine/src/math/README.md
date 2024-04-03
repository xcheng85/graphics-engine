# 

## Plane

## Affine Transformation
rotation + translation 


## 1. World-to-camera transformation

1. camera position in world space
2. target in world space
3. up vector in world space

construct the 4x4 homogenous matrix
OpenGL right-handed: col-maj
Vulkan right-handed: col-maj
DirectX left-handed: row-maj
Metal left-handed: row-maj

## 2. projection matrix

MatrixPerspectiveFovLH {
    float fovVertical
    float r,
    float n
    float f,
}

### 2.1 to view space [-r, r] x [-1, 1] x [n, f]

do frustrum clipping

1. n
2. f
3. vertical fov: alpha
4. aspect ratio r: w/h matches image dimension of swapchain images

### calculate horizontal fov based on vertical fov + aspect ratio r

### calculate focal distancerom vertical fov

d = cot(alpha/2)

5. horizontal fov: beta
6. projection plane: d   focal distance , [-1, 1] x [-1, 1]


### 2.2 to NDC [-1, 1] x [-1, 1] x [0, 1]

depth buffer precision problem
most range is used nearby nearplane

### to minimize depth precision problem
near and far plane as close as possible

perspective divide/homogenouse divide


### homogenous clip space vs NDC
x,y is the same
clip space z: near to far
NDC z: [0, 1]


## Quaternions and rotation
multiplying a complex number p by a unit q = roation of p

polar coordinates to proof



## Clipping before Rasterization

positive, negative sign against a plane 

interpolation attributes
interpolation texture coordinates in perspective-correct way






## After rasterization
z buffer

bit of depth image


## Gourand shading: per-vertex shading

## Phong shading: per-fragment shading


## Culling: view volume, occulsion, backface culling

bounding volume of object 



## DS