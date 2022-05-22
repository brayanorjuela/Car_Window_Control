#ifndef WINDOW_MOTOR_H_
#define WINDOW_MOTOR_H_

#include "systemc.h"
#include <string>

using namespace std;

SC_MODULE(Window_Motor)
{
    // ------------------- Inputs -------------------
    sc_in<Window_and_Sensor_Control::Indication_for_Motor> indication_motor;
 
    // ------------------- Outputs -------------------
    sc_out<TestBench::Motor_Action> window_motor;
    sc_out<bool> motor_error;

    // ------------------- Internal -------------------
    SC_CTOR(Window_Motor) {
        motor_error.initialize(false);
        SC_THREAD(Motor_Controlling);
        sensitive << indication_motor;
    }

    void Motor_Controlling() {
        int time_error_up;
        int time_error_down;
        bool Motorfehlerlampe;
        while (indication_motor.read() == Window_and_Sensor_Control::Off_Indication) {
            cout<<"Stopped";
            wait();
            while (indication_motor.read() == Window_and_Sensor_Control::Move_up_Indication | indication_motor.read() == Window_and_Sensor_Control::Move_down_Indication) {
                if (indication_motor.read() == Window_and_Sensor_Control::Move_up_Indication) {
                    window_motor.write(TestBench::Move_up);
                    cout<<"Moving Motor";
                    time_error_up = 1;
                    while (indication_motor.read() == Window_and_Sensor_Control::Move_up_Indication & time_error_up <= 3000) {

                        wait(1, SC_MS);
                        time_error_up++;
                    }
                    if (time_error_up > 3000) {
                        motor_error.write(true);
                        Motorfehlerlampe = 1;
                        sc_stop();
                    }
                }
                if (indication_motor.read() == Window_and_Sensor_Control::Move_down_Indication) {
                    window_motor.write(TestBench::Move_down);
                    cout<<"Moving Motor";
                    time_error_down = 1;
                    while (indication_motor.read() == Window_and_Sensor_Control::Move_down_Indication & time_error_down <= 3000) {

                        wait(1, SC_MS);
                        time_error_down++;
                    }
                    if (time_error_down > 3000) {
                        motor_error.write(true);
                        Motorfehlerlampe = 1;
                        sc_stop();
                    }
                }
            }
           if (indication_motor.read() == Window_and_Sensor_Control::Off_Indication){
               window_motor.write(TestBench::Off);
           }    
        }
    }

};

#endif
