Desarrollo de un motor de videojuegos estable, portable y fácilmente modulable,
con la capacidad para desarrollar juegos como Pokemon, Portal o Doom

Game Engine (MVP):  
 - [X] Setup inicial e instalación de dependencias  
 - [ ] Motor funcional para distintas plataformas  
     - [X] Linux  
     - [ ] Linux  
     - [ ] Android  
     - [ ] Mac  
 - [X] Logging system (spdlog.h)  
     - [ ] Backup del log  
     - [ ] Sacar el backup en un archivo al terminar la aplicación  
 - [x] Editor básico con ImGui  
 - [X] Input System  
 - [X] Event System  
     - [ ] Event buffer que guarde los eventos y los gestione en el Update, permite no tener que detener la aplicación para gestionar inmediatamente el evento que llegue y también la creación de acciones complejas para conjuntos de eventos  
 - [ ] Capacidad mínima estable de Render 2D mediante API agnostica  
 - [ ] Physics 2D  
 - [ ] ECS (Transform, Sprite/Render, Animation, Collider, Script...)  
     - [ ] TransformComponent  
     - [ ] Sprite/RenderComponent  
     - [ ] AnimationComponent (nodos en un futuro)  
     - [ ] ColliderComponent (active, gravity, mass, isTrigger, target...)  
     - [ ] ScriptComponent (Lua, C# en un futuro, C++ con dlls)
 - [ ] Sistemas para las entidades que tengan ciertos componentes  
     - [ ] MovementSystem (para 2D y 3D, ease in, ease out, snap, speed, configuración de inputs...)
 - [ ] Serializacion  
 - [ ] Scripting con Lua  
 - [ ] UILayer  
 - [ ] Audio  

Mejoras:  
 - [ ] Creación de entorno de desarrollo y proyecto  
 - [ ] Exportar ejecutable para diferentes plataformas desde el editor  
 - [ ] Capacidad mínima estable de Render 3D mediante API agnostica  
 - [ ] Capa de render para debug de gráficos, físicas, ui...  
 - [ ] Carga de modelos 3D  
 - [ ] Scripting con .NET C#  
 - [ ] Physics (3D) Bullet  
 - [ ] Tilemap Editor  
 - [ ] Layer para debug de render, fisicas...  
 - [ ] Sistema de animaciones y máquina de estados (con nodos?)  