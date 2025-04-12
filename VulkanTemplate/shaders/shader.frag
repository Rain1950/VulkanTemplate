#version 450

#extension GL_KHR_vulkan_glsl : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in mat4 nm;

layout(location = 0) out vec4 outColor;
layout(binding = 1) uniform sampler2D texSampler;


void main(){
	vec3 objectColor = fragColor *  texture( texSampler,fragTexCoord ).rgb;
	outColor = vec4(nm * vec4(normal * 100, 0.0));
}