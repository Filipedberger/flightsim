# 1
## What kind of procedural texture did you make?
We created a procedual texture with the following mathematical equations. SEE LAB2-1 FRAG

# 2
## How are the textures coordinates mapped on the bunny? Can you see how they vary over the model?
Vi noterar att texturen ser normal ut på x-y planet och att den "viker sig" på de andra delar.

## How can you make a texture repeat multiple times over the bunny?
Multiplicera texturkoordinaterna

## Why can't we just pass the texture object to the shader? There is a specific reason for this, a limited resource. What? (No, it is not that we must avoid re-uploading from CPU. The texture object is on the GPU!)


# 3
## How did you move the bunny to get it in view?

Flyttade den in i skärmen. -z led


# 4
## Given a certain vector for v, is there some direction you can't look?
Nollvektorn
Parallellt med p-l


# 5
## Did you implement your light calculations in the vertex or fragment shader? So, which kind of shading did you implement?

I fragment shadern. Phong shading


## Some geometry data must be vec4, others are just as well vec3's. Which ones, and why? How about vertices, light source, normal vectors...?

Saker som behöver homogena coordinater har vec4. T.ex. vertices.


# 7
## If you rotate an object or rotate the camera, what matrices are affected?

Rotera kamera: Ändrar det som går in i view matrix.
Rotera model: Ändra rotation på model och light rotation.
