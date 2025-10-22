#include "register_types.h"

#include "ring_buffer.h"
#include "rollback_tree.h"
#include "rollback_multiplayer.h"

void initialize_voidine_sdk_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		GDREGISTER_CLASS(CircularBuffer);
		GDREGISTER_CLASS(RollbackTree);
		GDREGISTER_CLASS(RollbackMultiplayer);
		if (GD_IS_CLASS_ENABLED(MultiplayerAPI)) {
			MultiplayerAPI::set_default_interface("RollbackMultiplayer");
		}
	}
}

void uninitialize_voidine_sdk_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {

	}
}
