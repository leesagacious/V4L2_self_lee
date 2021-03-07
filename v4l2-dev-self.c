#include <linux/list.h>
#include "v4l2-dev-self.h"
/*
 * A linked list used to store video devices
 * No longer use arrays to store.
 */
static LIST_HEAD(video_self_device)

int register_video_self_device(struct video_self_device *vsd)
{
	int ret;

	vsd->cdev = cdev_alloc();
	if (!vsd->cdev) {
		ret = -ENOMEM;
		goto cdev_failed;
	}

	vsd->cdev->ops = &vsd_fops;
	vsd->cdev->owner = owner;
	ret = cdev_add(vsd->cdev, MKDEV(), 1);
	if (ret < 0) {
		/*
		 * not use pr_err.
		 */
		pr_debug("cdev_add error %s\n", __func__);
		kfree(vsd->cdev);
		vsd->cdev = NULL;
		goto cdev_failed;
	}

	list_add_tail(&vsd->self_dev, &video_self_device);

	return ret;	
}	
