# waluigi-time
CS1230 Final Project Plan

Features:
VR (All 3)
We will be using the OpenVR APIs and “Device Pose” system to implement the functionality of
the controllers- we will be throwing the fireballs where the user holds down the controller
button to create the fireball, and released the button to release the fireball.

https://skarredghost.com/2018/03/15/introduction-to-openvr-101-series-what-is-openvr-and-how-to-get-started-with-its-apis/
https://github.com/ValveSoftware/openvr/tree/master/samples
https://github.com/osudrl/CassieVrControls/wiki/OpenVR-Quick-Start


Collision Detection (Dain)
Only the fireball will be moving, thus all the collision detection and physics will be built around just the
sphere of the fireball interacting with other objects. In this case, the other objects being targets and
environment. When the fireball hits a target, it and the target will be destroyed. If the fireball hits the
terrain, it will bounce twice being destroyed In the case of bouncing, it would be a matter of
determining the normal of the object being collider with at the collision point and bouncing similarly to
ray/intersect.
The fireball will move with basic physics attached to it - falling with gravity to move in an arc. The initial
speed and direction of the fireball will depend on the user’s controller movement before throwing. So
the initial velocity will be equal (or a scaled version) of the controller velocity.
Since the fireball is a sphere, to detect collisions, we need only check if an object exists within a radius
distance from the center of the sphere. To achieve this, we can loop through each object in the scene
and check if it’s within a certain distance. If it is, we can do a more intensive check to see if the object
shares any region with the fireball - causing a collision.
We could accomplish this using a similar system to intersect, by converting the object into the object
space of the sphere and checking collisions in that space.

Resources we will be using:
https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
http://www.peroxide.dk/papers/collision/collision.pdf
http://www.videotutorialsrock.com/opengl_tutorial/collision_detection/text.php


Deferred Lighting and Particles 
Particles should be simple, as we are essentially using what we learned from the particles lab, with
framebuffers, to simulate fire. What we will be doing here should be very similar, except in addition we
will be modifying the shape and color of the particles.
We will be implementing deferred lighting to help speed up the process of lighting the scene, because
having the fireball move around and therefore cause light to move with the fireball will be a relatively
expensive process, especially because we are working in a 3D scene. Therefore, I will be using deferred
lighting by essentially making a single pass to render the geometry of the scene first, store the
information in a collection of textures called the G-buffer, and then make another pass to light the
scene, and do this for every frame.

Resources we will be using:
Geometry rendering and lighting from class (like shapes and scenview)
https://learnopengl.com/Advanced-Lighting/Deferred-Shading
http://ogldev.atspace.co.uk/www/tutorial35/tutorial35.html


Terrain (Julie)
We will be making an outdoor terrain with tall, thin mountains, like the setting of the final fight in Avatar
the last Airbender. In addition to applying what Fwe learned from the terrain generation lab, we will be
using the 3-dimensional version of the midpoint displacement algorithm to procedurally generate the
terrain. We will be generating heightmaps, which we will then convert into 3D meshes, which we finally
will be rendering into 3D objects in our VR space. We will start with basic colors for the terrain, and
eventually apply shaders and texture mapping for more realistic effect.

Resources we will be using:
http://www.gamasutra.com/view/feature/1648/random_scattering_creating_.php?fbclid=IwAR1clKFAFb-pPd1KY5LzbXZnF0L368xN6J_ByCrrpd4rjggd_KzgCgjQkdw
http://pcg.wikidot.com/?fbclid=IwAR1SOZNfqH1Qg2cBm9aPEZv6N0AnaUflQqsLqFnG_2sI4GivvJdxo4cocxU
http://pcg.wikidot.com/category-pcg-algorithms?fbclid=IwAR0wAtNLRKRH5FVuM8tUPgyVV4mxQSmNHkC1CZ9Sbtp2Hdn8Ha9yCKhTJsc


Program Flow
The finished product will be a Virtual reality game in which the user is standing within a procedurally-
generated throwing fireballs at targets within a procedurally generated rocky terrain. The thrown
fireballs will essentially be spheres cloaked in particles to simulate a burning fire effect. These fireballsBrandon Li (bli31)
Julie Wang (jwang73)
Dain Woods (dwoods)
will collide with both terrain and targets, where they will bounce or be destroyed as mentioned in the
collision detection section. Lastly, the fireballs themselves will emit light that will light up nearby
sections of the terrain for more realistic effect.

Project timeline
11/26
Play around with VR stencil

11/28
Start working on project (Terrain, Collision Detection, Deferred Lighting)

12/3
Finish basic implementations-start combining code
Finish procedurally generated terrain- start working on adding color and lighting
Finish collision detection of sphere with arbitrary objects – combine with procedural generated
terrain
Finish code for sphere lighting up sceneview-like scene

12/5
Add particles and lighting to sphere to simulate fireball

12/7
Complete finishing touches- status update
Start working on adding in VR controls

12/12
Finish project
Start working on possible stretch features

12/15
Hand in final project
