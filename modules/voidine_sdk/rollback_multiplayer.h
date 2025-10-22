#pragma once

#include "scene/main/multiplayer_api.h"
#include "modules/multiplayer/scene_multiplayer.h"

class RollbackMultiplayer : public SceneMultiplayer {
	GDCLASS(RollbackMultiplayer, SceneMultiplayer);


protected:
	static void _bind_methods();

public:
	virtual Error poll() override;

	RollbackMultiplayer();
};
