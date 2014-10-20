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
 *  flink userspace library, subdevice type "pwm"                  *
 *                                                                 *
 *******************************************************************/
 
#include "flinklib.h"
#include "types.h"
#include "error.h"
#include "log.h"

int flink_pwm_get_baseclock(flink_subdev* subdev, uint32_t* frequency) {
	uint32_t offset;
	
	dbg_print("Reading base clock from PWM subdevice %d\n", subdev->id);
	
	offset = HEADER_SIZE + SUBHEADER_SIZE;
	dbg_print("  --> calculated offset is 0x%x!\n", offset);
	
	if(flink_read(subdev, offset, REGISTER_WITH, frequency) != REGISTER_WITH) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}


int flink_pwm_set_period(flink_subdev* subdev, uint32_t channel, uint32_t period) {
	uint32_t offset;
	
	dbg_print("Setting PWM period for channel %d on subdevice %d\n", subdev->id, channel);
	
	offset = HEADER_SIZE + SUBHEADER_SIZE + PWM_FIRSTPWM_OFFSET + REGISTER_WITH * channel;
	dbg_print("  --> calculated offset is 0x%x!\n", offset);
	
	if(flink_write(subdev, offset, REGISTER_WITH, &period) != REGISTER_WITH) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}


int flink_pwm_set_hightime(flink_subdev* subdev, uint32_t channel, uint32_t hightime) {
	uint32_t offset;
		
	dbg_print("Setting PWM hight time for channel %d on subdevice %d\n", subdev->id, channel);
	
	offset = HEADER_SIZE + SUBHEADER_SIZE + PWM_FIRSTPWM_OFFSET + subdev->nof_channels * REGISTER_WITH + REGISTER_WITH * channel;
	dbg_print("  --> calculated offset is 0x%x!\n", offset);
	
	if(flink_write(subdev, offset, REGISTER_WITH, &hightime) != REGISTER_WITH) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}
