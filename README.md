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
