#version 440 core

out vec4 FragColor;
in vec2 VertexUv;

uniform sampler2D screenQuad;

const float PIXEL_OFFSET = 1.0f / 330.0f;
const float CONTRAST = 0.29f;
const float PI = 3.1415926535;
uniform bool blurEffect;
uniform bool grayScaleEffect;
uniform bool thunderstormEffect;
uniform bool negativeEffect;
uniform bool shake;
uniform bool binoculur;
uniform bool fishEye;
uniform bool nightvision;
uniform float elapsedTime;

//test fire screen (alt partl)
uniform bool firescreen;
uniform sampler2D tex_noise;
uniform sampler2D tex_gradient;
uniform sampler2D tex_gradient_color;
uniform sampler2D tex_distortion;
uniform sampler2D tex_color;
uniform float alpha_threshold;
uniform float time;
uniform float speed;
uniform float delta;

uniform bool ripple;
uniform bool dithering;

uniform bool bloom;

uniform bool noise;
uniform bool sint;
uniform bool vigen;
uniform bool sobel;
uniform float width;
uniform float height;
float Scale = 1.0;
uniform bool radialblur;
uniform bool waverblur;
uniform bool waterBlur;



float find_closest(int x, int y, float c0)
{

	int dither[8][8] = {
	{ 0, 32, 8, 40, 2, 34, 10, 42}, /* 8x8 Bayer ordered dithering */
	{48, 16, 56, 24, 50, 18, 58, 26}, /* pattern. Each input pixel */
	{12, 44, 4, 36, 14, 46, 6, 38}, /* is scaled to the 0..63 range */
	{60, 28, 52, 20, 62, 30, 54, 22}, /* before looking in this table */
	{ 3, 35, 11, 43, 1, 33, 9, 41}, /* to determine the action. */
	{51, 19, 59, 27, 49, 17, 57, 25},
	{15, 47, 7, 39, 13, 45, 5, 37},
	{63, 31, 55, 23, 61, 29, 53, 21} }; 

	float limit = 0.0;
	if(x < 8)
	{
	limit = (dither[x][y]+1)/64.0;
	}


	if(c0 < limit)
	return 0.0;
	return 1.0;
}


float rand(vec2 n, float time)
{
  return 0.5 + 0.5* fract(sin(dot(n.xy, vec2(12.9898*time, 78.233*time)))* 43758.5453);
}
float WaverOffset(float pos, float length, float width, float speed) {
    return sin(pos * length + mod(time, 3.0) * speed) * width;
}

