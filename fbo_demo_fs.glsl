#version 430
layout(binding = 0) uniform sampler2D diffuse_tex; 
layout(binding = 1) uniform sampler2D fbo_tex; 

layout(location = 1) uniform float time;
layout(location = 2) uniform int pass;
layout(location = 3) uniform int mode;

layout(std140, binding = 0) uniform SceneUniforms
{
   mat4 PV;	//camera projection * view matrix
   vec4 eye_w;	//world-space eye position
};

layout(std140, binding = 1) uniform LightUniforms
{
   vec4 La;	//ambient light color
   vec4 Ld;	//diffuse light color
   vec4 Ls;	//specular light color
   vec4 light_w; //world-space light position
};

layout(std140, binding = 2) uniform MaterialUniforms
{
   vec4 ka;	//ambient material color
   vec4 kd;	//diffuse material color
   vec4 ks;	//specular material color
   float shininess; //specular exponent
};

in VertexData
{
   vec2 tex_coord;
   vec3 pw;       //world-space vertex position
   vec3 nw;   //world-space normal vector
} inData;   //block is named 'inData'

out vec4 fragcolor; //the output color for this fragment    

vec4 blur();
vec4 edge();
vec4 vignette();
vec4 glitch();
vec4 gamma();

void main(void)
{   
	if(pass == 0)
	{
		//Compute per-fragment Phong lighting
		vec4 ktex = texture(diffuse_tex, inData.tex_coord);
	
		vec4 ambient_term = ka*ktex*La;

		const float eps = 1e-8; //small value to avoid division by 0
		float d = distance(light_w.xyz, inData.pw.xyz);
		float atten = 1.0/(d*d+eps); //d-squared attenuation

		vec3 nw = normalize(inData.nw);			//world-space unit normal vector
		vec3 lw = normalize(light_w.xyz - inData.pw.xyz);	//world-space unit light vector
		vec4 diffuse_term = atten*kd*ktex*Ld*max(0.0, dot(nw, lw));

		vec3 vw = normalize(eye_w.xyz - inData.pw.xyz);	//world-space unit view vector
		vec3 rw = reflect(-lw, nw);	//world-space unit reflection vector

		vec4 specular_term = atten*ks*Ls*pow(max(0.0, dot(rw, vw)), shininess);

		fragcolor = ambient_term + diffuse_term + specular_term;
	}
    if(pass == 1)
	{
		if(mode == 0)
		{
			fragcolor = texelFetch(fbo_tex, ivec2(gl_FragCoord), 0);
		}
		else if(mode == 1)
		{
			fragcolor = blur();
		}
		else if(mode == 2)
		{
			fragcolor = edge();
		}
		else if(mode == 3)
		{
			fragcolor = vignette();
		}
		else if(mode == 4)
		{
			fragcolor = glitch();
		}
		else if(mode == 5)
		{
			fragcolor = gamma();
		}
	}
}

vec4 blur()
{      
   int hw = 5;
   float n=0.0;
   vec4 blur = vec4(0.0);
   for(int i=-hw; i<=hw; i++)
   {
      for(int j=-hw; j<=hw; j++)
      {
         blur += texelFetch(fbo_tex, ivec2(gl_FragCoord)+ivec2(i,j), 0);
         n+=1.0;
      }
   }
   blur = blur/n;
   return blur;
}

vec4 edge()
{      
   vec4 n = texelFetch(fbo_tex, ivec2(gl_FragCoord)+ivec2(0,+1), 0);
   vec4 s = texelFetch(fbo_tex, ivec2(gl_FragCoord)+ivec2(0,-1), 0);
   vec4 e = texelFetch(fbo_tex, ivec2(gl_FragCoord)+ivec2(+1,0), 0);
   vec4 w = texelFetch(fbo_tex, ivec2(gl_FragCoord)+ivec2(-1,0), 0);

   vec4 v_diff = abs(n-s);
   vec4 h_diff = abs(e-w);
   //vec4 edge = max(v_diff, h_diff);
   //vec4 edge = v_diff + h_diff;
   //vec4 edge = vec4(max(length(v_diff), length(h_diff)));
   vec4 edge = vec4(length(v_diff) + length(h_diff));
   return edge;
}

vec4 vignette()
{
   vec4 c = texelFetch(fbo_tex, ivec2(gl_FragCoord), 0);
   vec2 coord = inData.tex_coord - vec2(0.5);
   float darken = smoothstep(1.0, 0.3, length(coord));
   return darken*c;
}

vec4 glitch()
{
   float flicker = step(0.0, fract(0.25*time)-0.92)*abs(fract(sin(15656.0*time)));
   
   vec4 c = vec4(1.0);
   c.r = texture(fbo_tex, inData.tex_coord + vec2(0.25*flicker, 0.0)).r;
   c.g = texture(fbo_tex, inData.tex_coord + vec2(0.011*fract(time*1000.0 + 1500.0*inData.tex_coord.y), 0.0)).g;
   c.b = texture(fbo_tex, inData.tex_coord + vec2(-0.001, 0.005)).b;

   float lines = (mod(gl_FragCoord.y, 2.5) + 0.75)/2.0;
   
   return lines*c*(1.0-flicker);
}

vec4 gamma()
{
   vec4 c = texelFetch(fbo_tex, ivec2(gl_FragCoord), 0);
   float g = 1.0/1.5;
   return pow(c, vec4(g,g,g,1.0));
}