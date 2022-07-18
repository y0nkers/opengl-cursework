## What it is?
### This is a OpenGL app with simple interactive 3d scene that I developed as part of my coursework. 
#### Basically all the material was taken from my professor's [playlist](https://www.youtube.com/watch?v=GnoauFXoxwo&list=PLfyiNADFEW5dE5hJ4xWGbIOGu3V2k3iru) and [LearnOpenGL](https://learnopengl.com/), but for more complex things like 3D picking i used the [OGLDEV material](https://www.youtube.com/c/OGLDEV/videos)
![Intro](https://user-images.githubusercontent.com/75331348/179569417-03692806-b9cb-4cf9-a3af-139f176dc852.png)
### P.S. Some comments (and the text in the console in full) may not be in English, because i was too lazy to translate them, sorry :innocent:
## What can it do?
### The game engine has the following features:
- Tesselation models
- 3D Picking 
- Postprocessing effects

## Dependencies:
- [GLEW library](http://glew.sourceforge.net)
- [GLFW library](https://www.glfw.org)
- [GLM library](https://glm.g-truc.net)
- [Assimp library](https://github.com/assimp/assimp)
- [STB_IMAGE library](https://github.com/nothings/stb)

## Controls:
### Camera Movement: 
- WASD + PgDown/PgUp for Camera Down/Up
### Misc:
- Space - Wireframe mode (wireframes will not be displayed when postprocessing is enabled)
- C - Change cursor mode (for picking objects)
- Z - NoClip (free camera movement on Y-axis)
- R - Reset models rotation
- P - Toggle postprocessing effects
### Postprocessing effects (only 1 effect at a time):
- 1 - Grayscale
- 2 - Blur
- 3 - Negative
- 4 - Fisheye
- 5 - Bloom
### Arrows 	←↑↓→	 for objects rotating
## Screenshots
### Grayscale effect
![Grayscale](https://user-images.githubusercontent.com/75331348/179575714-651e1f38-0be8-4e0d-989a-756d41c4f72f.png)
### Blur effect
![Blur](https://user-images.githubusercontent.com/75331348/179575844-011219ff-d7bf-4613-9188-582d1b495e5f.png)
### Negative effect 
![Negative](https://user-images.githubusercontent.com/75331348/179575879-7b87038c-b494-46cd-a095-d4201a8d9a95.png)
### Fisheye effect
![Fisheye](https://user-images.githubusercontent.com/75331348/179575938-71987afd-ac3e-4ada-9a52-d3ccaee89040.png)
### Bloom effect
![Bloom](https://user-images.githubusercontent.com/75331348/179575979-184905dd-0bf5-432a-8664-bfebe21a8dcd.png)
### Object selecting 
![Object](https://user-images.githubusercontent.com/75331348/179576108-452748ca-2200-4cbb-8771-8f7b8eb8ff69.png)
### Small interactive: You open a mysterious box by rotating things and get a prize - a juicy watermelon! :watermelon:
![Interactive](https://user-images.githubusercontent.com/75331348/179576386-c9d0ca9a-4183-4538-8033-c83edcca7f99.png)
