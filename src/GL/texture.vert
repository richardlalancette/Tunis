R"(
/**
 * MIT License
 *
 * Copyright (c) 2018 Matt Chiasson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#if defined(GL_ES)
precision highp float;
#endif

uniform vec2 u_viewSize;

attribute vec2 a_position;
attribute vec2 a_texcoord;
attribute vec2 a_texoffset;
attribute vec2 a_texsize;
attribute vec4 a_color;

varying vec2 v_texcoord;
varying vec2 v_texoffset;
varying vec2 v_texsize;
varying vec4 v_color;

void main()
{
    v_texcoord   = a_texcoord;
    v_texoffset  = a_texoffset;
    v_texsize    = a_texsize;
    v_color      = a_color;
    gl_Position  = vec4(2.0 * a_position.x / u_viewSize.x - 1.0,
                        1.0 - 2.0 * a_position.y / u_viewSize.y,
                        0,
                        1);
};

)"
