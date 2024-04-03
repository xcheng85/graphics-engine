#include <glad/glad.h>
#include "pch.h"

namespace ge::resource::io
{
    // for one stage

    // input: path + ...args
    // profile: core vs compatible
    // version:
    // passing macro
    // c-style include

    // output:
    // processed

    // how shader stages form a shader program ?

    // one actor: has box component
    // has box renderer subsystem.
    // box component has the vs and fs shader

    // box renderer subsystem will iterate all the box component and compose the pipeline.

    // this is the component and subsystem

    // single responsibility:
    // 1. parse cli args
    // 2. parse include preprocessor
    // 3.
    class ShaderStageReader
    {
        // typedef std::map<std::string, GLuint> names_map;
        // typedef std::map<GLuint, std::string> files_map;
        class CliArgsParser
        {
            enum GL_PROFILE
            {
                CORE,
                COMPATIBILITY
            };

        public:
            CliArgsParser(std::string const &fp, std::string const &args);
            void parseArguments(std::string const &args);
            GE_INLINE const int &GLVersion() const { return this->_glVersion; }
            GE_INLINE const std::string &GLProfile() const { return this->_glProfile; }
            // combine to one line with \n; easy for stringstream write
            std::string MacroDefines() const;
            GE_INLINE const std::vector<std::string> &IncludeSearchPaths() const { return this->_includeSearchPaths; }

        private:
            std::string _glProfile{"core"};
            int _glVersion{-1};
            std::vector<std::string> _defines;
            std::vector<std::string> _includeSearchPaths;
        };

        // // c++ functor
        class IncludeParserFunctor
        {
        public:
            // dependencies injection + pipeline pattern
            std::string operator()(CliArgsParser const &cliArgsParser, std::string const &fp) const;

        private:
            std::string parseFileNameFromInclude(std::string const &line, std::size_t const &includeOffsetInLine) const;
        };

    public:
        ~ShaderStageReader();

        GLuint create(GLenum shaderStageType, std::string const &fp, std::string const &args = std::string());

    private:
    };
}