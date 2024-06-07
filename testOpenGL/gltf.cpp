#include <filesystem>
#include <assert.h>
#include <fstream>
#include "json.hpp"
#include "gltf.h"

using json = nlohmann::json;

namespace IO::GLTF
{
    // unordered_map cannot be constexpr
    static std::unordered_map<std::string, PROPERTY_KEY> sPropertyKeyLookup{
        {"scene", PROPERTY_KEY::scene},
        {"scenes", PROPERTY_KEY::scenes},
        {"asset", PROPERTY_KEY::asset},
        {"buffers", PROPERTY_KEY::buffers},
        {"bufferViews", PROPERTY_KEY::bufferViews},
        {"nodes", PROPERTY_KEY::nodes},
        {"meshes", PROPERTY_KEY::meshes},
        {"accessors", PROPERTY_KEY::accessors},
        {"materials", PROPERTY_KEY::materials},
        {"textures", PROPERTY_KEY::textures},
        {"images", PROPERTY_KEY::images},
        {"samplers", PROPERTY_KEY::samplers},
        {"skins", PROPERTY_KEY::skins},
        {"animations", PROPERTY_KEY::animations},
    };

    // unordered_map cannot be constexpr
    static std::unordered_map<std::string, Accessor::Type> sAccessorTypeLookup{
        {"SCALAR", Accessor::Type::SCALAR},
        {"VEC2", Accessor::Type::VEC2},
        {"VEC3", Accessor::Type::VEC3},
        {"VEC4", Accessor::Type::VEC4},
        {"MAT2", Accessor::Type::MAT2},
        {"MAT3", Accessor::Type::MAT3},
        {"MAT4", Accessor::Type::MAT4},
    };

    // forward declaration
    void parseScene(json &json_scene, Scene &scene, IMemoryAllocator *customMemoryAllocator);
    void parseInt(json &json_int, int32_t &i32);
    void parseNode(json &json_node, Node &node, IMemoryAllocator *customMemoryAllocator);

    void parseInt(json &json_int, int32_t &i32)
    {
        i32 = json_int.template get<int32_t>();
    }

    void parseInt32Array(json &json_intarray, CustomInt32Vector &i32Vector, IMemoryAllocator *customMemoryAllocator)
    {
        auto intNum = json_intarray.size();
        i32Vector.assign(intNum, 0);
        for (size_t i = 0; i < intNum; ++i)
        {
            i32Vector[i] = json_intarray[i];
            std::cout << i32Vector[i] << "\n";
        }
        // for string based kv pair
        // for (auto &[key, value] : json_intarray.items())
        // {
        //     i32Vector[key] = value;
        // }
    }

    void parseFloat32Array(json &json_f32array, CustomFloatVector &f32Vector, IMemoryAllocator *customMemoryAllocator)
    {
        auto float32Num = json_f32array.size();
        f32Vector.assign(float32Num, 0.f);
        for (size_t i = 0; i < float32Num; ++i)
        {
            f32Vector[i] = json_f32array[i];
            std::cout << f32Vector[i] << "\n";
        }
    }

    void parseString(json &json_string, CustomString &str)
    {
        // assign
        std::string value = json_string.template get<std::string>();
        std::cout << value << "\n";
        str.assign(value.size(), '0');
        // no memory resize
        str = value;
    }

    void parseBoolean(json &json_boolean, bool &b)
    {
        b = json_boolean.template get<bool>();
    }

    void parseMat4x4f(json &json_mat4x4f, mat4x4f &m)
    {
        const auto numFloats = json_mat4x4f.size();
        assert(numFloats == 16);

        size_t c, r;
        for (size_t i = 0; i < numFloats; ++i)
        {
            c = i / 4;
            r = i % 4;
            m.data[r][c] = json_mat4x4f.at(i);
        }
    }

