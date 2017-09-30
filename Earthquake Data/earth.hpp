#ifndef EARTH_HPP
#define EARTH_HPP
#define PI 3.1415926

#include "engine.hpp"
#include "config.hpp"
#include <vector>
using namespace std;
using glm::vec2;
using glm::vec3;

class Earth {
public:
    void initialize(Engine *engine, int slices, int stacks, float spherical);
    float isSpherical();
    void setSpherical(float spherical);
    vec3 getPosition(float latitude, float longitude);
    vec3 getNormal(float latitude, float longitude);
    void draw(bool textured);
    vec3 lerp(vec3 rec, vec3 sph, float index);
protected:
    int slices, stacks;
    int nVertices, nTriangles;
    float spherical;
    Engine *engine;
    
    // TODO: Define the necessary buffers and texture.
    // Feel free to add helper methods to update the buffers.
    VertexBuffer vertexBuffer, normalBuffer, texCoordBuffer;
    ElementBuffer indexBuffer;
    Texture texture;
    
    vector<vec3> recVerts, recNormals, sphVerts, sphNormals, smoothVerts, smoothNormals;
    vector<vec2> rec_texCoords, sph_texCoords;
};

inline void Earth::initialize(Engine *e, int sl, int st, float sp) {
    engine = e;
    slices = sl;
    stacks = st;
    spherical = sp;

    // TODO: Initialize nVertices, nTriangles, buffers, texture
    
    // rectangle form
    for (int i = 0; i <= slices; i++) {
        for (int j = 0; j <= stacks; j++) {
            float x_var = 2*PI*i/slices;
            float y_var = PI*j/stacks;

            recVerts.push_back(vec3(-PI+x_var, PI/2-y_var, 0));
            recNormals.push_back(vec3(0, 0, 1));
            rec_texCoords.push_back(vec2((float)i/slices, (float)j/stacks));
            
        }
    }
    
    nVertices = (int) recVerts.size();
    vertexBuffer = engine->allocateVertexBuffer(nVertices*sizeof(vec3));
    engine->copyVertexData(vertexBuffer, &recVerts[0], nVertices*sizeof(vec3));
        
    normalBuffer = engine->allocateVertexBuffer(nVertices*sizeof(vec3));
    engine->copyVertexData(normalBuffer, &recNormals[0], nVertices*sizeof(vec3));
    
    texCoordBuffer = engine->allocateVertexBuffer(nVertices*sizeof(vec2));
    engine->copyVertexData(texCoordBuffer, &rec_texCoords[0], nVertices*sizeof(vec2));
    
    vector<int> indices;
    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            indices.push_back(i*(stacks+1)+j);
            indices.push_back(i*(stacks+1)+j+1);
            indices.push_back(i*(stacks+1)+j+stacks+2);
            
            indices.push_back(i*(stacks+1)+j);
            indices.push_back(i*(stacks+1)+j+stacks+2);
            indices.push_back(i*(stacks+1)+j+stacks+1);
        }
    }
        
    nTriangles = (int) indices.size();
    indexBuffer = engine->allocateVertexBuffer(nTriangles*sizeof(int));
    engine->copyElementData(indexBuffer, &indices[0], nTriangles*sizeof(int));
    
    
    string path = Config::textureFile;
    texture = engine->loadTexture(path);
    
    // sphere form
    for (int i = 0; i <= slices; i++) {
        for (int j = 0; j <= stacks; j++) {
            float temp_longtitude = -PI+2*PI*i/slices;
            float temp_latitude = PI/2-PI*j/stacks;
            float x_var = cosf(temp_latitude)*sinf(temp_longtitude);
            float y_var = sinf(temp_latitude);
            float z_var = cosf(temp_latitude)*cosf(temp_longtitude);
            
            vec3 tempVec = vec3(x_var, y_var, z_var);
            //vec3 tempNorm = normalize(tempVec);
            
            sphVerts.push_back(tempVec);
            sphNormals.push_back(tempVec);
        }
    }
    
    
}

inline float Earth::isSpherical() {
    return spherical;
}

inline vec3 Earth::lerp(vec3 vec1, vec3 vec2, float index){
    float temp_x = vec1.x - index*(vec2.x - vec1.x);
    float temp_y = vec1.y - index*(vec2.y - vec1.y);
    float temp_z = vec1.z - index*(vec2.z - vec1.z);
    return vec3(temp_x, temp_y, -temp_z);
}

