#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Uniforms
{
   void Init();
   void BufferSceneData();

   //This structure mirrors the uniform block declared in the shader
   struct SceneUniforms
   {
      glm::mat4 PV;	//camera projection * view matrix
      glm::vec4 eye_w = glm::vec4(0.0f, 0.0f, 3.0f, 1.0f);	//world-space eye position
   };

   struct LightUniforms
   {
      glm::vec4 La = glm::vec4(0.5f, 0.5f, 0.55f, 1.0f);	//ambient light color
      glm::vec4 Ld = glm::vec4(0.5f, 0.5f, 0.25f, 1.0f);	//diffuse light color
      glm::vec4 Ls = glm::vec4(0.3f);	//specular light color
      glm::vec4 light_w = glm::vec4(0.0f, 1.2, 1.0f, 1.0f); //world-space light position
   };

   struct MaterialUniforms
   {
      glm::vec4 ka = glm::vec4(1.0f);	//ambient material color
      glm::vec4 kd = glm::vec4(1.0f);	//diffuse material color
      glm::vec4 ks = glm::vec4(1.0f);	//specular material color
      float shininess = 20.0f;         //specular exponent
   };

   extern SceneUniforms SceneData;
   extern LightUniforms LightData;
   extern MaterialUniforms MaterialData;

   //IDs for the buffer objects holding the uniform block data
   extern GLuint scene_ubo;
   extern GLuint light_ubo;
   extern GLuint material_ubo;

   namespace UboBinding
   {
      //These values come from the binding value specified in the shader block layout
      extern int scene;
      extern int light;
      extern int material;
   };

   //Locations for the uniforms which are not in uniform blocks
   namespace UniformLocs
   {
      extern int M; //model matrix
      extern int M1;
      extern int M2;
      extern int M3;
      extern int ObjectID;
      extern int PickedID;
      extern int time;
      extern int pass;
      extern int mode;
   };
};
