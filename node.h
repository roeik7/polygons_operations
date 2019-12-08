#ifndef NODE_H
#define NODE_H
#include "polygon.h"

struct _Node {
	polygon* poly;
	struct _Node * next;
};

typedef struct _Node Node;

#endif 