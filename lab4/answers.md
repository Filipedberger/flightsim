# 1
## What kind of data does the heightmap image data hold? What range is the heightmap data?
Gråskalad bilddata. Där ljust betyder högt och mörkt betyder lågt. Värden mellan 0 och 255.
## The terrain is 4x4. How many polygons does it draw?
9 * 2 = 18

# 2
## Did you need to extend your camera movement code? How?
Nej vi använde oss av samma kod som i labb 3.

# 3
## Which method did you use to find the normal vector?
"The cross"

# 4
## How did you figure out what surface to use? 
We used a if stetment based on a calculation of the rounded float position value, and the value.

## How did you calculate the height from the surface?
We check which triangle in the quad that the object is in, then use barycentric coordinates to calculate the y value.

# 5
## What kind of multitexturing function did you implement?
Hieght based with a threshold for tecture change.

# 6
## How did you identify the water surface?
Checking the surface normal and the height of the surface.