inline void Earth::setSpherical(float s) {
    spherical = s;

    // TODO: Also update the relevant buffers.
    // You should only need to update two buffers, not all of them.
    if (spherical != 1 && spherical !=0) {
        for (int i = 0; i < nVertices; i++) {
            vec3 temp_smooth = lerp(recVerts[i], sphVerts[i], spherical);
            smoothVerts.push_back(temp_smooth);
            smoothNormals.push_back(temp_smooth);
        }
    }
    
    
    if (spherical == 1) {
        //nVertices = (int) sphVerts.size();
        engine->copyVertexData(vertexBuffer, &sphVerts[0], nVertices*sizeof(vec3));
        engine->copyVertexData(normalBuffer, &sphNormals[0], nVertices*sizeof(vec3));
    } else if (spherical == 0) {
        //nVertices = (int) recVerts.size();
        engine->copyVertexData(vertexBuffer, &recVerts[0], nVertices*sizeof(vec3));
        engine->copyVertexData(normalBuffer, &recNormals[0], nVertices*sizeof(vec3));
    } else {
        engine->copyVertexData(vertexBuffer, &smoothVerts[0], nVertices*sizeof(vec3));
        engine->copyVertexData(normalBuffer, &smoothNormals[0], nVertices*sizeof(vec3));
    }

}

inline vec3 Earth::getPosition(float latitude, float longitude) {
    vec3 rectangularPosition(0,0,0), sphericalPosition(0,0,0);

    // TODO compute vertex positions on rectangle and sphere

    if (spherical == 0){
        rectangularPosition.y = latitude/180*PI;
        rectangularPosition.x = longitude/180*PI;
        return rectangularPosition;
    }
    else if (spherical == 1) {
        float temp_long = longitude/180*PI;
        float temp_lat = latitude/180*PI;
        sphericalPosition.x = cosf(temp_lat)*sinf(temp_long);
        sphericalPosition.y = sinf(temp_lat);
        sphericalPosition.z = cosf(temp_lat)*cosf(temp_long);
        return sphericalPosition;
    }
    else {
        
        // TODO compute the interpolated position
        float temp_long = longitude/180*PI;
        float temp_lat = latitude/180*PI;
        float sph_x = cosf(temp_lat)*sinf(temp_long);
        float sph_y = sinf(temp_lat);
        float sph_z = cosf(temp_lat)*cosf(temp_long);
        
        vec3 result = lerp(vec3(temp_long, temp_lat, 0), vec3(sph_x, sph_y, sph_z), spherical);
        return result;

    }
}

inline vec3 Earth::getNormal(float latitude, float longitude) {
    vec3 rectangularNormal(0,0,1), sphericalNormal(0,0,0);

    // TODO compute vertex positions on rectangle and sphere
    if (spherical == 0)
        return rectangularNormal;
    else if (spherical == 1){
        float temp_long = longitude/180*PI;
        float temp_lat = latitude/180*PI;
        sphericalNormal.x = cosf(temp_lat)*sinf(temp_long);
        sphericalNormal.y = sinf(temp_lat);
        sphericalNormal.z = cosf(temp_lat)*cosf(temp_long);
        return sphericalNormal;
    }
    else {

        // TODO compute the interpolated normal
        float temp_long = longitude/180*PI;
        float temp_lat = latitude/180*PI;
        float sph_x = cosf(temp_lat)*sinf(temp_long);
        float sph_y = sinf(temp_lat);
        float sph_z = cosf(temp_lat)*cosf(temp_long);
        
        vec3 result = lerp(vec3(temp_long, temp_lat, 0), vec3(sph_x, sph_y, sph_z), spherical);
        return result;

    }
}

inline void Earth::draw(bool textured) {
    // TODO: Draw the mesh (with or without texture, depending on the input)
    
    if (textured == true) {
        engine->setVertexArray(vertexBuffer);
        engine->setNormalArray(normalBuffer);
        engine->setTexCoordArray(texCoordBuffer);
        engine->setTexture(texture);
        engine->drawElements(GL_TRIANGLES, indexBuffer, nTriangles);
        engine->unsetTexture();
    }
    else {
        engine->setVertexArray(vertexBuffer);
        engine->setNormalArray(normalBuffer);
        engine->setTexCoordArray(texCoordBuffer);
        engine->drawElements(GL_TRIANGLES, indexBuffer, nTriangles);
    }
}

#endif
