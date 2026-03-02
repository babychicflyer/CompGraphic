# CompGraphic
CS-330-10508-M01 Comp Graphic and Visualization 2026 C-1 (Jan - Mar)
🖥️ CS‑330 Final Project — 3D Workspace Scene
🎯 Overview
This project is a 3D desktop workspace rendered with OpenGL and C++, created as the final project for Computational Graphics and Visualization (CS‑330).
It demonstrates practical skills in 3D modeling, lighting, textures, materials, and object transformation within an OpenGL pipeline.

The scene includes:

A wooden desk
A monitor and keyboard
A desk lamp with a soft red glow
A coffee mug
All elements are constructed from fundamental geometric shapes such as boxes, cylinders, cones, spheres, and tori.

🧱 Features
Feature	Description
Low‑poly modeling	Each object is built using basic mesh primitives for efficient real‑time rendering.
Texturing	Realistic textures applied to the desk (wood), monitor (metal), and screen (digital display).
Material properties	Each surface has custom diffuse, specular, and shininess values.
Lighting system	Directional lighting plus two point lights — including a soft red lamp light.
Transformations	Accurate scaling, translation, and rotation using GLM matrices.
💡 Lighting Setup
Three lights define the mood of the scene:

Directional light – general illumination from above.
Cool point light – subtle blue fill light from the right.
Lamp light – soft red glow emitted from the desk lamp bulb.
The lamp’s red light adds warmth and enhances visual contrast with the surrounding cool lighting.

cpp


// Red lamp light example
m_pShaderManager->setVec3Value("pointLights[1].ambient", 0.25f, 0.05f, 0.05f);
m_pShaderManager->setVec3Value("pointLights[1].diffuse", 0.8f, 0.1f, 0.1f);
m_pShaderManager->setVec3Value("pointLights[1].specular", 0.6f, 0.2f, 0.2f);
🧩 Object Breakdown
Desk – Box mesh with wood texture and semi‑gloss finish.
Monitor – Box and plane combination with glossy screen texture.
Keyboard – Simple box mesh with matte black material.
Lamp – Multi‑part model (cylinder base, angled pole, cone shade, and bulb).
Mug – Cylinder body with torus handle and ceramic material.
Each object includes realistic scaling and position within the 3D world.

🗂️ Project Structure


CS330-3DWorkspace/
├── src/
│   ├── SceneManager.cpp
│   ├── SceneManager.h
│   ├── ShaderManager.cpp
│   ├── ShapeMeshes.cpp
│   └── main.cpp
├── textures/
│   ├── desk_wood.jpg
│   ├── monitor.jpg
│   ├── screen.jpg
│   └── metal.jpg
├── ProjectReflection.docx
├── CS330_Final.exe
└── README.md
⚙️ How to Run
Open the Visual Studio project or solution file (.sln).
Build the project in Release or Debug mode.
Ensure the /textures folder is in the same directory as the executable.
Run the program — use mouse and keyboard for free‑camera movement:
WASD – forward/backward/strafe
Q/E – rise/lower the camera
Mouse – look around the scene
🔧 Technologies Used
C++ and OpenGL
GLM (OpenGL Mathematics Library)
stb_image for texture loading
Visual Studio 2022 development environment
🧠 Key Learning Outcomes
Applying Phong and Blinn‑Phong lighting models.
Using GLM transformations for matrix operations (scale, rotate, translate).
Building complex shapes from primitive meshes.
Managing textures and materials in modular shader architecture.
Understanding the interplay of light, material, and geometry in 3D scenes.
📸 Scene Preview
(You can insert a screenshot here if you’d like — for example:)

[Image blocked: 3D Workspace Preview]

🪶 Reflection Summary
This project provided deeper understanding of how geometry, lighting, and material systems work together to create realistic 3D environments.
The red lamp light became the focal design element, adding mood and warmth to an otherwise neutral desktop composition.



# CS-330: Computational Graphics and Visualization
## 3D Still-Life Scene — Final Project README

---

## 1. How Do I Approach Designing Software?

### What new design skills has your work on the project helped you to craft?

