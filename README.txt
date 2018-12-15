README Waluigi Time

Before playing, read your operations manual for important information about your health and safety. Allow adequate room around you during gameplay. Put on and tighten the wrist strap. Hold the remotes securely and do not let go of them. Press A to continue. (c) Nintendo.

Your goal is to smash as many targets as possible by chucking balls at them. To do this, you may choose either an underarm or overarm motion, but we warned that the latter will wear you out very quickly. Hold down the trigger button as you swing and release it to fire. The target will shatter if you hit it. Make sure your volume is set to a reasonable level.

The scene inherits from SceneView and thus utilizes the same m_phongShader that we used throughout the course of CS123. We decided to do this because much of the framework and setup code had already been done for us, and it fit in nicely with the new VR stencil code. The same as before, renderGeometry is called 60 times a second, which renders the scene.

We made use of the flyweight structure, never initializing more shapes (that contained VAOs) than we had to. Structs are stored instead that hold defining characteristics of each node, such as position, velocity, spawn time, and so forth. Uniforms are then applied and shapes drawn based on the stored data in the structs. Elements that disappear after a while, such as balls and the shatter effect, delete themselves when their lifespan is over, which simply results in them not being drawn anymore. Triangle strips were also used to speed up the efficiency of our code.

The features we included are collision detection, procedural generation, and of course VR. Collision is done in a similar way to Intersect, but includes more interaction with the environment (such as gravity and the shattering of targets). Procedural generation is based off of the poisson disk sampling algorithm, which creates a scattering effect for the many columns and targets. The columns themselves are randomized as well based off of offsets on their surfaces. The targets are also randomized. srand is called at the start to ensure every iteration creates a new scene.

p.s. you are Waluigi and you are angry that you didn't get put in Smash Ultimate but more Fire Emblem clones and devil plants and your enemey's girlfriend's best friend made it in. Have fun expelling your rage.
