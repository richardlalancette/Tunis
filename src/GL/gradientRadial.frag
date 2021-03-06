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

/*******************************************************************************
 * Special thanks to
 *
 *   Alex Sabourin
 *   Zack Dawson
 *
 * For helping me figure this out.
 ******************************************************************************/

#if defined(GL_ES)
precision highp float;
#endif

uniform vec2 u_viewSize;
uniform vec4 u_uniforms[7];

#define u_dt               u_uniforms[0].xy
#define u_focal            u_uniforms[0].zw
#define u_r0               u_uniforms[1].x
#define u_dr               u_uniforms[1].y
#define u_a                u_uniforms[1].z
#define u_colorStopCount   int(u_uniforms[1].w)
#define u_colorStop(INDEX) u_uniforms[2][INDEX]
#define u_color(INDEX)     u_uniforms[3+(INDEX)]

void main()
{
    float x = u_focal.x - gl_FragCoord.x;
    float y = u_focal.y - gl_FragCoord.y;
    float b = -2.0 * (y * u_dt.y + x * u_dt.x + u_r0 * u_dr);
    float c = x*x + y*y - u_r0*u_r0;
    float t = 1.0 - (0.5/u_a) * (-b + sqrt(b*b - 4.0*u_a*c));

    vec4 color = mix(u_color(0), u_color(1), clamp((t - u_colorStop(0))/(u_colorStop(1) - u_colorStop(0)), 0.0, 1.0));
    for (int i = 2; i < u_colorStopCount; ++i)
    {
        color = mix(color, u_color(i), clamp((t - u_colorStop(i-1))/(u_colorStop(i) - u_colorStop(i-1)), 0.0, 1.0));
    }

    gl_FragColor = color;

};

)"
