
struct video_self_device
{
	struct device self_dev;
	struct cdev *self_cdev;
};

int register_video_self_device(struct video_self_device *vsd);
