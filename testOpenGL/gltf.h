#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "memory.h"
#include "vector.h"
#include "matrix.h"

using namespace std;

namespace IO::GLTF
{
    enum PROPERTY_KEY
    {
        scene,
        scenes,
        asset,       // metadata
        buffers,     // interleaved data
        bufferViews, // how to inteprete buffer, padding bytes
        nodes,
        meshes,    // indexed and no-indexed, indices field
        accessors, // inteprete data with bufferview
        materials,
        textures,
        images,
        samplers,
        skins,
        animations,
    };

    using CustomInt32Vector = std::vector<int32_t, HeapAllocator<int32_t>>;

    // The following struct maps gltf specs.
    struct Scene
    {
        Scene() = delete;
        Scene(IMemoryAllocator *customMemoryAllocator) : nodes(customMemoryAllocator)
        {
        }
        // vector with custom allocator
        CustomInt32Vector nodes;
    };

    struct Node
    {
        // index of mesh in mesh-container
        int32_t mesh;
    };

    struct Primitive
    {
        struct Attribute
        {
            // key: "POSITION", "NORMAL"
            std::string key;
            int32_t accessorIdx;
        };
        using CustomAttributeVector = std::vector<Attribute, HeapAllocator<Attribute>>;

        Primitive() = delete;
        Primitive(IMemoryAllocator *customMemoryAllocator) : attributes(customMemoryAllocator)
        {
        }

        CustomAttributeVector attributes;
        // The index of the accessor that contains the vertex indices
        int32_t indices;
        // 0 POINTS
        // 1 LINES
        // 2 LINE_LOOP
        // 3 LINE_STRIP
        // 4 TRIANGLES
        // 5 TRIANGLE_STRIP
        // 6 TRIANGLE_FAN
        int32_t mode{4};
    };

    using CustomPrimitiveVector = std::vector<Primitive, HeapAllocator<Primitive>>;

    struct Mesh
    {
        Mesh() = delete;
        Mesh(IMemoryAllocator *customMemoryAllocator) : primitives(customMemoryAllocator)
        {
        }
        CustomPrimitiveVector primitives;
    };

    using CustomString = std::basic_string<char, std::char_traits<char>, HeapAllocator<char>>;
    struct Buffer
    {
        Buffer() = delete;
        Buffer(IMemoryAllocator *customMemoryAllocator) : uri(customMemoryAllocator)
        {
        }
        int32_t byteLength;
        // consider its size, if buffer baked in
        CustomString uri;
    };

    struct BufferView
    {
        // https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#reference-bufferview
        enum Target : int32_t
        {
            ARRAY_BUFFER = 34962,
            ELEMENT_ARRAY_BUFFER = 34963
        };

        BufferView() = default;

        int32_t buffer;        // index of buffer in buffers
        int32_t byteLength;    // how many bytes this buffer view contain in the buffer
        int32_t byteOffset{0}; // starting byte position
        int32_t byteStride;    // stride in bytes
        int32_t target;        // usage of this buffer: vb or ib
    };

    using CustomFloatVector = std::vector<float, HeapAllocator<float>>;

    struct Accessor
    {
        enum ComponentType : int32_t
        {
            BYTE = 5120,
            UNSIGNED_BYTE = 5121,
            SHORT = 5122,
            UNSIGNED_SHORT = 5123,
            UNSIGNED_INT = 5125,
            FLOAT = 5126
        };

        enum Type
        {
            SCALAR,
            VEC2,
            VEC3,
            VEC4,
            MAT2,
            MAT3,
            MAT4,
        };

        Accessor() = delete;
        Accessor(IMemoryAllocator *customMemoryAllocator) : max(customMemoryAllocator),
                                                            min(customMemoryAllocator)
        {
        }

        int32_t bufferView;    // index of bufferView array
        int32_t byteOffset{0}; // relative of buffer view, pay attention to the difference
        // required field
        int32_t componentType;
        bool normalized{false};
        // required field
        int32_t count; // number of elements
        // required field
        Type type;
        // length depends on type: 1, 2, 3, 4
        CustomFloatVector max;
        CustomFloatVector min;
    };

    // metadata
    struct Asset
    {
        Asset() = delete;
        Asset(IMemoryAllocator *customMemoryAllocator) : version(customMemoryAllocator)
        {
        }
        CustomString version;
    };

    using CustomSceneVector = std::vector<Scene, HeapAllocator<Scene>>;
    using CustomNodeVector = std::vector<Node, HeapAllocator<Node>>;
    using CustomMeshVector = std::vector<Mesh, HeapAllocator<Mesh>>;
    using CustomBufferVector = std::vector<Buffer, HeapAllocator<Buffer>>;
    using CustomBufferViewVector = std::vector<BufferView, HeapAllocator<BufferView>>;
    using CustomAccessorVector = std::vector<Accessor, HeapAllocator<Accessor>>;

    // for serialization and deserialization
    struct GLTF
    {
        GLTF() = delete;
        GLTF(IMemoryAllocator *customMemoryAllocator)
            : scenes(customMemoryAllocator),
              nodes(customMemoryAllocator),
              meshes(customMemoryAllocator),
              buffers(customMemoryAllocator),
              bufferViews(customMemoryAllocator),
              accessors(customMemoryAllocator),
              asset(customMemoryAllocator)
        {
        }
        uint32_t scene;           // active scene
        CustomSceneVector scenes; // all scenes
        CustomNodeVector nodes;
        CustomMeshVector meshes;
        CustomBufferVector buffers;
        CustomBufferViewVector bufferViews;
        CustomAccessorVector accessors;
        Asset asset;
    };

    class GLTFParser
    {
    public:
        GLTFParser(IMemoryAllocator *customMemoryAllocator);
        GLTF parse(const std::string &path);

    private:
        IMemoryAllocator *_customMemoryAllocator;
    };
}