    void parseVec3f(json &json_vec3f, vec3f &v)
    {
        const auto numFloats = json_vec3f.size();
        assert(numFloats == 3);
        v.data[0] = json_vec3f.at(0);
        v.data[1] = json_vec3f.at(1);
        v.data[2] = json_vec3f.at(2);
    }

    void parseVec4f(json &json_vec4f, vec4f &v)
    {
        const auto numFloats = json_vec4f.size();
        assert(numFloats == 4);
        v.data[0] = json_vec4f.at(0);
        v.data[1] = json_vec4f.at(1);
        v.data[2] = json_vec4f.at(2);
        v.data[3] = json_vec4f.at(3);
    }

    // hide impl detail from the declaration
    void parseScenes(json &json_scenes, GLTF &gltf, IMemoryAllocator *customMemoryAllocator)
    {
        auto scenesNum = json_scenes.size();
        // memory allocation
        gltf.scenes.assign(scenesNum, Scene(customMemoryAllocator));
        for (size_t i = 0; i < scenesNum; ++i)
        {
            parseScene(json_scenes[i], gltf.scenes[i], customMemoryAllocator);
        }
    }
    // vector of int (node index)
    void parseScene(json &json_scene, Scene &scene, IMemoryAllocator *customMemoryAllocator)
    {
        parseInt32Array(json_scene["nodes"], scene.nodes, customMemoryAllocator);
    }

    void parseNodes(json &json_nodes, GLTF &gltf, IMemoryAllocator *customMemoryAllocator)
    {
        auto nodesNum = json_nodes.size();
        gltf.nodes.assign(nodesNum, Node{customMemoryAllocator});

        for (size_t i = 0; i < nodesNum; ++i)
        {
            parseNode(json_nodes[i], gltf.nodes[i], customMemoryAllocator);
        }
    }

    void parseNode(json &json_node, Node &node, IMemoryAllocator *customMemoryAllocator)
    {
        // mesh is optional, index to mesh container
        if (json_node.contains("mesh"))
        {
            parseInt(json_node["mesh"], node.mesh);
        }
        if (json_node.contains("children"))
        {
            parseInt32Array(json_node["children"], node.children, customMemoryAllocator);
        }
        if (json_node.contains("matrix"))
        {
            mat4x4f tmp;
            parseMat4x4f(json_node["matrix"], tmp);
            node.matrix = std::make_optional<mat4x4f>(tmp);
        }
        if (json_node.contains("translation"))
        {
            vec3f tmp;
            parseVec3f(json_node["translation"], tmp);
            node.translation = std::make_optional<vec3f>(tmp);
        }
        if (json_node.contains("scale"))
        {
            vec3f tmp;
            parseVec3f(json_node["scale"], tmp);
            node.scale = std::make_optional<vec3f>(tmp);
        }
        if (json_node.contains("rotation"))
        {
            vec4f tmp;
            parseVec4f(json_node["rotation"], tmp);
            node.rotationQuat = std::make_optional<vec4f>(tmp);
        }
    }
    // attributes is required
    // indices: optional, only applies to index-based rendering
    void parsePrimitive(json &json_primitive, Primitive &primitive)
    {
        json attributes = json_primitive["attributes"];
        const auto attributeNum = attributes.size();

        primitive.attributes.assign(attributeNum, Primitive::Attribute());
        // each kv pair
        // key is semantic: position, normal, texCoord
        // value is index of accesor
        size_t attributeIdx{0};
        for (const auto &kv : attributes.items())
        {
            auto &attribute = primitive.attributes[attributeIdx++];
            attribute.key = kv.key();
            attribute.accessorIdx = kv.value();
            std::cout << format("{}:{}\n", attribute.key, attribute.accessorIdx);
        }

        if (json_primitive.contains("indices"))
        {
            parseInt(json_primitive["indices"], primitive.indices);
        }
    }