void main()
{	
	// Normal
	vec4 outputColor = texture(screenQuad, VertexUv);

	// Change contrast of texture so dark colors are slightly more darker and brighter colors are slightly brighter
	outputColor.rgb = (outputColor.rgb - 0.4f) * (1.0f + CONTRAST) + 0.4f;
	
	if (thunderstormEffect)
	{
		// Array of offsets for surrounding texture coordinates
		vec2 offsets[9] = vec2[]
		(
			vec2(-PIXEL_OFFSET, PIXEL_OFFSET),  // top left
			vec2( 0.0f, PIXEL_OFFSET), 			// top center
			vec2( PIXEL_OFFSET, PIXEL_OFFSET),  // top right
			vec2(-PIXEL_OFFSET, 0.0f),   		// center left
			vec2( 0.0f, 0.0f),   				// center
			vec2( PIXEL_OFFSET, 0.0f),   		// center right
			vec2(-PIXEL_OFFSET, -PIXEL_OFFSET), // bottom left
			vec2( 0.0f, -PIXEL_OFFSET), 		// bottom center
			vec2( PIXEL_OFFSET, -PIXEL_OFFSET)  // bottom right    
		);
		
		// A kernel filter that creates a simple blur effect
		float kernel[9] = float[]
		(
			0.0625f, 0.125f, 0.0625f,
			0.125f, 0.25f, 0.125f,
			0.0625f, 0.125f, 0.0625f
		);
		
		vec3 sampleTex[9];
		for(int i = 0; i < 9; ++i)
		{
			sampleTex[i] = vec3(texture(screenQuad, VertexUv.st + offsets[i]));
		}
		
		// Combine sampled colors
		for(int i = 0; i < 9; ++i)
		{
			outputColor.rgb += sampleTex[i] * kernel[i];
		}
		
		FragColor = vec4(outputColor.rgb, 1.0f);
	}
	
	if (blurEffect)
	{
		//// Array of offsets for surrounding texture coordinates
		vec2 offsets[9] = vec2[]
		(
			vec2(-PIXEL_OFFSET, PIXEL_OFFSET),  // top left
			vec2( 0.0f, PIXEL_OFFSET), 			// top center
			vec2( PIXEL_OFFSET, PIXEL_OFFSET),  // top right
			vec2(-PIXEL_OFFSET, 0.0f),   		// center left
			vec2( 0.0f, 0.0f),   				// center
			vec2( PIXEL_OFFSET, 0.0f),   		// center right
			vec2(-PIXEL_OFFSET, -PIXEL_OFFSET), // bottom left
			vec2( 0.0f, -PIXEL_OFFSET), 		// bottom center
			vec2( PIXEL_OFFSET, -PIXEL_OFFSET)  // bottom right    
		);
		//
		//// A kernel filter that creates a simple blur effect
		float kernel[9] = float[]
		(
			0.0625f, 0.125f, 0.0625f,
			0.125f, 0.25f, 0.125f,
			0.0625f, 0.125f, 0.0625f
		);
        //
		vec3 sampleTex[9];
		for(int i = 0; i < 9; ++i)
		{
			sampleTex[i] = vec3(texture(screenQuad, VertexUv.st + offsets[i]));
		}
		
		//// Combine sampled colors
		for(int i = 0; i < 9; ++i)
		{
			outputColor.rgb += sampleTex[i] * kernel[i];
		}
		
		FragColor = vec4(outputColor.rgb, 1.0f);
	}
	else if (grayScaleEffect)
	{
		// Grayscale: Remove all colors except black, white and gray (average all color components)
		
		FragColor = texture(screenQuad, VertexUv);
		float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0f;
		FragColor = vec4(average, average, average, 1.0f);
	}else if(negativeEffect){
		vec3 col = 1-texture(screenQuad, VertexUv).rgb;
		FragColor = vec4(col, 1.0);
	}else if(shake){
	//// Array of offsets for surrounding texture coordinates
		vec2 offsets[9] = vec2[]
		(
			vec2(-PIXEL_OFFSET, PIXEL_OFFSET),  // top left
			vec2( 0.0f, PIXEL_OFFSET), 			// top center
			vec2( PIXEL_OFFSET, PIXEL_OFFSET),  // top right
			vec2(-PIXEL_OFFSET, 0.0f),   		// center left
			vec2( 0.0f, 0.0f),   				// center
			vec2( PIXEL_OFFSET, 0.0f),   		// center right
			vec2(-PIXEL_OFFSET, -PIXEL_OFFSET), // bottom left
			vec2( 0.0f, -PIXEL_OFFSET), 		// bottom center
			vec2( PIXEL_OFFSET, -PIXEL_OFFSET)  // bottom right    
		);
		//
		//// A kernel filter that creates a simple blur effect
		float kernel[9] = float[]
		(
			0.1f, 0.125f, 0.1f,
			0.250f, 0.5f, 0.250f,
			0.1f, 0.125f, 0.1f
		);
        //
		vec3 sampleTex[9];
		for(int i = 0; i < 9; ++i)
		{
			sampleTex[i] = vec3(texture(screenQuad, VertexUv.st + offsets[i]));
		}
		
		//// Combine sampled colors
		for(int i = 0; i < 9; ++i)
		{
			outputColor.rgb += sampleTex[i] * kernel[i];
		}
		
		FragColor = vec4(outputColor.rgb, 1.0f);
		
	}else if(binoculur){
		vec2 lensRadius=vec2(0.45, 0.38);
		vec3 col = texture(screenQuad, VertexUv).rgb;
		//vec4 Color = texture2D(sceneTex, gl_TexCoord[0].xy);
		float dist = distance(VertexUv.xy, vec2(0.5,0.5));
		col *= smoothstep(lensRadius.x, lensRadius.y, dist);
		FragColor = vec4(col,1.0f);
	}else if(fishEye){
			 float aperture = 178.0;
		  float apertureHalf = 0.5 * aperture * (PI / 180.0);
		  float maxFactor = sin(apertureHalf);
		  vec2 uv;
		  vec2 xy = 2.0 * VertexUv.xy - 1.0;
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
		  else
		  {
			uv = VertexUv.xy;
		  }
		  vec4 c = texture(screenQuad, uv);
		  FragColor = c;
	
	}
	else if(nightvision){
		
		 float luminanceThreshold=0.2; // 0.2
		 float colorAmplification=4.0; // 4.0
		 vec2 uv;           
		uv.x = 0.4*sin(elapsedTime*50.0);                                 
		uv.y = 0.4*cos(elapsedTime*50.0);                                 
		//float m = texture(maskTex, VertexUv.st).r;
		vec3 c = texture(screenQuad, VertexUv.st).rgb;
  
		float lum = dot(vec3(0.30, 0.59, 0.11), c);
		if (lum < luminanceThreshold)
		  c *= colorAmplification; 
  
		vec3 visionColor = vec3(0.1, 0.95, 0.2);
		//FragColor.rgb = (c + (n*0.2)) * visionColor * m;
		FragColor.rgb = (c) * visionColor;
		 FragColor.a = 1.0;
	
	}else if(firescreen){
	  vec2 uv = VertexUv.xy * vec2(4.0, 1.0);
 
	  vec2 uv_noise = uv * 0.3;
 
	  uv_noise.y -= time * speed;
 
	  vec2 uv_grad = uv * vec2(1.0, 1.0);
 
	  vec2 uv_distor = uv;
	  vec3 tdistortion = texture(tex_distortion,uv_distor).rgb * 0.015;
 
	  vec2 uv_noise_distor = vec2(uv_noise.x + tdistortion.g, uv_noise.y + tdistortion.r);  
	  vec3 tnoise = texture(tex_noise, uv_noise_distor).rgb;
	  tnoise *= 1.75;
 
	  float tgradient = 1.0-texture(tex_gradient,uv_grad).r;
 
	  vec3 tgradientcolor = texture(tex_gradient_color,uv_grad).rgb;
 
	  vec3 tcolor = texture(tex_color,uv_noise_distor*3.0).rgb * vec3(2.0, 1.6, 1.2);
 
	  float alpha0 = (tnoise.x + tnoise.y + tnoise.z) / 3.0;
 
	  float flamewob = ((1.0 + (sin(time + uv.x*8.0) * cos(time - uv.x*5.0)))*0.5) * 0.30;
 
	  float thres = alpha_threshold * tgradient *2.15 + flamewob;
 
	  float alpha = 1.0-smoothstep(thres-0.15, thres+0.3, alpha0);  
 
 
	  vec3 flame = clamp(tnoise * 2.1, 0.0, 1.0) * tgradientcolor; 
	  vec3 color = mix(tcolor, flame, (alpha-alpha0)*1.5);
 
	  FragColor = vec4(color, alpha);
	
	
	}else if(ripple){
		float radius = .5;
		float t = clamp(time / 6., 0., 1.);
 
		vec2 coords = VertexUv.st;
		vec2 dir = coords - vec2(.5);
	
		float dist = distance(coords, vec2(.5));
		vec2 offset = dir * (sin(dist * 80. - time*15.) + .5) / 30.;
 
		vec2 texCoord = coords + offset;
		vec4 diffuse = texture(screenQuad, texCoord);
 
		vec4 mixin = texture(screenQuad, texCoord);
 
 		FragColor = mixin * t + diffuse * (1. - t);
	}else if(dithering){
		vec4 lum = vec4(0.299, 0.587, 0.114, 0);
		float grayscale = dot(texture(screenQuad, VertexUv.xy), lum);
		vec3 rgb = texture(screenQuad, VertexUv.xy).rgb;

		vec2 xy = FragColor.xy * Scale;
		int x = int(mod(xy.x, 8));
		int y = int(mod(xy.y, 8));

		vec3 finalRGB;
		finalRGB.r = find_closest(x, y, rgb.r);
		finalRGB.g = find_closest(x, y, rgb.g);
		finalRGB.b = find_closest(x, y, rgb.b);

		float final = find_closest(x, y, grayscale);
		FragColor = vec4(finalRGB, 1.0);
	
	}else if(bloom){
	   vec4 sum = vec4(0);
	   vec2 texcoord = vec2(VertexUv);
	   int j;
	   int i;

	   for( i= -4 ;i < 4; i++)
	   {
			for (j = -3; j < 3; j++)
			{
				sum += texture(screenQuad, texcoord + vec2(j, i)*0.004) * 0.25;
			}
	   }
		   if (texture(screenQuad, texcoord).r < 0.3)
		{
		   FragColor = sum*sum*0.012 + texture(screenQuad, texcoord);
		}
		else
		{
			if (texture(screenQuad, texcoord).r < 0.5)
			{
				FragColor = sum*sum*0.009 + texture(screenQuad, texcoord);
			}
			else
			{
				FragColor = sum*sum*0.0075 + texture(screenQuad, texcoord);
			}
		}
	
	}
	else if(noise){
	  float y = rand(VertexUv.xy, time);
	  if (y > 0.975)
	  {
		float x = rand(VertexUv.xy, time);
		FragColor = vec4(x, x, x, 1.0);
	  }else
	  {
		FragColor = texture2D(screenQuad,  VertexUv);
	  }
	
	}else if(sint){
		//Простой но классный эффект на равне с коллебаниями (ripple)
		vec2 test = VertexUv  + (sin(VertexUv*50)*0.01); 
		vec4 color = texture(screenQuad,  test);
		FragColor = vec4(color);
	}else if(sobel){
	    vec4 samplez[9];
		vec2 tc_offset[9];

		int count = 0;
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				tc_offset[count++] = vec2(i/width,j*height);
			}
		}

		for (int i = 0; i < 9; i++)
		{
			samplez[i] = texture(screenQuad, VertexUv.st + tc_offset[i]);
		}

	//    -1 -2 -1       1 0 -1 
	// H = 0  0  0   V = 2 0 -2
	//     1  2  1       1 0 -1
	//
	// result = sqrt(H^2 + V^2)

		vec4 horizEdge = samplez[2] + (2.0*samplez[5]) + samplez[8] -
						 (samplez[0] + (2.0*samplez[3]) + samplez[6]);

		vec4 vertEdge = samplez[0] + (2.0*samplez[1]) + samplez[2] -
						(samplez[6] + (2.0*samplez[7]) + samplez[8]);

		FragColor.rgb = sqrt((horizEdge.rgb * horizEdge.rgb) + 
								(vertEdge.rgb * vertEdge.rgb));
		FragColor.a = 1.0;
	}else if(radialblur){
		 vec2 center=vec2(0.5,0.5);
		 float radius=0.5;
		 float dist=0.2;
		 float intensity=1;
		 float count=5;
	    vec2 vec = VertexUv.st - center;
		float len = length(vec);
		float fade = smoothstep(0, radius, len);
		vec2 dir = normalize(vec) * dist;
		float factor = len * 0.1 * intensity;
		dir *= factor;

		vec4 sum = vec4(0.0);
		vec2 v = vec2(0.0);

		for(int i = 0; i < count; i++) {
			v = dir * i;
			sum += texture(screenQuad, VertexUv.st + v);
			sum += texture(screenQuad, VertexUv.st - v);
		}
		sum *= 1.0 / (count * 2.0);

		vec4 color = mix(texture(screenQuad, VertexUv.st), sum, fade*intensity);
		FragColor = color;
	
	}else if(waverblur){
		vec2 length=vec2(20,5);
		vec2 speed=vec2(40,15);
		vec2 width=vec2(0.003,0.2);
		float offsetX = WaverOffset(VertexUv.s, length[0], width[0], speed[0]);
		float offsetY = WaverOffset(VertexUv.t, length[1], width[1], speed[1]); 

		vec2 uv = VertexUv.st + vec2(offsetX, offsetY);

		vec4 color = texture(screenQuad, uv); 
		FragColor = vec4(color.rgb, 1.0);
	
	}else if(waterBlur){

		 vec2 center=vec2(0.5,0.5);
		 float freq=24;
		 float amount=0.03;
		 float speed=3;
		vec2 coord = VertexUv.st - center;
		float len = length(coord);

		vec2 uv = VertexUv.st + coord * amount * abs(cos(len*freq - time*speed)) / len;
    
		vec4 color = texture(screenQuad, uv);
		FragColor = color;
	}
	else 
	{
		FragColor = vec4(outputColor.rgb, 1.0f);
		/*
		игра с гаммой
		float gamma = 0.8;
		FragColor.rgb = pow(outputColor.rgb, vec3(1.0/gamma));
		*/
	}
} 