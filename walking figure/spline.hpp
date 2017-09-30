#ifndef SPLINE_HPP
#define SPLINE_HPP

#include <glm/glm.hpp>
#include <vector>
using glm::vec3;

class SplinePoint3 {
public:
    float t;    // time
    vec3 p, dp; // value and derivative
    SplinePoint3(float t, vec3 p, vec3 dp): t(t), p(p), dp(dp) {}
};

class Spline3 {
public:
    // list of spline control points. assumed to be in increasing order of t
    std::vector<SplinePoint3> points;

    // value of t for first and last control points
    float minTime() {return points.front().t;}
    float maxTime() {return points.back().t;}

    // returns i such that t lies between points[i].t and points[i+1].t.
    // also modifies t to lie in [minTime(), maxTime()] if necessary.
    int findSegment(float &t);

    // returns value of spline function at time t
    vec3 getValue(float t);

    // returns d/dt of spline function at time t
    vec3 getDerivative(float t);
};

inline int Spline3::findSegment(float &t) {

    // TODO: If t is outside the range [minTime(), maxTime()], replace
    // it with the closest time in that range. Then, find the segment
    // that contains t, so that you can perform cubic Hermite
    // interpolation within it.
    
    if (t < minTime()) {
        t = minTime();
    } else if (t > maxTime()) {
        t = maxTime();
    }
    
    int ret = 0;
    for (int i = 0; i < points.size(); i++) {
        if (points[i].t <= t && points[i+1].t >= t){
            ret = i;
        }
    }
    return ret;
}

inline vec3 Spline3::getValue(float t) {

    // TODO: Find the segment that contains t, and use the cubic
    // Hermite interpolation formula to find the interpolated value
    // within it. Note that the formula discussed in class is only
    // valid for t0 = 0, t1 = 1, so you will have to use a modified
    // formula.
    
    int range = findSegment(t);
    SplinePoint3 p1 = points[range];
    SplinePoint3 p2 = points[range+1];
    float scale = (t - p1.t) / (p2.t - p1.t);
    vec3 pLeft = p1.p;
    vec3 dLeft = p1.dp;
    vec3 pRight = p2.p;
    vec3 dRight = p2.dp;
    
    vec3 ret = (2*pow(scale,3) - 3*scale*scale + 1) * pLeft + (pow(scale,3) - 2*scale*scale + scale) * (p2.t - p1.t) * dLeft +
                (-2*pow(scale,3) + 3*scale*scale) * pRight + (pow(scale, 3) - scale*scale) * (p2.t - p1.t) * dRight;
    
    return ret;

}

inline vec3 Spline3::getDerivative(float t) {

    // TODO: Find the segment that contains t. Differentiate the cubic
    // Hermite interpolation formula to find the derivative of the
    // spline function. Be careful about how rescaling affects
    // derivatives.
    
    int range = findSegment(t);
    SplinePoint3 p1 = points[range];
    SplinePoint3 p2 = points[range+1];
    float scale = (t - p1.t) / (p2.t - p1.t);
    vec3 pLeft = p1.p;
    vec3 dLeft = p1.dp;
    vec3 pRight = p2.p;
    vec3 dRight = p2.dp;

    vec3 ret = (6*scale*scale - 6*scale) * pLeft + (3*scale*scale - 4*scale + 1) * (p2.t - p1.t) * dLeft +
                (-6*scale*scale + 6*scale) * pRight + (3*scale*scale - 2*scale) * (p2.t - p1.t) * dRight;
    
    return ret;

}

#endif
