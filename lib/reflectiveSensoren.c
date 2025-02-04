/*******************************************************************************************
 *    ____   _____ _______            _________  _____     _____  ____  _____  ___  ____   *
 *   / __ \ / ____|__   __|          |_   ___  ||_   _|   |_   _||_   \|_   _||_  ||_  _|  *
 *  | |  | | (___    | |    _______    | |_  \_|  | |       | |    |   \ | |    | |_/ /    *
 *  | |  | |\___ \   | |   |_______|   |  _|      | |   _   | |    | |\ \| |    |  __'.    *
 *  | |__| |____) |  | |              _| |_      _| |__/ | _| |_  _| |_\   |_  _| |  \ \_  *
 *   \____/|_____/   |_|             |_____|    |________||_____||_____|\____||____||____| *
 *                                                                                         *
 *******************************************************************************************
 *                                                                                         *
 *            flink userspace library, subdevice function "Refelx-Sensoren"                *
 *                                                                                         *
 *******************************************************************************************/
 
/** @file reflectiveSensoren.c
 *  @brief flink userspace library, subdevice function "Refelx-Sensoren".
 *
 *  Contains the high-level functions for a flink subdevice
 *  which realizes the function "Refelx-Sensoren".
 * 
 *  is based of ain.c
 *
 *  @author Patrick Good
 */

#include "flinklib.h"
#include "types.h"
#include "error.h"
#include "log.h"

#include <stdint.h>

/**
 * @brief Sets the resolution of a Reflective-Sensor subdevice
 * @param subdev: Subdevice.
 * @param resolution: Contains the resolution in number of resolvable steps.
 * @return int: 0 on success, -1 in case of failure.
 */
int flink_reflectivesensor_get_resolution(flink_subdev* subdev, uint32_t* resolution){
	uint32_t offset;
	offset = HEADER_SIZE + SUBHEADER_SIZE;
	
	if(flink_read(subdev, offset, REGISTER_WITH, resolution) != REGISTER_WITH) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}

/**
 * @brief Reads an Reflective-Sensor input channel
 * @param subdev: Subdevice containing the channel.
 * @param channel: Channel number.
 * @param value: Contains the digitized value of the channel input.
 * @return int: 0 on success, -1 in case of failure.
 */
int flink_reflectivesensor_get_value(flink_subdev* subdev, uint32_t channel, uint32_t* value){
	uint32_t offset;
	

	dbg_print("Get value of refelctive sensor for channel %d on subdevice %d\n", subdev->id, channel);
	offset = HEADER_SIZE + SUBHEADER_SIZE + REFLECTIVE_SENSOR_FIRST_VALUE_OFFSET + channel*REGISTER_WITH;
	dbg_print("  --> calculated offset is 0x%x!\n", offset);

	if(flink_read(subdev, offset, REGISTER_WITH, value) != REGISTER_WITH) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}

/**
 * @brief This sets the upper limit of the hysteresis for a reflective sensor.
 * @param subdev: Subdevice containing the channel.
 * @param channel: Channel number.
 * @param value: Countains the value for the upper bound.
 * @return int: 0 on success, -1 in case of failure.
 */
int flink_reflectivesensor_set_upper_hysterese(flink_subdev* subdev, uint32_t channel, uint32_t value){
	uint32_t offset;

	dbg_print("Set value of refelctiv sensor for channel %d on subdevice %d\n", subdev->id, channel);
	offset = HEADER_SIZE + SUBHEADER_SIZE + REFLECTIVE_SENSOR_FIRST_VALUE_OFFSET + REGISTER_WITH*subdev->nof_channels;
	offset += channel*REGISTER_WITH;
	dbg_print("  --> calculated offset is 0x%x!\n", offset);

	if(flink_write(subdev, offset, REGISTER_WITH, &value) != REGISTER_WITH) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}

/**
 * @brief This gets the upper limit of the hysteresis for a reflective sensor.
 * @param subdev: Subdevice containing the channel.
 * @param channel: Channel number.
 * @param value: Countains the value for the upper bound.
 * @return int: 0 on success, -1 in case of failure.
 */
int flink_reflectivesensor_get_upper_hysterese(flink_subdev* subdev, uint32_t channel, uint32_t* value){
	uint32_t offset;

	dbg_print("Get value of refelctiv sensor for channel %d on subdevice %d\n", subdev->id, channel);
	offset = HEADER_SIZE + SUBHEADER_SIZE + REFLECTIVE_SENSOR_FIRST_VALUE_OFFSET + REGISTER_WITH*subdev->nof_channels;
	offset += channel*REGISTER_WITH;
	dbg_print("  --> calculated offset is 0x%x!\n", offset);

	if(flink_read(subdev, offset, REGISTER_WITH, value) != REGISTER_WITH) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}

/**
 * @brief This sets the under limit of the hysteresis for a reflective sensor.
 * @param subdev: Subdevice containing the channel.
 * @param channel: Channel number.
 * @param value: Countains the value for the upper bound.
 * @return int: 0 on success, -1 in case of failure.
 */
int flink_reflectivesensor_set_lower_hysterese(flink_subdev* subdev, uint32_t channel, uint32_t value){
	uint32_t offset;

	dbg_print("Set value of refelctiv sensor for channel %d on subdevice %d\n", subdev->id, channel);
	offset = HEADER_SIZE + SUBHEADER_SIZE + REFLECTIVE_SENSOR_FIRST_VALUE_OFFSET + 2*REGISTER_WITH*subdev->nof_channels;
	offset += channel*REGISTER_WITH;
	dbg_print("  --> calculated offset is 0x%x!\n", offset);

	if(flink_write(subdev, offset, REGISTER_WITH, &value) != REGISTER_WITH) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}

/**
 * @brief This gets the under limit of the hysteresis for a reflective sensor.
 * @param subdev: Subdevice containing the channel.
 * @param channel: Channel number.
 * @param value: Countains the value for the upper bound.
 * @return int: 0 on success, -1 in case of failure.
 */
int flink_reflectivesensor_get_lower_hysterese(flink_subdev* subdev, uint32_t channel, uint32_t* value){
	uint32_t offset;

	dbg_print("Get value of refelctive sensor for channel %d on subdevice %d\n", subdev->id, channel);
	offset = HEADER_SIZE + SUBHEADER_SIZE + REFLECTIVE_SENSOR_FIRST_VALUE_OFFSET + 2*REGISTER_WITH*subdev->nof_channels;
	offset += channel*REGISTER_WITH;
	dbg_print("  --> calculated offset is 0x%x!\n", offset);

	if(flink_read(subdev, offset, REGISTER_WITH, value) != REGISTER_WITH) {
		libc_error();
		return EXIT_ERROR;
	}
	return EXIT_SUCCESS;
}