#include "rollback_multiplayer.h"

#include "core/os/os.h"

// poll can be executed manually by the user, or automatically in SceneTree before the process step
// if multiplayer_poll is set to true (default)
Error RollbackMultiplayer::poll() {
	// OS::get_singleton()->get_main_loop();

	return SceneMultiplayer::poll();
}

void RollbackMultiplayer::_bind_methods() {
}

RollbackMultiplayer::RollbackMultiplayer() {
}
