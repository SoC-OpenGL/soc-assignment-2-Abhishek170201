#version 400
struct Material
{
        sampler2D diffuse;
	sampler2D specular;
	sampler2D normalmap;
	float shininess;
};
struct pointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
struct directionLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
struct spotLight
{
	vec3 position;
	vec3 direction
	float cutoff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform pointLight pointlight;
uniform directionLight directionlight;
uniform spotLight spotlight;


out vec4 fragcolor;

in vec2 Tex;
in vec3 fragpos;
in vec3 Normal;

uniform vec3 viewPos;
uniform int shadingmode;
uniform int lightmode; 

vec4 PointLight(pointLight light,vec3 fragpos,vec3 Normal,vec3 Tex,int shadingmode);
vec4 DirectionLight(directionlight light,vec3 fragpos,vec3 Normal,vec3 Tex,int shadingmode);
vec4 SpotLight(spotLight light,vec3 fragpos,vec3 Normal,vec3 Tex,int shadingmode);


void main()
{if(lightmode==1)
 fragcolor=PointLight(pointlight,fragpos,Normal,Tex,shadingmode);
 if(lightmode==2)
 fragcolor=DirectionLight(directionlight,fragpos,Normal,Tex,shadingmode);
 if(lightmode==3)
 fragcolor=SpotLight(spotlight,fragpos,normal,Tex,shadingmode);
}


vec4 PointLight(pointLight light,vec3 fragpos,vec3 Normal,vec3 Tex,int shadingmode)
{float distance=length(light.position-fragpos);
 float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*distance*distance);
 
 vec3 normal=texture(normalmap,Tex).rgb;
 normal=normalize(normal*2.0 - 1.0);

 
 vec3 ambient =light.ambient*texture(material.diffuse,Tex).rgb;
 vec3 norm=normalize(Normal);
 vec3 lightDir=normalize(light.position-fragpos);

 float diff=max(dot(norm,lightDir),0); 
 vec3 diffuse=diff*light.diffuse*texture(material.diffuse,Tex).rgb;

 vec3 viewDir=normalize(viewPos-fragpos);
 
float spec;
if(shadingmode==1)
 {vec3 reflecDir=reflect(-lightPos,norm);
 spec=pow(max(dot(reflectDir,viewDir),0),material.shininess);

 }
else
{vec3 halfwayDir=normalize(lightDir+viewDir);
spec=pow(max(dot(norm,halfwayDir),0),4*material.shininess)
}
 vec3 specular=spec*light.specular*texture(material.specular,Tex).rgb;

 ambient*=attenuation;
 diffuse*=attenuation;
 specular*=attenuation;

 vec3 result=ambient+diffuse+specular;
result*=texture(material.albedo,Tex).rgb; 
 vec4 frag=vec4(result,1.0f);

 return frag;
}

vec4 DirectionLight(directionlight light,vec3 fragpos,vec3 Normal,vec3 Tex,int shadingmode)
{float distance=length(light.position-fragpos);
 float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*distance*distance);
  vec3 Normal=texture(normalmap,Tex).rgb;
 normal=normalize(normal*2.0 - 1.0);
 vec3 ambient =light.ambient*texture(material.diffuse,Tex).rgb;
 vec3 norm=normalize(Normal);
 vec3 lightDir=light.direction;

 float diff=max(dot(norm,lightDir),0); 
 vec3 diffuse=diff*light.diffuse*texture(material.diffuse,Tex).rgb;

 vec3 viewDir=normalize(viewPos-fragpos);
 
float spec;
if(shadingmode==1)
 {vec3 reflecDir=reflect(-lightPos,norm);
 spec=pow(max(dot(reflectDir,viewDir),0),material.shininess);

 }
else
{vec3 halfwayDir=normalize(lightDir+viewDir);
spec=pow(max(dot(norm,halfwayDir),0),4*material.shininess)
}
 vec3 specular=spec*light.specular*texture(material.specular,Tex).rgb;

 ambient*=attenuation;
 diffuse*=attenuation;
 specular*=attenuation;

 vec3 result=ambient+diffuse+specular;
result*=texture(material.albedo,Tex).rgb;
 vec4 frag=vec4(result,1.0f);

 return frag;
}

vec4 SpotLight(spotLight light,vec3 fragpos,vec3 Normal,vec3 Tex,int shadingmode)
{float theta=dot(normalize(light.position-fragpos),normalize(-light.direction));
 float epsilon=light.cutoff-light.outercutoff;
 float intensity=clamp((theta-light.outercutoff)epsilon,0.0,1.0);

 if(theta>=light.cutoff)
{float distance=length(light.position-fragpos);
 float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*distance*distance);
  vec3 normal=texture(normalmap,Tex).rgb;
 normal=normalize(normal*2.0 - 1.0);
 vec3 ambient =light.ambient*texture(material.diffuse,Tex).rgb;
 vec3 norm=normalize(Normal);
 vec3 lightDir=normalize(light.position-fragpos);

 float diff=max(dot(norm,lightDir),0); 
 vec3 diffuse=diff*light.diffuse*texture(material.diffuse,Tex).rgb;

 vec3 viewDir=normalize(viewPos-fragpos);
 
float spec;
if(shadingmode==1)
 {vec3 reflecDir=reflect(-lightPos,norm);
 spec=pow(max(dot(reflectDir,viewDir),0),material.shininess);

 }
else
{vec3 halfwayDir=normalize(lightDir+viewDir);
spec=pow(max(dot(norm,halfwayDir),0),4*material.shininess)
}
 vec3 specular=spec*light.specular*texture(material.specular,Tex).rgb;

 ambient*=attenuation;
 diffuse*=attenuation;diffuse*=intensity;
 specular*=attenuation;specular*=intensity;


 vec3 result=ambient+diffuse+specular;
 result*=texture(material.albedo,Tex).rgb;
 vec4 frag=vec4(result,1.0f);
}

else
vec4 frag=vec4(light.ambient*texture(material.diffuse,Tex).rgb,1.0);


return frag;
}




