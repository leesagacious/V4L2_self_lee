#include <linux/list.h>
#include "v4l2-dev-self.h"
/*
 * A linked list used to store video devices
 * No longer use arrays to store.
 */
static LIST_HEAD(video_self_device)
