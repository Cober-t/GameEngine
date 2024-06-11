Desarrollo de un motor de videojuegos estable, portable y fácilmente modulable,
con la capacidad para desarrollar juegos como Pokemon, Portal o Doom

Game Engine (MVP):  

 - [x] Setup inicial e instalación de dependencias  

 - [ ] Motor funcional para distintas plataformas  
     - [x] Windows  
     - [ ] Linux  
     - [ ] Android  

 - [x] Logging system (spdlog.h)  
     - [x] Backup del log  
     - [x] Sacar el backup en un archivo al terminar la aplicación  

 - [x] Editor básico con ImGui  
     - [x] ViewportPanel  
     - [x] Menu / Settings  
     - [x] Data    
     - [x] Console  
     - [x] Content Browser  
     - [x] Scene Hierarchy  
     - [x] Properties

 - [x] Input System  
     - [ ] Gamepads  
     - [x] Better KeyCodes  

 - [x] Event System  
     - [ ] Event buffer que guarde los eventos y los gestione en el Update, permite no tener que detener la aplicación para gestionar inmediatamente el evento que llegue y también la creación de acciones complejas para conjuntos de eventos  

 - [x] Capacidad mínima estable de Render 2D mediante API agnostica  
     - [x] Basic Render2D buffers and batching  
     - [x] Shaders  
     - [x] Load Textures  
     - [x] Framebuffer  
     - [x] Game Camera  
     - [x] Editor Camera  

 - [x] ECS
     - [x] Create Entity with unique UUID  
     - [x] TransformComponent  
     - [x] TagComponent  
     - [x] Sprite/RenderComponent  
     - [ ] AnimationComponent (nodos en un futuro)  
     - [x] ColliderComponent (active, gravity, mass, isTrigger, target...)  
     - [x] NativeScriptComponent
     - [ ] ScriptComponent (Lua, C# en un futuro)

 - [ ] Sistemas para las entidades que tengan ciertos componentes  
     - [ ] MovementSystem (para 2D y 3D, ease in, ease out, snap, speed, configuración de inputs...)  

 - [x] Physics 2D  
 - [x] Scene  
 - [x] Serializacion / Deserializacion  
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