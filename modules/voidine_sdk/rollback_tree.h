#pragma once

#include "scene/main/scene_tree.h"
#include "core/os/thread_safe.h"

class RollbackTree : public SceneTree {
	_THREAD_SAFE_CLASS_

	GDCLASS(RollbackTree, SceneTree);

private:
	bool is_on_physics_loop = false;

protected:
	static void _bind_methods();

public:
	virtual void iteration_prepare() override;
	virtual bool process(double p_time) override;

	virtual void before_iteration_loop();
	virtual void after_iteration_loop();

	// virtual void initialize() override;
	// virtual void finalize() override;
};
