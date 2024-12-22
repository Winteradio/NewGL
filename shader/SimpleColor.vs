#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 oColor;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProj;

void main()
{
   oColor = aColor;

   vec4 vPos = vec4(aPos.x, aPos.y, aPos.z, 1.0);

   // gl_Position = mProj * mView * mModel * vPos;
   gl_Position = mProj * mView * mModel * vPos;
};