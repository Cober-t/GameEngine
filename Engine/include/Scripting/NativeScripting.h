#include "Core/Core.h"
#include "Events/Event.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"

namespace Cober {
    
 	typedef void (*InitScriptsFn)(Scene* scene, Entity entity);
	typedef void (*UpdateScriptFn)(Scene* scene, float dt);
	typedef void (*EventScriptFn)(Scene* scene, Event& event);
  	typedef void (*NotifyBeginContactFn)(Entity* entityA, Entity* entityB);
	typedef void (*NotifyEndContactFn)(Entity* entityA, Entity* entityB);
	typedef void (*DeleteScriptsFn)(Scene* scene);

    namespace NativeScriptFn 
	{
	    CB_API void Init(Scene* scene);
		CB_API void InitEntity(Scene* scene, Entity entity);
        CB_API void Update(Scene* scene, float dt);
		CB_API void OnEvent(Scene* scene, Event& event);
		CB_API void NotifyBeginContact(Entity* entityA, Entity* entityB);
		CB_API void NotifyEndContact(Entity* entityA, Entity* entityB);
		CB_API void DeleteScripts(Scene* scene);

		CB_API bool FreeScriptLibrary();
    }
}