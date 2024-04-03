// #include "shader_stage.h"
// #include "core/file_io.h"

// extern std::string getShaderDirectory();
// extern std::string loadFile(std::string const &fpath);

// namespace ge::resource::io
// {
//     ShaderStageReader::CliArgsParser::CliArgsParser(std::string const &fp, std::string const &args)
//     {
//         // unix vs windows
//         std::size_t pathOffset = fp.find_last_of("/");
//         std::string path = fp.substr(0, pathOffset + 1);

//         // automatically added to search includes
//         this->_includeSearchPaths.push_back(path);
//         this->parseArguments(args);
//     }

//     void ShaderStageReader::CliArgsParser::parseArguments(std::string const &args)
//     {
//         // "--version 150 --profile core --define GEN_ERROR"
//         std::stringstream sstream(args);
//         std::string currentParam;

//         while (!sstream.eof())
//         {
//             sstream >> currentParam;

//             std::size_t foundMacroDefine = currentParam.find("-D");
//             std::size_t foundInclude = currentParam.find("-I");

//             if (foundMacroDefine != std::string::npos)
//                 this->_defines.emplace_back(currentParam.substr(2, currentParam.size() - 2));
//             else if (foundInclude != std::string::npos)
//                 this->_includeSearchPaths.push_back(getShaderDirectory() + currentParam.substr(2, currentParam.size() - 2));
//             else if (currentParam == "--define")
//             {
//                 std::string define;
//                 sstream >> define;
//                 this->_defines.push_back(define);
//             }
//             else if ((currentParam == "--version") || (currentParam == "-v"))
//                 sstream >> _glVersion;
//             else if ((currentParam == "--profile") || (currentParam == "-p"))
//                 sstream >> _glProfile;
//             else if (currentParam == "--include")
//             {
//                 std::string include;
//                 sstream >> include;
//                 this->_includeSearchPaths.push_back(getShaderDirectory() + include);
//             }
//         }
//     }

//     std::string ShaderStageReader::CliArgsParser::MacroDefines() const
//     {
//         std::string definesCombined;
//         std::string s("#define ");
//         std::string e("\n");
//         for (const auto &define : _defines)
//             definesCombined += s + define + e;
//         return definesCombined;
//     }

//     std::string ShaderStageReader::IncludeParserFunctor::operator()(CliArgsParser const &cliArgsParser, std::string const &fp) const
//     {
//         std::string glslSource = loadFile(fp);
//         GE_ASSERT(!glslSource.empty());

//         std::stringstream sstream;
//         sstream << glslSource;
//         std::string line, glslSourceAfterProcessInclude;

//         // Handle command line version and profile arguments
//         if (cliArgsParser.GLVersion() != -1)
//         {
//             glslSourceAfterProcessInclude += std::format("#version %d %s\n", cliArgsParser.GLVersion(), cliArgsParser.GLProfile());
//         }
//         // Handle command line defines
//         glslSourceAfterProcessInclude += cliArgsParser.MacroDefines();

//         // read glsl line by line
//         while (std::getline(sstream, line))
//         {
//             std::size_t offset = 0;

//             // GL Version
//             offset = line.find("#version");
//             if (offset != std::string::npos)
//             {
//                 std::size_t commentOffset = line.find("//");
//                 if (commentOffset != std::string::npos && commentOffset < offset)
//                 {
//                     // #version was commented out
//                     // #version
//                     continue;
//                 }

//                 if (cliArgsParser.GLVersion() == -1)
//                 {
//                     // GLVersion is not provided in the CLI args.
//                     // insert to the first line of glsl
//                     glslSourceAfterProcessInclude = line + std::string("\n") + glslSourceAfterProcessInclude;
//                 }
//                 continue;
//             }

//             // Include preprocessor
//             offset = line.find("#include");
//             if (offset != std::string::npos)
//             {
//                 std::size_t commentOffset = line.find("//");
//                 if (commentOffset != std::string::npos && commentOffset < offset)
//                     continue;
//                 // fbo-splash.glsl
//                 std::string includeFileName = parseFileNameFromInclude(line, offset);
//                 const auto &allIncludeSearchPaths = cliArgsParser.IncludeSearchPaths();
//                 for (const auto &includePath : allIncludeSearchPaths)
//                 {
//                     std::string fp{includePath + includeFileName};
//                     std::string includeGLSL = loadFile(fp);
//                     if (!includeGLSL.empty())
//                     {
//                         // the first match win, may not be th
//                         glslSourceAfterProcessInclude += includeGLSL;
//                         break;
//                     }
//                 }
//                 continue;
//             }
//             // base case, just copy to the dest
//             glslSourceAfterProcessInclude += line + "\n";
//         }
//         return glslSourceAfterProcessInclude;
//     }

//     std::string ShaderStageReader::IncludeParserFunctor::parseFileNameFromInclude(
//         std::string const &line, std::size_t const &includeOffsetInLine) const
//     {
//         std::string Result;
//         // #include "fbo-splash.glsl"
//         std::string::size_type firstQuote = line.find("\"", includeOffsetInLine);
//         std::string::size_type secondQuote = line.find("\"", firstQuote + 1);
//         return line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
//     }

//     GLuint ShaderStageReader::create(GLenum shaderStageType, std::string const &fp, std::string const &args)
//     {
//         GE_ASSERT(!fp.empty());
//         // pipeline process
//         // result is in state
//         CliArgsParser argsParser{fp, args};
//         std::string glslSource = IncludeParserFunctor()(argsParser, fp);
//         GE_ASSERT(!glslSource.empty());
//         // debugging the result
//         std::cout << std::format("{}\n", glslSource);

//         GLuint handle = glCreateShader(shaderStageType);
//         const auto ptr = glslSource.c_str();
//         glShaderSource(handle, 1, &ptr, NULL);
//         glCompileShader(handle);

//         // std::pair<files_map::iterator, bool> ResultFiles = this->ShaderFiles.insert(std::make_pair(Name, Filename));
//         // assert(ResultFiles.second);
//         // std::pair<names_map::iterator, bool> ResultNames = this->ShaderNames.insert(std::make_pair(Filename, Name));
//         // assert(ResultNames.second);
//         // std::pair<names_map::iterator, bool> ResultChecks = this->PendingChecks.insert(std::make_pair(Filename, Name));
//         // assert(ResultChecks.second);

//         return handle;
//     }

// }