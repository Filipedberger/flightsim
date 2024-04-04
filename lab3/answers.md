# 1
## How can you get all four blades to rotate with just one time-dependent rotation matrix?
Visa kod

## How do you make the wings follow the body's movements?
Förstår inte frågan

# 2
## What kind of control did you implement?
Visa programmet. Vi flyttar på kameran och vart vi kollar.

## Can you make this kind of control in some other way than manipulating a "look-at" matrix?
Rotera modelen istället för kameran.

# 3
## How did you handle the camera matrix for the skybox?
Flytta skyboxen till camera pos

## How did you represent the objects? Is this a good way to manage a scene or would you do it differently for a "real" application?
Vi skapade nya modeller för varje objekt och laddade upp till GPUn. Detta är inte en bra lösning för en "real" applikation, det är nog bättre att generialisera genom att använda klasser.

## What special considerations are needed when rendering a skybox?
Culling & zbuffer avstängda innan de rendreras, och sedan sättas på.

## What is the problem with the “labskybox" object used in the lab? (The problem doesn't have to be corrected.)
Den täcker inte riktigt hela. Vi flyttade därför ner den något.


# 4
## Where is the camera in view coordinates?
I origo

## Why was blue facing the camera when visualizing the normal vectors?
Z-komponenten i normalen var 1 och då blir det blått. RGB

## For light source 3, why did we get a white area in the distance for the specular light but not for the diffuse?
För att den är directional, och inte bryr sig om avståndet.

## How do you generate a vector from the surface to the eye?
cameraPos - SurfacePos (0 - SurfacePos)
Räkna ut vektorn från kameran till ytan.

## Which vectors need renormalization in the fragment shader?
Varying. Normalen.



