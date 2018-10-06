#ifndef TUNISSHADERPROGRAM_H
#define TUNISSHADERPROGRAM_H

#include <TunisGL.h>

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


namespace tunis
{
    namespace detail
    {

        class Shader
        {
        public:

            Shader(const char * name);
            virtual ~Shader();
            operator GLuint() const;
            bool status() const;
            const char *name() const;

        protected:

            void compile(GLenum type, const char *source, int len);

            const char *shaderName;
            GLuint shaderId = 0;
            GLint compileStatus = GL_FALSE;
        };

        class ShaderVertTexture : public Shader
        {
        public: ShaderVertTexture();
        };


        class ShaderFragTexture : public Shader
        {
        public: ShaderFragTexture();
        };

        class ShaderVertGradientRadial : public Shader
        {
        public: ShaderVertGradientRadial();
        };

        class ShaderFragGradientRadial : public Shader
        {
        public: ShaderFragGradientRadial();
        };

        class ShaderProgram
        {
        public:

            ShaderProgram(const Shader &vert, const Shader &frag, const char *name);
            virtual ~ShaderProgram();
            operator GLuint() const;
            bool status() const;
            const char *name() const;
            void useProgram();

            void setViewSizeUniform(int32_t width, int32_t height);

            virtual void enableVertexAttribArray() = 0;
            virtual void disableVertexAttribArray() = 0;

        protected:

            const char* programName;
            GLuint programId = 0;
            GLint linkStatus = GL_FALSE;

        private:

            // uniform locations
            GLint u_viewSize = 0;

            // uniform values
            int32_t viewWidth = 0;
            int32_t viewHeight = 0;

        };

        class ShaderProgramTexture : public ShaderProgram
        {
        public:
            ShaderProgramTexture();

            virtual void enableVertexAttribArray() override;
            virtual void disableVertexAttribArray() override;

        private:

            // attribute locations
            GLint a_position = 0;
            GLint a_texcoord = 0;
            GLint a_color = 0;

        };

        union UniformBlock
        {
            struct RadialGradient
            {
                glm::vec2 u_focal;
                glm::vec2 u_dt;
                float u_r0;
                float u_dr;
                float u_a;
                float u_colorStopCount;
                glm::vec4 u_offset;
                glm::vec4 u_color[4];
            } radialGradient;
        };

        class ShaderProgramGradient : public ShaderProgram
        {
        public:

            ShaderProgramGradient();

            virtual void enableVertexAttribArray() override;
            virtual void disableVertexAttribArray() override;

            void setUniforms(const UniformBlock &uniforms);

        private:

            // attribute locations
            GLint a_position = 0;

            // uniform lacations
            GLint u_uniforms = 0;
        };
    }

}

#include "TunisShaderProgram.inl"

#endif // TUNISSHADERPROGRAM_H
