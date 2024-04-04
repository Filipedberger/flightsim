# 1
## Where is the origin placed in the on-screen coordinate system?
I the center

## Which direction are the X and Y axes pointing in the on-screen coordinate system?
X: Right

Y: Up
# 2
## What is the purpose of the "in", "out" and "uniform" modifiers?
in: Attribute variables

out: Variables that are interpolated between verticies

uniform: Same matrix applied to all verticies

## What is the output of the fragment shader?
The color of the fragment that is sent to the frame buffer

## What does the function glUniformMatrix4fv do?
It specifies the value of an uniform variable

# 3
## What is the frame rate of the animation?
100 FPS

# 4
## Did you need to do anything different when uploading the color data?
Modifera så att vi skickar färgdata igenom vertex shader till fragment shadern.

## The "in" and "out" modifiers are now used for something different. What?

# 5
## What problems did you encounter while building the pyramid?
Fel orientering och ordning av vertices. glDrawArrays

## How do you change the facing of a polygon?
Ändra ordningen.

# 6
## Why do we need normal vectors for a model?
För att veta vilka ytor som är synliga.

## What did you do in your fragment shader?
Normal som inargument, abs(normal)

## Should a normal vector always be perpendicular to a certain triangle? If not, why?
Yes

## Now we are using glBindBuffer and glBufferData again. They deal with buffers, but in what way?
glBindBuffer: Bind the named buffer object to the buffer array.

glBufferData: Fills the specified object buffer with data.

