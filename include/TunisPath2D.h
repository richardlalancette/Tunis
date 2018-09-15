#ifndef TUNISPATH2D_H
#define TUNISPATH2D_H

#include <TunisTypes.h>
#include <TunisSOA.h>

#if defined(_MSC_VER)
#include <basetsd.h>
#include <stdlib.h>
#include <search.h>
typedef SSIZE_T ssize_t;
#endif
#include <MPE_fastpoly2tri.h>


namespace tunis
{

// forward declaration
class Context;

namespace detail {

// forward declaration
struct ContextPriv;

using MemPool = std::vector<uint8_t>;

enum PathCommandType {
    CLOSE = 0,
    MOVE_TO,
    LINE_TO,
    BEZIER_TO,
    QUAD_TO,
    ARC,
    ARC_TO,
    ELLIPSE,
    RECT,
};

struct PathCommandArray : public SoA<PathCommandType, float, float,float, float, float, float, float, float>
{
    inline PathCommandType &type(size_t idx) { return get<0>(idx); }
    inline float &param0(size_t idx) { return get<1>(idx); }
    inline float &param1(size_t idx) { return get<2>(idx); }
    inline float &param2(size_t idx) { return get<3>(idx); }
    inline float &param3(size_t idx) { return get<4>(idx); }
    inline float &param4(size_t idx) { return get<5>(idx); }
    inline float &param5(size_t idx) { return get<6>(idx); }
    inline float &param6(size_t idx) { return get<7>(idx); }
    inline float &param7(size_t idx) { return get<8>(idx); }
};

struct PointArray : public SoA<glm::vec2, glm::vec2, glm::vec2>
{
    inline glm::vec2 &pos(size_t idx) { return get<0>(idx); }
    inline glm::vec2 &dir(size_t idx) { return get<1>(idx); }
    inline glm::vec2 &exc(size_t idx) { return get<2>(idx); }
};

struct SubPathArray : public SoA<MPEPolyContext, MemPool, PointArray, PointArray, uint8_t >
{
    inline MPEPolyContext &polyContext(size_t idx) { return get<0>(idx); }
    inline MemPool &mempool(size_t idx) { return get<1>(idx); }
    inline PointArray &outerPoints(size_t idx) { return get<2>(idx); }
    inline PointArray &innerPoints(size_t idx) { return get<3>(idx); }
    inline uint8_t &closed(size_t idx) { return get<4>(idx); }
};

}

class Path2D : public RefCountedSOA<
        detail::PathCommandArray,
        detail::SubPathArray,
        uint8_t,
        glm::vec2,
        glm::vec2>
{
    inline detail::PathCommandArray &commands() { return get<0>(); }
    inline detail::SubPathArray &subPaths() { return get<1>(); }
    inline uint8_t &dirty() { return get<2>(); }
    inline glm::vec2 &boundTopLeft() { return get<3>(); }
    inline glm::vec2 &boundBottomRight() { return get<4>(); }

    friend Context;
    friend detail::ContextPriv;

public:

    /*!
     * \brief Path2D Default Constructor.
     */
    Path2D();

    /*!
     * \brief reset clears all the recorded subpath for starting over.
     */
    void reset();

    /*!
     * \brief closePath causes the point of the pen to move back to the start of
     * the current sub-path. It tries to add a straight line (but does not
     * actually draw it) from the current point to the start.
     *
     * \note If the shape has already been closed or has only one point, this
     * function does nothing.
     */
    void closePath();

    /*!
     * \brief moveTo moves the starting point of a new sub-path to the (x, y)
     * coordinates.
     *
     * \param x The x axis of the point.
     * \param y The y axis of the point.
     */
    void moveTo(float x, float y);

    /*!
     * \brief lineTo connects the last point in the sub-path to the x, y
     * coordinates with a straight line (but does not actually draw it).
     *
     * \param x The x axis of the coordinate for the end of the line.
     * \param y The y axis of the coordinate for the end of the line.
     */
    void lineTo(float x, float y);

    /*!
     * \brief bezierCurveTo adds a cubic Bézier curve to the path. It requires
     * three points. The first two points are control points and the third one
     * is the end point. The starting point is the last point in the current
     * path, which can be changed using moveTo() before creating the Bézier
     * curve.
     *
     * \param cp1x The x coordinate of the first control point.
     * \param cp1y The y coordinate of the first control point.
     * \param cp2x The x coordinate of the second control point.
     * \param cp2y The y coordinate of the second control point.
     * \param x The x coordinate of the end point.
     * \param y The y coordinate of the end point.
     */
    void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x,
                       float y);

