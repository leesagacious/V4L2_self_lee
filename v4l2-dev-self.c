// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021, leesagacious.
 */

#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/ktime.h>
#include "v4l2-dev-self.h"

static DEFINE_MUTEX(vsd_device_lock);
/*
 * A linked list used to store video devices
 * No longer use arrays to store.
 */
static LIST_HEAD(video_self_device)

int register_video_self_device(struct video_self_device *vsd)
{
	int ret;
	ktime_t invoke_time, delta, return_time;
	u64 microsecond;

	invoke_time = ktime_get();

	vsd->cdev = cdev_alloc();
	if (!vsd->cdev) {
		ret = -ENOMEM;
		goto delete_action;
	}

	vsd->cdev->ops = &vsd_fops;
	vsd->cdev->owner = owner;
	ret = cdev_add(vsd->cdev, MKDEV(), 1);
	if (ret < 0) {
		/*
		 * not use pr_err.
		 */
		dev_err(&vsd->self_dev, "cdev_add error %s\n", __func__);
		kfree(vsd->cdev);
		vsd->cdev = NULL;
		goto delete_action;
	}

	/*
	 * Mutex also implements the spin waiting mechanism.
	 */
	mutex_lock(&vsd_device_lock);
	list_add_tail(&vsd->self_dev, &video_self_device);
	mutex_unlock(&vsd_device_lock);

	ret = device_register(&vsd->dev);
	if (ret < 0) {
		dev_err(&vsd->self_dev, "device_register has failed : %s\n", __func__);
		goto delete_action;
	}

	return_time = ktime_get();

	delta = ktime_sub(return_time, invoke_time);
	microsecond = (u64)ktime_to_ns(delta) >> 10;

	dev_dbg("the time spent on register_video_self_device is: %lld\n", microsecond);

	return 0;

delete_action:

	return ret;
}
