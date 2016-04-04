# Box2D Raycasting Test

I undertook a wee experiment to see if you could use `b2World::RayCast` to render the Box2D world using old-school Wolfenstein3D-style raycasting. You can!

**Installation**: [SFML](http://www.sfml-dev.org/) 2.3.2 is used for window management and for easy graphics. The latest version of [Box2D](https://github.com/erincatto/Box2D), including the .lib and .pdb files, has been included in the repo for convenience.

Once you've downloaded the repo, the only thing you should need to do to build is to fix the Visual Studio project settings so that it knows where to find your copy of the SFML 2.3.2 headers and .libs. Then you *should* be good to go.

What I haven't figured out yet:
- How to texture the walls - without a way to figure out how far along the wall the ray hit point is, this is kinda hard.
- How sprites should be incorporated.
- What kind of cool physicsy FPS I should make using this!

Hopefully other people can use this code for fun and profit!

You can download a build from here: https://www.dropbox.com/s/t6t2l3zmqxqjmok/box2d_raycasting_test.zip?dl=0

Controls:
- **Q** to switch between Box2D debug draw mode and raycasting mode
- **WASD** to move the camera
- **Left/Right Arrow** to rotate the camera
- **Z/X** to decrease/increase the viewing angle
- **F/G** to decrease/increase the image resolution
- **E** to toggle between the two ways that the distance from the camera to the ray hit point can be calculated: 1) Euclidean distance or 2) perpendicular distance
- **R** to toggle between the two ways that the ray direction can be calculated: 1) by rotating the camera's forward vector, or 2) by skewing it along the viewing plane vector
