#include "Uniforms.h"
#include <GL/glew.h>

namespace Uniforms
{
   SceneUniforms SceneData;
   LightUniforms LightData;
   MaterialUniforms MaterialData;

   //IDs for the buffer objects holding the uniform block data
   GLuint scene_ubo = -1;
   GLuint light_ubo = -1;
   GLuint material_ubo = -1;

   namespace UboBinding
   {
      //These values come from the binding value specified in the shader block layout
      int scene = 0;
      int light = 1;
      int material = 2;
   };

   //Locations for the uniforms which are not in uniform blocks
   namespace UniformLocs
   {
      int M = 0; //model matrix
      int M1 = 0;
      int M2 = 0;
      int M3 = 0;
      int ObjectID = 4;
      int PickedID = 5;
      int time = 1;
      int pass = 2;
      int mode = 3;
   };

   void Init()
   {
      //Create and initialize uniform buffers
      glGenBuffers(1, &Uniforms::scene_ubo);
      glBindBuffer(GL_UNIFORM_BUFFER, scene_ubo);
      glBufferData(GL_UNIFORM_BUFFER, sizeof(SceneUniforms), nullptr, GL_STREAM_DRAW); //Allocate memory for the buffer, but don't copy (since pointer is null).
      glBindBufferBase(GL_UNIFORM_BUFFER, UboBinding::scene, scene_ubo); //Associate this uniform buffer with the uniform block in the shader that has the same binding.

      glGenBuffers(1, &light_ubo);
      glBindBuffer(GL_UNIFORM_BUFFER, light_ubo);
      glBufferData(GL_UNIFORM_BUFFER, sizeof(LightUniforms), &LightData, GL_STREAM_DRAW); //Allocate memory for the buffer, but don't copy (since pointer is null).
      glBindBufferBase(GL_UNIFORM_BUFFER, UboBinding::light, light_ubo); //Associate this uniform buffer with the uniform block in the shader that has the same binding.

      glGenBuffers(1, &material_ubo);
      glBindBuffer(GL_UNIFORM_BUFFER, material_ubo);
      glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialUniforms), &MaterialData, GL_STREAM_DRAW); //Allocate memory for the buffer, but don't copy (since pointer is null).
      glBindBufferBase(GL_UNIFORM_BUFFER, UboBinding::material, material_ubo); //Associate this uniform buffer with the uniform block in the shader that has the same binding.

      glBindBuffer(GL_UNIFORM_BUFFER, 0);
   }

   void BufferSceneData()
   {
      glBindBuffer(GL_UNIFORM_BUFFER, scene_ubo); //Bind the OpenGL UBO before we update the data.
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SceneData), &SceneData); //Upload the new uniform values.
      glBindBuffer(GL_UNIFORM_BUFFER, 0); //unbind the ubo
   }
};
