## Buffer Object in Modern OpenGL
Pure in-OpenGL buffers: In-direct rendering, transform feedback, compute shader

static buffer: 
//static buffer: set flags​ to 0 and use data​ as the initial upload

immutable vs mutable storage

## Update Buffer method1: gl*BufferSubData 
can be wasteful in performance
1. bind
2. gl*BufferSubData
3. unbind

## Update Buffer method2: mapping
get a pointer to the buffer object's storage and write directly to it.
1. bind
2. map
3. unmap

## Buffer for multi-draw
multiple meshes into one buffer

## Buffer for Indrect Rendering
Binding target for buffer: GL_DRAW_INDIRECT_BUFFER

## Multi-draw + Indrect

who fills the buffer ? 

This could be a Compute Shader, a specially designed Geometry Shader coupled with Transform Feedback, or an OpenCL/CUDA process.


The indirect rendering functions take their data from the buffer currently bound to the GL_DRAW_INDIRECT_BUFFER binding

Indirect rendering



## No-DSA Texture
```shell
#1). create 2). bind 3). allocate 4). upload
# 1: glGenTextures(1,&tex);
# 2: glBindTexture(GL_TEXTURE_2D,tex);
# 3: glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, w,h, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
# 4: glTexSubImage2D(GL_TEXTURE_2D, level, x,y,w,h, GL_BGRA, GL_UNSIGNED_BYTE, data);
```
## DSA Texture
no bind needed
```shell
#1: glCreateTextures(GL_TEXTURE_2D,1,&tex);
#2: // glBindTextureUnit(unit, tex);
#3: glTextureStorage2D(tex, levels, GL_RGBA8, w,h);
#4: glTextureSubImage2D(tex, level, x,y,w,h, GL_BGRA, GL_UNSIGNED_BYTE, data);
```

## glTextureStorage2D vs glTexImage2D
glTextureStorage2D:

```c++
for(int i=0; i<levels; i++) 
   glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, max(1,w>>i),max(1,h>>i), 0, GL_BGRA,GL_UNSIGNED_BYTE, nullptr);
```

## Bindless Texture
glfwExtensionSupported("ARB_bindless_texture")

Texture handles are created 

GLuint64 glGetTextureHandleARB(GLuint texture​);
GLuint64 glGetTextureSamplerHandleARB(GLuint texture​, GLuint sampler​);

Image handles are created using glGetImageHandleARB.
GLuint64 glGetImageHandleARB( GLuint texture​, GLint level​, GLboolean layered​, GLint layer​, GLenum format​ );

Handle residency

uvec2 Diffuse; 64bit as address

host app need to pass this value to the fragment shader through:
1. uniform storage buffer
2. shader storage buffer



## Scene Graph
object-oriented sg vs data-oriented sg

### Why object-oriented bad ? 
pointer, scattered, not cache-friendly.
recursion, 


memory layout


entity: stable struct, only save index(offset)

struct Entity
{
	int parent_;

	// first child for a node (or -1)
	int firstChild_;

	// next sibling for a node (or -1)
	int nextSibling_;
	
    // last added node (or -1)
	int lastSibling_;

	// cached node level
	int level_;
};


vector: localTransform
vector: worldTransform: recalculate from concatenation

// mapping nodeIndex -> meshIndex
std::unordered_map<uint32_t, uint32_t> meshes_;

## Bindless Texture & Indirect Rendering