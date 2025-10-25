#pragma once

#include "core/os/thread_safe.h"
#include "core/os/time.h"
#include "rollback_synchronizer.h"
#include "scene/main/scene_tree.h"

class RollbackTree : public SceneTree {
	_THREAD_SAFE_CLASS_

	GDCLASS(RollbackTree, SceneTree);

private:
	static double get_system_time() {
		return OS::get_singleton()->get_ticks_usec() / 1'000'000.0;
	}

	struct SystemClock {
		double offset = 0;
	};

	struct SteppingClock {
		double time = 0;
		double last_time = 0;
	};

	SystemClock system_clock;
	SteppingClock stepping_clock;

	uint64_t _tick = 0;
	int _ticks_in_loop = 0;
	int _ticks_executed_in_loop = 0;

	double clock_stretch_factor = 1.0;
	double accumulator = 0.0;

	Vector<RollbackSynchronizer *> synchronizers;

protected:
	static void _bind_methods();

public:
	virtual int get_override_physics_steps() override;
	virtual void initialize() override;
	virtual void iteration_prepare() override;
	virtual void iteration_end() override;

	void add_synchronizer(RollbackSynchronizer *p_synchronizer) {
		ERR_FAIL_NULL(p_synchronizer);

		synchronizers.push_back(p_synchronizer);
	}
	void remove_synchronizer(RollbackSynchronizer *p_synchronizer) {
		ERR_FAIL_NULL(p_synchronizer);

		synchronizers.erase(p_synchronizer);
	}
};
