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
 *  flink userspace library, public header file                    *
 *                                                                 *
 *******************************************************************/

#ifndef FLINKLIB_H_
#define FLINKLIB_H_

#include <stdint.h>
#include <sys/types.h>

#include "flink_funcid.h"		// "flink_funcid.h" is created by: flinkinterface/func_id/create_flink_funcid.sh

#ifdef __cplusplus
extern "C" {
#endif

// ############ Device handles ############

typedef struct _flink_dev    flink_dev;
typedef struct _flink_subdev flink_subdev;


// ############ Base operations ############

flink_dev* flink_open(const char* file_name);
int        flink_close(flink_dev* dev);


// ############ Low level operations ############

int     flink_ioctl(flink_dev* dev, int cmd, void* arg);
ssize_t flink_read(flink_subdev* subdev, uint32_t offset, uint8_t size, void* rdata);
ssize_t flink_write(flink_subdev* subdev, uint32_t offset, uint8_t size, void* wdata);
int     flink_read_bit(flink_subdev* subdev, uint32_t offset, uint8_t bit, void* rdata);
int     flink_write_bit(flink_subdev* subdev, uint32_t offset, uint8_t bit, void* wdata);


// ############ Subdevice operations ############

#define REGISTER_WITH						4	// byte
#define HEADER_SIZE						16	// byte
#define SUBHEADER_SIZE						16	// byte
#define STATUS_OFFSET						0x0010	// byte
#define CONFIG_OFFSET						0x0014	// byte
#define INFO_DESC_SIZE						28		// byte
#define PWM_BASECLK_OFFSET					0x0000	// byte
#define PWM_FIRSTPWM_OFFSET					0x0004	// byte
#define PPWA_BASECLK_OFFSET					0x0000	// byte
#define PPWA_FIRSTPPWA_OFFSET					0x0004	// byte
#define ANALOG_INPUT_FIRST_VALUE_OFFSET				0x0004	// byte
#define REFLECTIVE_SENSOR_FIRST_VALUE_OFFSET				0x0004	// byte
#define ANALOG_OUTPUT_FIRST_VALUE_OFFSET			0x0004	// byte
#define WD_FIRST_COUNTER_OFFSET					0x0004	// byte
#define STEPPER_MOTOR_FIRST_CONF_OFFSET    0x0004 // byte
#define RESET_BIT						0
#define GLOBAL_STEP_RESET               1


#define NONEXCL_ACCESS						0
#define EXCL_ACCESS						1

// General
int           flink_get_nof_subdevices(flink_dev* dev);
flink_subdev* flink_get_subdevice_by_id(flink_dev* dev, uint8_t subdev_id);
flink_subdev* flink_get_subdevice_by_unique_id(flink_dev* dev, uint32_t unique_id);

uint8_t       flink_subdevice_get_id(flink_subdev* subdev);
uint16_t      flink_subdevice_get_function(flink_subdev* subdev);
uint8_t       flink_subdevice_get_subfunction(flink_subdev* subdev);
uint8_t       flink_subdevice_get_function_version(flink_subdev* subdev);
uint32_t      flink_subdevice_get_baseaddr(flink_subdev* subdev);
uint32_t      flink_subdevice_get_memsize(flink_subdev* subdev);
uint32_t      flink_subdevice_get_nofchannels(flink_subdev* subdev);
uint32_t      flink_subdevice_get_unique_id(flink_subdev* subdev);
int           flink_subdevice_select(flink_subdev* subdev, uint8_t exclusive);
int           flink_subdevice_reset(flink_subdev* subdev);
const char*   flink_subdevice_id2str(uint8_t subdev_id);

// Info
int flink_info_get_description(flink_subdev* subdev, char* value);

// Analog input
int flink_analog_in_get_resolution(flink_subdev* subdev, uint32_t* resolution);
int flink_analog_in_get_value(flink_subdev* subdev, uint32_t channel, uint32_t* value);

// Analog output
int flink_analog_out_get_resolution(flink_subdev* subdev, uint32_t* resolution);
int flink_analog_out_set_value(flink_subdev* subdev, uint32_t channel, uint32_t value);

// Digital in-/output
#define FLINK_OUTPUT 1
#define FLINK_INPUT 0
int flink_dio_get_baseclock(flink_subdev* subdev, uint32_t* frequency);
int flink_dio_set_direction(flink_subdev* subdev, uint32_t channel, uint8_t output);
int flink_dio_set_value(flink_subdev* subdev, uint32_t channel, uint8_t high);
int flink_dio_get_value(flink_subdev* subdev, uint32_t channel, uint8_t* value);
int flink_dio_set_debounce(flink_subdev* subdev, uint32_t channel, uint32_t debounce);
int flink_dio_get_debounce(flink_subdev* subdev, uint32_t channel, uint32_t* debounce);

// Counter
int flink_counter_set_mode(flink_subdev* subdev, uint8_t mode);
int flink_counter_get_count(flink_subdev* subdev, uint32_t channel, uint32_t* data);

// PWM
int flink_pwm_get_baseclock(flink_subdev* subdev, uint32_t* frequency);
int flink_pwm_set_period(flink_subdev* subdev, uint32_t channel, uint32_t period);
int flink_pwm_get_period(flink_subdev* subdev, uint32_t channel, uint32_t* period);
int flink_pwm_set_hightime(flink_subdev* subdev, uint32_t channel, uint32_t hightime);
int flink_pwm_get_hightime(flink_subdev* subdev, uint32_t channel, uint32_t* hightime);

// PPWA
int flink_ppwa_get_baseclock(flink_subdev* subdev, uint32_t* frequency);
int flink_ppwa_get_period(flink_subdev* subdev, uint32_t channel, uint32_t* period);
int flink_ppwa_get_hightime(flink_subdev* subdev, uint32_t channel, uint32_t* hightime);

// Watchdog
int flink_wd_get_baseclock(flink_subdev* subdev, uint32_t* base_clk);
int flink_wd_get_status(flink_subdev* subdev, uint8_t* status);
int flink_wd_set_counter(flink_subdev* subdev, uint32_t value);
int flink_wd_arm(flink_subdev* subdev);

// Stepper Motor
int flink_stepperMotor_get_baseclock(flink_subdev* subdev, uint32_t* frequency);
int flink_stepperMotor_set_local_config_reg(flink_subdev* subdev, uint32_t channel, uint32_t config);
int flink_stepperMotor_get_local_config_reg(flink_subdev* subdev, uint32_t channel, uint32_t* config);
int flink_stepperMotor_set_local_config_reg_bits_atomic(flink_subdev* subdev, uint32_t channel, uint32_t config_set);
int flink_stepperMotor_reset_local_config_reg_bits_atomic(flink_subdev* subdev, uint32_t channel, uint32_t config_reset);
int flink_stepperMotor_set_prescaler_start(flink_subdev* subdev, uint32_t channel, uint32_t prescaler);
int flink_stepperMotor_get_prescaler_start(flink_subdev* subdev, uint32_t channel, uint32_t* prescaler);
int flink_stepperMotor_set_prescaler_top(flink_subdev* subdev, uint32_t channel, uint32_t prescaler);
int flink_stepperMotor_get_prescaler_top(flink_subdev* subdev, uint32_t channel, uint32_t* prescaler);
int flink_stepperMotor_set_acceleration(flink_subdev* subdev, uint32_t channel, uint32_t acceleration);
int flink_stepperMotor_get_acceleration(flink_subdev* subdev, uint32_t channel, uint32_t* acceleration);
int flink_stepperMotor_set_steps_to_do(flink_subdev* subdev, uint32_t channel, uint32_t steps);
int flink_stepperMotor_get_steps_to_do(flink_subdev* subdev, uint32_t channel, uint32_t* steps);
int flink_stepperMotor_get_steps_have_done(flink_subdev* subdev, uint32_t channel, uint32_t* steps);
int flink_steppermotor_global_step_reset(flink_subdev* subdev);

// Reflective sensor
int flink_reflectivesensor_get_resolution(flink_subdev* subdev, uint32_t* resolution);
int flink_reflectivesensor_get_value(flink_subdev* subdev, uint32_t channel, uint32_t* value);
int flink_reflectivesensor_set_upper_hysterese(flink_subdev* subdev, uint32_t channel, uint32_t value);
int flink_reflectivesensor_get_upper_hysterese(flink_subdev* subdev, uint32_t channel, uint32_t* value);
int flink_reflectivesensor_set_lower_hysterese(flink_subdev* subdev, uint32_t channel, uint32_t value);
int flink_reflectivesensor_get_lower_hysterese(flink_subdev* subdev, uint32_t channel, uint32_t* value);

// Interrupt
int flink_register_irq(flink_dev *dev, uint32_t irq_number);
int flink_unregister_irq(flink_dev *dev, uint32_t irq_number);
int flink_get_signal_offset(flink_dev *dev, uint32_t *offset);
int flink_set_irq_multiplex(flink_subdev *subdev, uint32_t irq, uint32_t flink_irq);
int flink_get_irq_multiplex(flink_subdev *subdev, uint32_t irq, uint32_t *flink_irq);

// ############ Exit states ############
#define EXIT_SUCCESS	0
#define EXIT_ERROR		-1

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // FLINKLIB_H_
