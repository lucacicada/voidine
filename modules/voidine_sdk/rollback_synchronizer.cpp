#include "rollback_synchronizer.h"
#include "rollback_tree.h"

void RollbackSynchronizer::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			RollbackTree *rollback_tree = Object::cast_to<RollbackTree>(get_tree());
			if (rollback_tree) {
				// rollback_tree->add_synchronizer(this);
			}
			break;
		}
		case NOTIFICATION_EXIT_TREE: {
			RollbackTree *rollback_tree = Object::cast_to<RollbackTree>(get_tree());
			if (rollback_tree) {
				// rollback_tree->remove_synchronizer(this);
			}
			break;
		}
	}
}

void RollbackSynchronizer::_bind_methods() {
}
