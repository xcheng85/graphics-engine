# All about engine design

## 1. Subsystems

1. input system
2. log
3. scenegraph manager
4. dll lib loader 
5. material
6. mesh
7. Partical
8. Hud
9. Font
10. ResourceBackgroundQueue: async i/o
11. Memory pool/stack
12. Thread pool
13. file system

### Startup and shutdown
Dependencies between subsystem

is order of subsystem init mattering ? 


### memory management

pre-allocated memory block

stack-based (limitation last allocated block must be free first)

mono in pmr ? 

1. global aray from BSS segment. size limitation, 
on BSS, size can be very large

2. pointer. 

pool-based allocator
1. matrics
2. iterators
3. node in linked list
4. mesh instance

allocator with alignment requirments
return aligned memory block



## ecs
e: entity: 
c: component: business logic utilizing more generic sub-system
s: sub-system: rendering, animation, collision, audio. 

scene-manager is on top of them and be exposed to the application game loop


build on top of resource manager layer and low-level renderer

being consumed by components of entities

component: related engine subsystem: rendering animation, collistion, mesh instance 



## Rendering sub-system
Simplest use case



Material shader component for entity


Mesh submission
Pipeline state
Texture,






### Material and shaders
program
shaderstage
link
uniform handle


buffer binding
buffer submission
vao

bind depth test

mvp buffer
viewport
bind shaderprogram
bind vao
submit

## Resources & asset
shader 
