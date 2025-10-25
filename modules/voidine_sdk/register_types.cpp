#include "register_types.h"

#include "circular_buffer.h"
#include "rollback_multiplayer.h"
#include "rollback_synchronizer.h"
#include "rollback_tree.h"

void initialize_voidine_sdk_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(CircularBuffer);

		GDREGISTER_CLASS(RollbackTree);
		GDREGISTER_CLASS(RollbackMultiplayer);
		GDREGISTER_CLASS(RollbackSynchronizer);
		if (GD_IS_CLASS_ENABLED(MultiplayerAPI)) {
			MultiplayerAPI::set_default_interface("RollbackMultiplayer");
		}

		// ProjectSettings::get_singleton()->set("application/run/main_loop_type", "RollbackTree");
	}
}

void uninitialize_voidine_sdk_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
	}
}
