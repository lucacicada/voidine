#pragma once

#include "scene/main/node.h"

class RollbackSynchronizer : public Node {
	GDCLASS(RollbackSynchronizer, Node)

protected:
	static void _bind_methods();

	void _notification(int p_what);
};