    void parsePrimitives(json &json_primitives, CustomPrimitiveVector &primitiveVector, IMemoryAllocator *customMemoryAllocator)
    {
        auto primitivesNum = json_primitives.size();
        primitiveVector.assign(primitivesNum, Primitive{customMemoryAllocator});
        for (size_t i = 0; i < primitivesNum; ++i)
        {
            parsePrimitive(json_primitives[i], primitiveVector[i]);
        }
    }

    void parseMesh(json &json_mesh, Mesh &mesh, IMemoryAllocator *customMemoryAllocator)
    {
        // mesh is optional, index to mesh container
        if (json_mesh.contains("primitives"))
        {
            parsePrimitives(json_mesh["primitives"], mesh.primitives, customMemoryAllocator);
        }
    }

    void parseMeshes(json &json_meshes, GLTF &gltf, IMemoryAllocator *customMemoryAllocator)
    {
        auto meshesNum = json_meshes.size();
        gltf.meshes.assign(meshesNum, Mesh{customMemoryAllocator});

        for (size_t i = 0; i < meshesNum; ++i)
        {
            parseMesh(json_meshes[i], gltf.meshes[i], customMemoryAllocator);
        }
    }

    // byteLength: required
    void parseBuffer(json &json_buffer, Buffer &buffer, IMemoryAllocator *customMemoryAllocator)
    {
        if (json_buffer.contains("byteLength"))
        {
            parseInt(json_buffer["byteLength"], buffer.byteLength);
        }
        if (json_buffer.contains("uri"))
        {
            parseString(json_buffer["uri"], buffer.uri);
        }
    }

    void parseBuffers(json &json_buffers, GLTF &gltf, IMemoryAllocator *customMemoryAllocator)
    {
        const auto buffersNum = json_buffers.size();
        gltf.buffers.assign(buffersNum, Buffer{customMemoryAllocator});

        for (size_t i = 0; i < buffersNum; ++i)
        {
            parseBuffer(json_buffers[i], gltf.buffers[i], customMemoryAllocator);
        }
    }

    void parseBufferView(json &json_bufferview, BufferView &bufferView, IMemoryAllocator *customMemoryAllocator)
    {
        if (json_bufferview.contains("buffer"))
        {
            parseInt(json_bufferview["buffer"], bufferView.buffer);
        }
        if (json_bufferview.contains("byteLength"))
        {
            parseInt(json_bufferview["byteLength"], bufferView.byteLength);
        }
        if (json_bufferview.contains("byteOffset"))
        {
            parseInt(json_bufferview["byteOffset"], bufferView.byteOffset);
        }
        if (json_bufferview.contains("byteStride"))
        {
            parseInt(json_bufferview["byteStride"], bufferView.byteStride);
        }
        if (json_bufferview.contains("target"))
        {
            parseInt(json_bufferview["target"], bufferView.target);
        }
    }

    void parseBufferViews(json &json_bufferviews, GLTF &gltf, IMemoryAllocator *customMemoryAllocator)
    {
        const auto bufferViewsNum = json_bufferviews.size();
        gltf.bufferViews.assign(bufferViewsNum, BufferView{});

        for (size_t i = 0; i < bufferViewsNum; ++i)
        {
            parseBufferView(json_bufferviews[i], gltf.bufferViews[i], customMemoryAllocator);
        }
    }

    void parseAccessor(json &json_accessor, Accessor &accessor, IMemoryAllocator *customMemoryAllocator)
    {
        if (json_accessor.contains("bufferView"))
        {
            parseInt(json_accessor["bufferView"], accessor.bufferView);
        }
        if (json_accessor.contains("byteOffset"))
        {
            parseInt(json_accessor["byteOffset"], accessor.byteOffset);
        }
        if (json_accessor.contains("componentType"))
        {
            parseInt(json_accessor["componentType"], accessor.componentType);
        }
        if (json_accessor.contains("normalized"))
        {
            parseBoolean(json_accessor["normalized"], accessor.normalized);
        }
        if (json_accessor.contains("count"))
        {
            parseInt(json_accessor["count"], accessor.count);
        }
        if (json_accessor.contains("type"))
        {
            assert(sAccessorTypeLookup.count(json_accessor["type"]));
            accessor.type = sAccessorTypeLookup[json_accessor["type"]];
        }
        if (json_accessor.contains("max"))
        {
            parseFloat32Array(json_accessor["max"], accessor.max, customMemoryAllocator);
        }
        if (json_accessor.contains("min"))
        {
            parseFloat32Array(json_accessor["min"], accessor.min, customMemoryAllocator);
        }
    }

