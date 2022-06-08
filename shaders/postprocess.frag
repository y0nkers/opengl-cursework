#version 410 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

uniform bool grayscale;
uniform bool blur;
uniform bool negative;
uniform bool fisheye;
uniform bool bloom;

const float PI = 3.1415926535;
float offset = 1.0f / 300.0f;

vec2 offsets[9] = vec2[]
(
	vec2(-offset, offset),  // top left
	vec2( 0.0f, offset), 	// top center
	vec2( offset, offset),  // top right
	vec2(-offset, 0.0f),   	// center left
	vec2( 0.0f, 0.0f),   	// center
	vec2( offset, 0.0f),   	// center right
	vec2(-offset, -offset), // bottom left
	vec2( 0.0f, -offset), 	// bottom center
	vec2( offset, -offset)  // bottom right    
);

void main()
{    
    if (grayscale)
	{	
		FragColor = texture(screenTexture, TexCoords);
		float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0f;
		FragColor = vec4(average, average, average, 1.0f);
	}
	else if (blur)
	{
		float kernel[9] = float[]
		(
			0.0625f, 0.125f, 0.0625f,
			0.125f, 0.25f, 0.125f,
			0.0625f, 0.125f, 0.0625f
		);

		vec3 sampleTex[9];
		for(int i = 0; i < 9; ++i) sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		for(int i = 0; i < 9; ++i) FragColor.rgb += sampleTex[i] * kernel[i];
	}
	else if (negative)
	{
		FragColor = vec4(1.0 - texture(screenTexture, TexCoords).rgb, 1.0);
	}
	else if (fisheye) 
	{
		float aperture = 178.0;
		float apertureHalf = 0.5 * aperture * (PI / 180.0);
		float maxFactor = sin(apertureHalf);
		vec2 uv;
		vec2 xy = 2.0 * TexCoords.xy - 1.0;
		float d = length(xy);
		if (d < (2.0-maxFactor))
		{
			d = length(xy * maxFactor);
			float z = sqrt(1.0 - d * d);
			float r = atan(d, z) / PI;
			float phi = atan(xy.y, xy.x);
    
			uv.x = r * cos(phi) + 0.5;
			uv.y = r * sin(phi) + 0.5;
		}
		else uv = TexCoords.xy;

		FragColor = texture(screenTexture, uv);
	}
	else if(bloom)
	{
		vec4 result;

		for (int i = -3; i < 3; i++)
		{
			for (int j = -4; j < 4; j++) result += texture(screenTexture, TexCoords + vec2(i, j) * 0.004) * 0.25;
		}
		if (texture(screenTexture, TexCoords).r < 0.3) FragColor = result * result * 0.012 + texture(screenTexture, TexCoords);
		else
		{
			if (texture(screenTexture, TexCoords).r < 0.5) FragColor = result * result * 0.009 + texture(screenTexture, TexCoords);
			else FragColor = result * result * 0.0075 + texture(screenTexture, TexCoords);
		}
	}
	else FragColor = texture(screenTexture, TexCoords);
}