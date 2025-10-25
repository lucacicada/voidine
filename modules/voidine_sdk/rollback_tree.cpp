#include "rollback_tree.h"

void RollbackTree::initialize() {
	SceneTree::initialize();

	const double system_time = get_system_time();

	system_clock.offset = 0;

	stepping_clock.time = system_time;
	stepping_clock.last_time = system_time;

	accumulator = system_time;
}

// deterministic lockstep with rollover
int RollbackTree::get_override_physics_steps() {
	const double physics_step = 1.0 / Engine::get_singleton()->get_physics_ticks_per_second();
	const int max_physics_steps = Engine::get_singleton()->get_max_physics_steps_per_frame();

	const double system_time = get_system_time();

	const double elapsed = system_time - stepping_clock.last_time;
	stepping_clock.last_time = system_time;

	// step the clock forward, scaled by stretch factor
	stepping_clock.time += (elapsed * clock_stretch_factor);

	double clock_diff = (system_time + system_clock.offset) - stepping_clock.time;

	const double gain = 0.01;
	const double min_diff = 0.001;
	const double clock_stretch_max = 1.05;
	const double clock_stretch_min = 1.0 / 1.05;
	const double smoothing_alpha = 0.1;

	const double target_stretch = CLAMP(1.0 + gain * clock_diff, clock_stretch_min, clock_stretch_max);
	const double mask = double(std::abs(clock_diff) >= min_diff);

	// linear smoothing
	clock_stretch_factor = Math::lerp(clock_stretch_factor, target_stretch, smoothing_alpha * mask);

	int ticks_in_loop = 0;
	while (accumulator < stepping_clock.time && ticks_in_loop < max_physics_steps) {
		accumulator += physics_step;
		++ticks_in_loop;
	}

	_ticks_in_loop = ticks_in_loop;
	_ticks_executed_in_loop = 0;

	return ticks_in_loop;
}

// before physic sync, in_physics = true
void RollbackTree::iteration_prepare() {
	SceneTree::iteration_prepare();

	// if (_ticks_executed_in_loop == 0) // this is the first tick
}

// in full physics sync, in_physics = true
// RollbackTree::physics_process

// after physic sync, in_physics = true
void RollbackTree::iteration_end() {
	SceneTree::iteration_end();

	++_tick;
	++_ticks_executed_in_loop;
	// if (_ticks_executed_in_loop == _ticks_in_loop) // this was the last tick
}

// after physic loop, in_physics = false
// RollbackTree::process

void RollbackTree::_bind_methods() {
}
