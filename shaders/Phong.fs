#version 150

/// @brief[in] the vertex normal
in vec3 fragmentNormal;
/// @brief our output fragment colour
out vec4 fragColour;

/// @brief material structure
struct Materials
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

// @brief light structure
struct Lights
{
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};
// @param material passed from our program
uniform Materials material;

uniform Lights light;
in vec3 lightDir;
// out the blinn half vector
in vec3 halfVector;
in vec3 eyeDirection;
in vec3 vPosition;


/// @brief a function to compute point light values
/// @param[in] _light the number of the current light

vec4 pointLight()
{
  vec3 N = normalize(fragmentNormal);
  vec3 halfV;
  float ndothv;
  float attenuation;
  vec3 E = normalize(eyeDirection);
  vec3 L = normalize(lightDir);
  float lambertTerm = dot(N,L);
  vec4 diffuse=vec4(0);
  vec4 ambient=vec4(0);
  vec4 specular=vec4(0);
  if (lambertTerm > 0.0)
  {
  float d;            // distance from surface to light position
  vec3 VP;            // direction from surface to light position

  // Compute vector from surface to light position
  VP = vec3 (light.position) - vPosition;

  // Compute distance between surface and light position
    d = length (VP);

    if(!gl_FrontFacing)
    {
        N = -N;
    }

    diffuse+=material.diffuse*light.diffuse*lambertTerm;
    ambient+=material.ambient*light.ambient;
    specular+=material.specular*light.specular*pow(ndothv, material.shininess);
  }
return ambient + diffuse + specular;
}



void main ()
{

fragColour=pointLight();
}

