/*******************************************************************
 *   _________     _____      _____    ____  _____    ___  ____    *
 *  |_   ___  |  |_   _|     |_   _|  |_   \|_   _|  |_  ||_  _|   *
 *    | |_  \_|    | |         | |      |   \ | |      | |_/ /     *
 *    |  _|        | |   _     | |      | |\ \| |      |  __'.     *
 *   _| |_        _| |__/ |   _| |_    _| |_\   |_    _| |  \ \_   *
 *  |_____|      |________|  |_____|  |_____|\____|  |____||____|  *
 *                                                                 *
 *******************************************************************
 *                                                                 *
 *  fLink userspace library, base functionality                    *
 *                                                                 *
 *******************************************************************/
 
#include "flinklib.h"
#include "flinkioctl.h"
#include "types.h"
#include "valid.h"
#include "error.h"
#include "log.h"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


/*******************************************************************
 *                                                                 *
 *  Internal (private) methods                                     *
 *                                                                 *
 *******************************************************************/

/**
 * @brief Read number of subdevices from fink device.
 * 
 * @param dev Device to read
 * @return int Number of flink devices or -1 in case of error.
 */
static int read_nof_subdevices(flink_dev* dev) {
	uint8_t n = 0;
	
	dbg_print("reading number of subdevices...\n");
	
	if(flink_ioctl(dev, READ_NOF_SUBDEVICES, &n) < 0) {
		dbg_print("   --> failed!\n");
		libc_error();
		return EXIT_ERROR;
	}
	dbg_print("  --> %u\n", n);
	return n;
}

/**
 * @brief Read header of all subdevices and update flink device.
 * 
 * @param dev flink device to update
 * @return int number of subdevices read, or -1 in case of error.
 */
static int get_subdevices(flink_dev* dev) {
	flink_subdev* subdev = NULL;
	int i = 0, ret = 0;
	
	if(!validate_flink_dev(dev)) {
		flink_error(FLINK_EINVALDEV);
		return EXIT_ERROR;
	}
	
	// Read nof subdevices
	dev->nof_subdevices = read_nof_subdevices(dev);
	
	// Allocate memory
	dev->subdevices = calloc(dev->nof_subdevices, sizeof(flink_subdev));
	if(dev->subdevices == NULL) { // allocation failed
		libc_error();
		dev->nof_subdevices = 0;
		return EXIT_ERROR;
	}
	
	// Fillup all information
	for(i = 0; i < dev->nof_subdevices; i++) { // for each subdevice
		subdev = dev->subdevices + i;
		subdev->id = i;
		ret = flink_ioctl(dev, READ_SUBDEVICE_INFO, subdev);
		subdev->parent = dev;
	}
	
	return ret;
}


/*******************************************************************
 *                                                                 *
 *  Internal (private) methods                                     *
 *                                                                 *
 *******************************************************************/

/**
 * @brief Opens a flink device file
 * 
 * @param file_name device file (null terminated array)
 * @return flink_dev* pointer to the opened flink device or NULL in case of error.
 */
flink_dev* flink_open(const char* file_name) {
	flink_dev* dev = NULL;
	
	// Allocate memory for flink_t
	dev = malloc(sizeof(flink_dev));
	if(dev == NULL) { // allocation failed
		libc_error();
		return NULL;
	}
	
	// Open device file
	dev->fd = open(file_name, O_RDWR);
	if(dev->fd < 0) { // failed to open device
		free(dev);
		libc_error();
		return NULL;
	}
	
	if(get_subdevices(dev) < 0) { // reading subdevices failed
		close(dev->fd);
		free(dev);
		return NULL;
	}
	
	return dev;
}


/**
 * @brief Close an open flink device
 * 
 * @param dev device to close
 * @return int 0 on success, -1 in case of failure
 */
int flink_close(flink_dev* dev) {
	flink_subdev* subdev = NULL;
	int i = 0;

	if(!validate_flink_dev(dev)) {
		flink_error(FLINK_EINVALDEV);
		return EXIT_ERROR;
	}
	
	if(dev->subdevices) {
		free(dev->subdevices);
	}
	
	close(dev->fd);
	free(dev);
	return EXIT_SUCCESS;
}


/**
 * @brief Returns the number of subdevices of a fink device.
 * 
 * @param dev Device to read
 * @return int Number of flink devices or -1 in case of error.
 */
int flink_get_nof_subdevices(flink_dev* dev) {
	
	if(!validate_flink_dev(dev)) {
		flink_error(FLINK_EINVALDEV);
		return EXIT_ERROR;
	}
	
	return dev->nof_subdevices;
}


/**
 * @brief Reset a flink subdevice
 * 
 * @param subdev Subdevice to reset
 * @return int 0 on sucess, -1 else.
 */
int flink_subdevice_reset(flink_subdev* subdev) {
	uint32_t offset = CONFIG_OFFSET;
	uint8_t reset = 1;
	
	if(!validate_flink_subdev(subdev)) {
		flink_error(FLINK_EINVALSUBDEV);
		return EXIT_ERROR;
	}
	
	if(flink_write_bit(subdev, offset, RESET_BIT, &reset)) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}


/**
 * @brief Select a flink subdevice for further operations
 * 
 * @param subdev Subdevice to select.
 * @param exclusive Block access to this subdevice for other processes.
 * @return int 0 on success, else -1.
 */
int flink_subdevice_select(flink_subdev* subdev, uint8_t exclusive) {
	ioctl_cmd_t cmd = SELECT_SUBDEVICE;
	if(exclusive) cmd = SELECT_SUBDEVICE_EXCL;
	
	if(flink_ioctl(subdev->parent, cmd, &(subdev->id)) < 0) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}


flink_subdev* flink_get_subdevice_by_id(flink_dev* dev, uint8_t subdev_id) {
	
	// Check flink device structure
	if(!validate_flink_dev(dev)) {
		flink_error(FLINK_EINVALDEV);
		return 0;
	}
	
	// Check subdevice id
	if(subdev_id > dev->nof_subdevices) {
		flink_error(FLINK_EINVALSUBDEV);
		return 0;
	}
	
	return dev->subdevices + subdev_id;
}


uint8_t flink_subdevice_get_id(flink_subdev* subdev) {
	return subdev->id;
}


uint16_t flink_subdevice_get_type(flink_subdev* subdev) {
	return subdev->type_id;
}


uint8_t flink_subdevice_get_subtype(flink_subdev* subdev) {
	return subdev->sub_type_id;
}


uint8_t flink_subdevice_get_ifversion(flink_subdev* subdev) {
	return subdev->if_version;
}


uint32_t flink_subdevice_get_baseaddr(flink_subdev* subdev) {
	return subdev->base_addr;
}


uint32_t flink_subdevice_get_memsize(flink_subdev* subdev) {
	return subdev->mem_size;
}


uint32_t flink_subdevice_get_nofchannels(flink_subdev* subdev) {
	return subdev->nof_channels;
}
