#pragma once
#ifndef spotLight_h
#define spotLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class SpotLight {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float k_c;
    float k_l;
    float k_q;
    int lightNumber;

    SpotLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic, int num) {

        position = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        lightNumber = num;
    }
    void setUpPointLight(Shader& lightingShader)
    {
        lightingShader.use();
        if (lightNumber == 1) {
            lightingShader.setVec3("spotLight[0].position", position);
            lightingShader.setVec3("spotLight[0].ambient", ambientOn * ambient);
            lightingShader.setVec3("spotLight[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("spotLight[0].specular", specularOn * specular);
            lightingShader.setFloat("spotLight[0].k_c", k_c);
            lightingShader.setFloat("spotLight[0].k_l", k_l);
            lightingShader.setFloat("spotLight[0].k_q", k_q);
        }
        else if (lightNumber == 2) {
            lightingShader.setVec3("spotLight[1].position", position);
            lightingShader.setVec3("spotLight[1].ambient", ambientOn * ambient);
            lightingShader.setVec3("spotLight[1].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("spotLight[1].specular", specularOn * specular);
            lightingShader.setFloat("spotLight[1].k_c", k_c);
            lightingShader.setFloat("spotLight[1].k_l", k_l);
            lightingShader.setFloat("spotLight[1].k_q", k_q);
        }
        else if (lightNumber == 3) {
            lightingShader.setVec3("spotLight[2].position", position);
            lightingShader.setVec3("spotLight[2].ambient", ambientOn * ambient);
            lightingShader.setVec3("spotLight[2].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("spotLight[2].specular", specularOn * specular);
            lightingShader.setFloat("spotLight[2].k_c", k_c);
            lightingShader.setFloat("spotLight[2].k_l", k_l);
            lightingShader.setFloat("spotLight[2].k_q", k_q);
        }
    }
    void upd_spotlight_pos(Shader& lightingShader, float posX, float posY, float posZ, int num)
    {
        lightingShader.use();
        position = glm::vec3(posX, posY, posZ);
        if (lightNumber == 1) {
            lightingShader.setVec3("spotLight[0].position", position);
        }
        else if (lightNumber == 2) {
            lightingShader.setVec3("spotLight[1].position", position);
        }
        else if (lightNumber == 3) {
            lightingShader.setVec3("spotLight[2].position", position);
        }
    }
    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        specularOn = 0.0;
    }
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* DirLight_h */
