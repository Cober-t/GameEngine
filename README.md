Desarrollo de un motor de videojuegos estable, portable y fácilmente modulable,
con la capacidad para desarrollar juegos como Pokemon, Portal o Doom

Game Engine (MVP):  

 - [X] Setup inicial e instalación de dependencias  

 - [ ] Motor funcional para distintas plataformas  
     - [X] Windows  
     - [ ] Linux  
     - [ ] Android  
     - [ ] Mac  

 - [X] Logging system (spdlog.h)  
     - [X] Backup del log  
     - [X] Sacar el backup en un archivo al terminar la aplicación  

 - [x] Editor básico con ImGui  
     - [X] ViewportPanel  
     - [x] Menu / Settings  
     - [x] Data    
     - [x] Console  
     - [x] Content Browser  
     - [x] Scene Hierarchy  
     - [x] Properties

 - [X] Input System  
     - [ ] Gamepads  
     - [x] Better KeyCodes  

 - [X] Event System  
     - [ ] Event buffer que guarde los eventos y los gestione en el Update, permite no tener que detener la aplicación para gestionar inmediatamente el evento que llegue y también la creación de acciones complejas para conjuntos de eventos  

 - [X] Capacidad mínima estable de Render 2D mediante API agnostica  
     - [X] Basic Render2D buffers and batching  
     - [X] Shaders  
     - [X] Load Textures  
     - [X] Framebuffer  
     - [X] Game Camera  
     - [X] Editor Camera  

 - [X] ECS
     - [X] Create Entity with unique UUID  
     - [X] TransformComponent  
     - [X] TagComponent  
     - [x] Sprite/RenderComponent  
     - [ ] AnimationComponent (nodos en un futuro)  
     - [X] ColliderComponent (active, gravity, mass, isTrigger, target...)  
     - [ ] ScriptComponent (Lua, C# en un futuro, C++ con dlls)

 - [ ] Sistemas para las entidades que tengan ciertos componentes  
     - [ ] MovementSystem (para 2D y 3D, ease in, ease out, snap, speed, configuración de inputs...)  

 - [X] Physics 2D  
 - [X] Scene  
 - [X] Serializacion / Deserializacion  
 - [ ] Scripting con Lua  
 - [ ] UILayer  
 - [ ] Audio  



Mejoras:  
 - [ ] Creación de entorno de desarrollo y proyecto  
 - [ ] Exportar ejecutable para diferentes plataformas desde el editor  
 - [ ] Capacidad mínima estable de Render 3D mediante API agnostica  
 - [ ] Capa de render para debug de gráficos, físicas, ui...  
 - [ ] Carga de modelos 3D  
 - [ ] Physics 3D (Bullet)  
 - [ ] Tilemap Editor  
 - [ ] Scripting con .NET C#  
 - [ ] Sistema de animaciones y máquina de estados (con nodos?)  