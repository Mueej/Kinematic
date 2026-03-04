#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 FragNormal;

uniform vec3 color;
uniform vec3 lightColor;
uniform bool isLit;

void main()
{
   if (isLit) {
      // Simple directional light setup
      vec3 norm = normalize(FragNormal);
      vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3)); // Hardcoded directional sun

      // Very basic ambient light so shadows aren't pitch black
      float ambient = 0.5;
      
      // Very basic diffuse (how directly face points at sun)
      float diff = max(dot(norm, lightDir), 0.0);
      
      // Total impact
      float intensity = ambient + (diff * 0.7);
      
      vec3 result = color * lightColor * intensity;
      FragColor = vec4(result, 1.0);
   } else {
      FragColor = vec4(color, 1.0);
   }
}