    /*!
     * \brief quadraticCurveTo adds a quadratic Bézier curve to the path. It
     * requires two points. The first point is a control point and the second
     * one is the end point. The starting point is the last point in the current
     * path, which can be changed using moveTo() before creating the quadratic
     * Bézier curve.
     *
     * \param cpx The x axis of the coordinate for the control point.
     * \param cpy The y axis of the coordinate for the control point.
     * \param x The x axis of the coordinate for the end point.
     * \param y The y axis of the coordinate for the end point.
     */
    void quadraticCurveTo(float cpx, float cpy, float x, float y);

    /*!
     * \brief arc adds an arc to the path which is centered at (x, y) position
     * with radius r starting at startAngle and ending at endAngle going in the
     * given direction by anticlockwise (defaulting to clockwise).
     *
     * \param x The x coordinate of the arc's center.
     * \param y The y coordinate of the arc's center.
     * \param radius The arc's radius.
     * \param startAngle The angle at which the arc starts, measured clockwise
     * from the positive x axis and expressed in radians.
     * \param endAngle The angle at which the arc ends, measured clockwise from
     * the positive x axis and expressed in radians.
     * \param anticlockwise An optional boolean which, if true, causes the arc
     * to be drawn counter-clockwise between the two angles. By default it is
     * drawn clockwise.
     */
    void arc(float x, float y, float radius, float startAngle, float endAngle,
             bool anticlockwise = false);

    /*!
     * \brief arcTo adds an arc to the path with the given control points and
     * radius.
     *
     * The arc drawn will be a part of a circle, never elliptical. Typical use
     * could be making a rounded corner.
     *
     * One way to think about the arc drawn is to imagine two straight segments,
     * from the starting point (latest point in current path) to the first
     * control point, and then from the first control point to the second
     * control point. These two segments form a sharp corner with the first
     * control point being in the corner. Using arcTo, the corner will instead
     * be an arc with the given radius.
     *
     * The arc is tangential to both segments, which can sometimes produce
     * surprising results, e.g. if the radius given is larger than the distance
     * between the starting point and the first control point.
     *
     * If the radius specified doesn't make the arc meet the starting point
     * (latest point in the current path), the starting point is connected to
     * the arc with a straight line segment.
     *
     * \param x1 x-axis coordinates for the first control point.
     * \param y1 y-axis coordinates for the first control point.
     * \param x2 x-axis coordinates for the second control point.
     * \param y2 y-axis coordinates for the second control point.
     * \param radius The arc's radius.
     */
    void arcTo(float x1, float y1, float x2, float y2, float radius);

    /*!
     * \brief ellipse adds an ellipse to the path which is centered at (x, y)
     * position with the radii radiusX and radiusY starting at startAngle and
     * ending at endAngle going in the given direction by anticlockwise
     * (defaulting to clockwise).
     *
     * \param x The x axis of the coordinate for the ellipse's center.
     * \param y The y axis of the coordinate for the ellipse's center.
     * \param radiusX The ellipse's major-axis radius.
     * \param radiusY The ellipse's minor-axis radius.
     * \param rotation The rotation for this ellipse, expressed in radians.
     * \param startAngle The starting point, measured from the x axis, from
     * which it will be drawn, expressed in radians.
     * \param endAngle The end ellipse's angle to which it will be drawn,
     * expressed in radians.
     * \param anticlockwise An optional boolean which, if true, draws the
     * ellipse anticlockwise (counter-clockwise), otherwise in a clockwise
     * direction.
     */
    void ellipse(float x, float y, float radiusX, float radiusY, float rotation,
                 float startAngle, float endAngle, bool anticlockwise = false);

    /*!
     * \brief rect creates a path for a rectangle at position (x, y) with a size
     * that is determined by width and height. Those four points are connected
     * by straight lines and the sub-path is marked as closed, so that you can
     * fill or stroke this rectangle.
     * \param x The x coordinate for the left side of the rectangle.
     * \param y The y coordinate for the top of the rectangle.
     * \param width The rectangle's width.
     * \param height The rectangle's height.
     */
    void rect(float x, float y, float width, float height);

};

}

#include <TunisPath2D.inl>

#endif // TUNISPATH2D_H
