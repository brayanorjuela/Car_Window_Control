#include "systemc.h"
#include "TestBench.h"
#include "Window_and_Sensor_Control.h"
#include "Window_Motor.h"
#include <string>

int sc_main(int argc, char *argv[])
{
	// Channel definitions:
	sc_signal<bool> sig_up_pressed;
	sc_signal<bool> sig_up_deep_pressed;
	sc_signal<bool> sig_down_pressed;
	sc_signal<bool> sig_down_deep_pressed;

	sc_signal<bool> sig_unlocked;
	sc_signal<bool> sig_motor_error;
	
	sc_signal<Window_and_Sensor_Control::Indication_for_Motor> sig_indication_motor;
	sc_signal<TestBench::Motor_Action> sig_window_motor;
	sc_signal<TestBench::Window_State> sig_window_sensor;

	// Connect testbench ports to channels:
	TestBench tb("TestBench", argv[1]);
	tb.btn_up_deep_pressed(sig_up_deep_pressed);
	tb.btn_up_pressed(sig_up_pressed);
	tb.car_unlocked(sig_unlocked);
	tb.window_motor(sig_window_motor);
	tb.btn_down_deep_pressed(sig_down_deep_pressed);
	tb.btn_down_pressed(sig_down_pressed);
	tb.window_sensor(sig_window_sensor);
	tb.motor_error(sig_motor_error);
	
	// TODO connect your module(s) to channels

	// Connect window_and_sensor_control ports to channels:
	Window_and_Sensor_Control wsc("Window_and_Sensor_Control");
	wsc.btn_up_deep_pressed(sig_up_deep_pressed);
	wsc.btn_up_pressed(sig_up_pressed);
	wsc.car_unlocked(sig_unlocked);
	wsc.btn_down_deep_pressed(sig_down_deep_pressed);
	wsc.btn_down_pressed(sig_down_pressed);
	wsc.indication_motor(sig_indication_motor);
	wsc.window_sensor(sig_window_sensor);

	// Connect window_motor ports to channels:
	Window_Motor wm("Window_Motor");
	wm.indication_motor(sig_indication_motor);
	wm.motor_error(sig_motor_error);
	wm.window_motor(sig_window_motor);

	// Run simulation:
	sc_start(100, SC_SEC);
	return 0;
}
