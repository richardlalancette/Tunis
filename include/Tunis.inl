#include <Tunis.h>

namespace tunis
{

inline void Context::beginPath()
{
    currentPath.reset();
}

inline void Context::closePath()
{
    currentPath.closePath();
}

inline void Context::moveTo(float x, float y)
{
    currentPath.moveTo(std::move(x), std::move(y));
}

inline void Context::lineTo(float x, float y)
{
    currentPath.lineTo(std::move(x), std::move(y));
}

inline void Context::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    currentPath.bezierCurveTo(std::move(cp1x), std::move(cp1y), std::move(cp2x), std::move(cp2y), std::move(x), std::move(y));
}

inline void Context::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    currentPath.quadraticCurveTo(std::move(cpx), std::move(cpy), std::move(x), std::move(y));
}

inline void Context::arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise)
{
    currentPath.arc(std::move(x), std::move(y), std::move(radius), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    currentPath.arcTo(std::move(x1), std::move(y1), std::move(x2), std::move(y2), std::move(radius));
}

inline void Context::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    currentPath.ellipse(std::move(x), std::move(y), std::move(radiusX), std::move(radiusY), std::move(rotation), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::rect(float x, float y, float width, float height)
{
    currentPath.rect(std::move(x), std::move(y), std::move(width), std::move(height));
}

inline void Context::fill(FillRule fillRule)
{
    fill(currentPath, std::move(fillRule));
}

inline void Context::stroke()
{
    stroke(currentPath);
}


}

