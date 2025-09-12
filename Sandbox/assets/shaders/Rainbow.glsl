#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;   // opcjonalne, ale przyda siê do wzoru

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_UV;

void main()
{
    v_UV = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core

in vec2 v_UV;
out vec4 FragColor;

uniform float u_Time;        // ustawiane co klatkê
uniform float u_Saturation;  // np. 1.0
uniform float u_Value;       // np. 1.0
uniform float u_Speed;       // np. 0.3–1.0
uniform float u_Tiling;      // skala „têczy” wzd³u¿ paletki, np. 1.0–3.0
uniform float u_Alpha;       // np. 1.0

// proste HSV->RGB bez ga³êzi
vec3 hsv2rgb(vec3 c)
{
    vec3 p = abs(fract(c.x + vec3(0.0, 2.0/3.0, 1.0/3.0)) * 6.0 - 3.0);
    vec3 rgb = clamp(p - 1.0, 0.0, 1.0);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

void main()
{
    // Hue animowany w czasie + zale¿ny od UV.y (pionowa „têcza”)
    float hue = fract(u_Time * u_Speed + v_UV.y * u_Tiling);
    vec3  rgb = hsv2rgb(vec3(hue, u_Saturation, u_Value));

    FragColor = vec4(rgb, u_Alpha);
}
