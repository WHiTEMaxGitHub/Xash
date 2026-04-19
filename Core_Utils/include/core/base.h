#pragma once

#define MAKE_UNIQUE_AST(node_type, node_ptr)\
	std::unique_ptr<node_type>{dynamic_cast<node_type*>(node_ptr)}