
#version 450

#extension GL_KHR_vulkan_glsl : enable

layout(binding = 0) uniform UniformBufferObject{
	mat4 model;
	mat4 view;
	mat4 proj;
	float time;
} ubo;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;





void main(){
	gl_Position = ubo.proj* ubo.view * ubo.model *   vec4(inPosition ,1.0);
	gl_Position.y += sin(ubo.time*10 + inPosition.y*100)/20;
	
	fragColor = inColor;
	//fragColor = vec3(clamp(inColor.x  + sin(ubo.time)*50 + 25,1,255),clamp(inColor.y  + sin(ubo.time + 2)*50 ,1,255),clamp(inColor.z  + sin(ubo.time+0.5f)*50 +40  ,1,255));
	fragTexCoord = inTexCoord;

}