    void parseAccessors(json &json_accessors, GLTF &gltf, IMemoryAllocator *customMemoryAllocator)
    {
        const auto accessorsNum = json_accessors.size();
        gltf.accessors.assign(accessorsNum, Accessor{customMemoryAllocator});

        for (size_t i = 0; i < accessorsNum; ++i)
        {
            parseAccessor(json_accessors[i], gltf.accessors[i], customMemoryAllocator);
        }
    }

    void parseAsset(json &json_asset, GLTF &gltf, IMemoryAllocator *customMemoryAllocator)
    {
        if (json_asset.contains("version"))
        {
            parseString(json_asset["version"], gltf.asset.version);
        }
    }

    GLTFParser::GLTFParser(IMemoryAllocator *customMemoryAllocator) : _customMemoryAllocator{customMemoryAllocator}
    {
    }

    GLTF GLTFParser::parse(const std::string &path)
    {
        assert(std::filesystem::exists(path));
        // raii
        std::ifstream inputFile{path, ios_base::in};
        assert(inputFile.good());
        inputFile.seekg(0, inputFile.end);
        size_t fileByteSize = inputFile.tellg();
        inputFile.seekg(0, inputFile.beg);

        // c-string '\0' at the end (in case)
        auto bufferPtr = static_cast<char *>(_customMemoryAllocator->allocate(fileByteSize + 1, 1));
        inputFile.read(bufferPtr, fileByteSize);
        inputFile.close();

        // do sth with bufferPtr
        // requires char*
        // .json CRLF
        // bufferPtr size is larger than fileByteSize, mannually cut it off.
        bufferPtr[fileByteSize] = 0;

        json gltf = json::parse(bufferPtr);
        std::cout << gltf.dump(4) << std::endl;

        GLTF res{_customMemoryAllocator};
        for (auto properties : gltf.items())
        {
            const auto &k = properties.key();
            switch (IO::GLTF::sPropertyKeyLookup[k])
            {
            case IO::GLTF::PROPERTY_KEY::scene:
            {
                // entryPoint of gltf
                // type deduction based on the default value 0
                res.scene = gltf.value(k, 0);
            }
            break;
            case IO::GLTF::PROPERTY_KEY::scenes:
            {
                parseScenes(gltf[k], res, _customMemoryAllocator);
            }
            break;
            case IO::GLTF::PROPERTY_KEY::nodes:
            {
                parseNodes(gltf[k], res, _customMemoryAllocator);
            }
            break;
            case IO::GLTF::PROPERTY_KEY::meshes:
            {
                parseMeshes(gltf[k], res, _customMemoryAllocator);
            }
            break;
            case IO::GLTF::PROPERTY_KEY::buffers:
            {
                parseBuffers(gltf[k], res, _customMemoryAllocator);
            }
            break;
            case IO::GLTF::PROPERTY_KEY::bufferViews:
            {
                parseBufferViews(gltf[k], res, _customMemoryAllocator);
            }
            break;
            case IO::GLTF::PROPERTY_KEY::accessors:
            {
                parseAccessors(gltf[k], res, _customMemoryAllocator);
            }
            break;
            case IO::GLTF::PROPERTY_KEY::asset:
            {
                parseAsset(gltf[k], res, _customMemoryAllocator);
            }
            break;
            default:
                assert(false);
            }
        }

        // done with the memory
        _customMemoryAllocator->deallocate(bufferPtr);
        return res; // compiler optimization here
    }
}