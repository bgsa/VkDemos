#version 450 
#extension GL_ARB_separate_shader_objects : enable 

//compile:
//C:\VulkanSDK\1.1.82.0\Bin\glslangValidator.exe -V "C:\Users\Bruno\Projects\VSCode\VkBootstrap\resources\shaders\shader.vert" -o "C:\Users\Bruno\Projects\VSCode\VkBootstrap\resources\shaders\vert.spv"

layout(binding = 0) uniform UniformBufferObject { 
	mat4 modelMatrix; 
	mat4 viewMatrix; 
	mat4 projectionMatrix; 
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor; 

out gl_PerVertex { 
    vec4 gl_Position; 
}; 

void main() 
{	
	mat4 p = mat4(
				1.00000000, 0.000000000, 0.000000000, 0.000000000,
				0.000000000, 1.00000000, 0.000000000, 0.000000000,
				0.000000000, 0.000000000, -1.00200200, -1.00000000,
				0.000000000, 0.000000000, -2.00200200, 0.000000000 );
	
	mat4 pHUD = mat4(
				0.00249999994, 0.000000000, 0.000000000, 0.000000000,
				0.000000000, 0.00333333341,0.000000000, 0.000000000,
				0.000000000, 0.000000000, -1.00000000, 0.000000000,
				-1.00000000, -1.00000000, -0.000000000, 1.00000000 );
			
	mat4 v = mat4(
				-1.00000000, 0.000000000, 0.000000000, 0.000000000,
				0.000000000, 0.883787930, 0.467887729, -2.65136385,
				0.000000000, 0.467887729, -0.883787930, -20.6390476,
				0.000000000, 0.000000000, 0.000000000, 1.00000000 );
			
	mat4 m = mat4(
				100.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 100.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f );


	//gl_Position = vec4(inPosition, 0.0, 1.0);			
	//gl_Position = ubo.projectionMatrix * ubo.viewMatrix * ubo.modelMatrix * vec4(inPosition, 0.0, 1.0);

	gl_Position = ubo.projectionMatrix * ubo.modelMatrix * vec4(inPosition, 0.0, 1.0);
		
	fragColor = inColor;
}
