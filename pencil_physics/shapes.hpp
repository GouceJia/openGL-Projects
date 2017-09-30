#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <Box2D/Box2D.h>

using namespace std;
using glm::vec2;

// TODO: Modify these classes to keep track of the corresponding Box2D
// bodies. Feel free to add helpful methods like
//     mat4 getTransformation()
// and
//     vec2 getLocalPoint(vec2 worldPoint).

class Circle {
public:
    vec2 center;
    float radius;
    b2World *w;
    b2Body *circleObj;
    
    Circle() {}
    Circle(vec2 center, float radius, b2World *world, bool dynamic) {
        this->center = center;
        this->radius = radius;
        this->w = world;
        
        b2BodyDef circleDef;
        if (dynamic) {
            circleDef.type = b2_dynamicBody;
        } else {
            circleDef.type = b2_staticBody;
        }
        circleDef.position.Set(center.x, center.y);
        circleObj = w->CreateBody(&circleDef);
        b2CircleShape circleShape;
        circleShape.m_p.Set(0.0f, 0.0f);
        circleShape.m_radius = radius;
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = 0.2;
        fixtureDef.friction = 0.4;
        fixtureDef.restitution = 0.2;
        circleObj->CreateFixture(&fixtureDef);
    }
    
    b2Body * getBody() {
        return circleObj;
    }
    
    vec2 getCurPos(void) {
        b2Vec2 temp = circleObj->GetPosition();
        return vec2(temp.x, temp.y);
    }
    
    float getAngle(void) {
        return circleObj->GetAngle();
    }
    
    bool contains(vec2 worldPoint) {
        return glm::length(worldPoint - this->getCurPos()) <= radius;
    }
    
    void destroy() {
        w->DestroyBody(circleObj);
    }
};

class Box {
public:
    vec2 center;
    vec2 size;
    b2Body *boxObj;
    b2World *w;
    
    Box() {}
    Box(vec2 center, vec2 size, b2World *world, bool dynamic) {
        this->center = center;
        this->size = size;
        this->w = world;
        
        b2BodyDef boxDef;
        if (dynamic) {
            boxDef.type = b2_dynamicBody;
        } else {
            boxDef.type = b2_staticBody;
        }
        boxDef.position.Set(center.x, center.y);
        boxObj = w->CreateBody(&boxDef);
        b2PolygonShape boxShape;
        boxShape.SetAsBox(size.x/2, size.y/2);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 0.2;
        fixtureDef.friction = 0.4;
        fixtureDef.restitution = 0.2;
        boxObj->CreateFixture(&fixtureDef);
    }
    
    b2Body * getBody() {
        return boxObj;
    }
    
    vec2 getCurPos(void) {
        b2Vec2 temp = boxObj->GetPosition();
        return vec2(temp.x, temp.y);
    }
    
    float getAngle(void) {
        return boxObj->GetAngle();
    }
    
    bool contains(vec2 worldPoint) {
        vec2 d = worldPoint - this->getCurPos();
        float angle = this->getAngle();
        vec2 t = vec2(glm::cos(-angle)*d.x - glm::sin(-angle)*d.y, glm::sin(-angle)*d.x + glm::cos(-angle)*d.y);
        return (abs(t.x) <= size.x/2 && abs(t.y) <= size.y/2);
    }
    
    void destroy() {
        w->DestroyBody(boxObj);
    }
};

class Polyline {
public:
    vector<vec2> vertices;
    b2Body *lineBody;
    b2World* w;
    
    Polyline() {}
    Polyline(vector<vec2> vertices,b2World *world) {
        this->vertices = vertices;
        this->w = world;
        
        b2BodyDef lineDef;
        lineDef.position.Set(0,0);
        lineDef.type = b2_staticBody;
        lineBody = world->CreateBody(&lineDef);
        b2ChainShape lineShape;
        lineShape.CreateChain((b2Vec2*)this->translate(), this->vertices.size());
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &lineShape;
        fixtureDef.friction = 0.6;
        fixtureDef.restitution = 0.1;
        lineBody->CreateFixture(&fixtureDef);
        
    }
    
    void * translate(){
        b2Vec2* out = new b2Vec2[vertices.size()];
        for (int i = 0; i < vertices.size(); i++){
            out[i] = *new b2Vec2(vertices[i].x, vertices[i].y);
        }
        return (void*) out;
    }
    
    void destroy() {
        w->DestroyBody(lineBody);
    }
};

#endif