Working on this 3D still-life scene helped me develop a stronger sense of spatial reasoning and visual composition. Before this project, I thought about software design almost entirely in terms of logic and data flow. Building a 3D scene forced me to also think about how objects relate to each other physically — their proportions, placement on a surface, and how light interacts with different materials. I gained hands-on experience translating a real-world reference image into a structured set of geometric objects, which required careful planning before writing a single line of code. This strengthened my ability to break a complex visual goal down into discrete, manageable components.

### What design process did you follow for your project work?

I followed an iterative, reference-based design process. I began by selecting a real-world still-life composition and identifying the individual objects I would need to recreate. From there, I sketched out which primitive shapes — cylinders, cubes, planes — could approximate each object. I then built the scene incrementally, starting with basic geometry and gradually layering in textures, materials, and lighting. Each milestone served as a checkpoint where I evaluated the scene against my reference image and made adjustments before moving forward. This kept the work organized and prevented small errors from compounding into larger structural problems later on.

### How could tactics from your design approach be applied in future work?

The practice of breaking a complex end goal into visual and functional components applies directly to software engineering. Whether designing a user interface, a system architecture, or a data pipeline, starting from a clear reference point — a wireframe, a spec document, a user story — and building iteratively toward it is a transferable skill. The habit of validating work at each milestone rather than only at the end is something I intend to carry into all future development projects.

---

## 2. How Do I Approach Developing Programs?

### What new development strategies did you use while working on your 3D scene?

One of the most important strategies I applied was modular construction — building each object in the scene as a self-contained unit before assembling everything together. This made it much easier to isolate bugs and make changes to individual objects without breaking the rest of the scene. I also made heavier use of transformation matrices than I had in previous work, learning to chain translations, rotations, and scaling operations to position objects precisely. Additionally, I became more deliberate about managing shader programs and texture bindings, keeping track of which resources were active at any given point in the rendering pipeline.

### How did iteration factor into your development?

Iteration was central to how I developed this project. Nearly every object went through multiple versions — adjusting UV coordinates after seeing how a texture actually mapped onto a mesh, tweaking light positions after observing unexpected shadows, or resizing objects after placing them in the scene and realizing the proportions were off. Rather than trying to get everything right on the first pass, I treated each build as a draft that I would refine. This mindset made the development process feel less frustrating and more productive, since I always had a working version of the scene to build on.

### How has your approach to developing code evolved throughout the milestones?

At the beginning of the course, I tended to write code in long, monolithic blocks and test only at the end. Over the course of the milestones, I shifted toward writing smaller, focused functions and testing frequently as I went. I also became more consistent about organizing my code — separating scene setup, object rendering, and input handling into distinct sections. By the final project, I was thinking ahead about how future changes might affect the codebase, and I structured things accordingly. This is a meaningful shift in how I approach development and one I expect to keep building on.

---

## 3. How Can Computer Science Help Me in Reaching My Goals?

### How do computational graphics and visualizations give you new knowledge and skills that can be applied in your future educational pathway?

This course gave me a concrete understanding of how rendering pipelines work at a low level — from vertex buffers and shader programs to texture mapping and lighting models. That knowledge will be valuable in future coursework involving computer graphics, game development, or simulation. Understanding what happens under the hood of a graphics system makes it much easier to learn higher-level tools like game engines or visualization libraries, because the foundational concepts are already in place. It also strengthened my linear algebra and matrix math skills in an applied context, which will carry over into machine learning, computer vision, and other areas of computer science.

### How do computational graphics and visualizations give you new knowledge and skills that can be applied in your future professional pathway?

As someone pursuing a career in software development, understanding graphics programming opens doors to a broader range of projects and industries. Many software products — from data dashboards to AR/VR applications to game tooling — rely on rendering and visualization at some level. Being comfortable with how 3D scenes are constructed, how shaders work, and how real-time rendering pipelines are structured makes me a more versatile developer. It also improves my ability to collaborate with designers and technical artists, since I now have the vocabulary and practical experience to understand their work and communicate effectively across disciplines.
