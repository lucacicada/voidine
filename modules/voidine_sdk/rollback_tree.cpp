#include "rollback_tree.h"

void RollbackTree::iteration_prepare() {
	if (!is_on_physics_loop) {
		is_on_physics_loop = true;
		before_iteration_loop(); // first frame of physics loop
	}

	SceneTree::iteration_prepare();
}

bool RollbackTree::process(double p_time) {
	if (is_on_physics_loop) {
		is_on_physics_loop = false;
		after_iteration_loop(); // first frame out of physics loop
	}

	return SceneTree::process(p_time);
}

void RollbackTree::before_iteration_loop() {
	// Custom logic before the iteration loop starts
}
void RollbackTree::after_iteration_loop() {
	// Custom logic after the iteration loop ends
}

// void RollbackTree::initialize() {
// 	SceneTree::initialize();
// 	// connect(SceneTree::node_added_name, callable_mp(this, &AnimationBezierTrackEdit::_zoom_changed));
// }

// void RollbackTree::finalize() {
// 	SceneTree::finalize();
// }

void RollbackTree::_bind_methods() {
}
