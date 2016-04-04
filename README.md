![](http://i.imgur.com/eaTWgUF.png)
![](http://i.imgur.com/0yXav4T.png)

# Box2D Raycasting Test

I undertook a wee experiment to see if you could use `b2World::RayCast` to render the Box2D world using old-school *Wolfenstein 3D*-style raycasting. You can!

([Here's a good tutorial on raycasting](http://lodev.org/cgtutor/raycasting.html), if you're wondering what this is all about.)

More screenshots on [imgur](http://imgur.com/a/ftIpR).

**Installation:** [SFML](http://www.sfml-dev.org/) 2.3.2 is used for window management and for easy graphics. The latest version of [Box2D](https://github.com/erincatto/Box2D), including the .lib and .pdb files, has been included in the repo for convenience.

Once you've downloaded the repo, the only thing you should need to do to build is to fix the Visual Studio project settings so that it knows where to find your copy of the SFML 2.3.2 headers and .libs. The .dlls are already in the /Debug and /Release folders. Then you *should* be good to go.

(I'm on Windows, using VS2013, but if you're not it should be possible to adapt.)

**What I haven't figured out yet:**
- How to texture the walls - without a way to figure out how far along the wall the ray hit point is, this is kinda hard.
- How sprites should be drawn.
- What kind of cool physicsy FPS I should make using this!

I'm probably going to spend a little time working out how to do the above, but I might not have the time. Either way, hopefully other people can use this code as a reference!

There should be a .zip you can download containing a build of the demo under 'Releases' on this here repo.

**Controls:**
- **Q** to switch between Box2D debug draw mode and raycasting mode
- **WASD** to move the camera
- **Left/Right Arrow** to rotate the camera
- **Z/X** to decrease/increase the viewing angle
- **F/G** to decrease/increase the image resolution
- **E** to toggle between the two ways that the distance from the camera to the ray hit point can be calculated:
 - 1) Euclidean distance
 - 2) Perpendicular distance
- **R** to toggle between the two ways that the ray direction can be calculated:
  - 1) Rotating the camera's forward vector
  - 2) Skewing the camera's forward vector along the viewing plane
