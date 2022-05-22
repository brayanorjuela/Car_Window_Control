#ifndef WINDOW_CONTROL_AND_SENSOR_H_
#define WINDOW_CONTROL_AND_SENSOR_H_

#include "systemc.h"
#include <string>

using namespace std;

SC_MODULE(Window_and_Sensor_Control)
{
    // ------------------- Inputs -------------------
    sc_in<bool> btn_up_pressed;
    sc_in<bool> btn_up_deep_pressed;

    sc_in<bool> btn_down_pressed;
    sc_in<bool> btn_down_deep_pressed;

    sc_in<bool> car_unlocked;
    sc_in <TestBench::Window_State> window_sensor;

    // ------------------- Outputs -------------------
    enum Indication_for_Motor
    {
        Off_Indication = 0,
        Move_up_Indication = 1,
        Move_down_Indication = 2
    };
    sc_out<Indication_for_Motor> indication_motor;

    // ------------------- Internal -------------------
    SC_CTOR(Window_and_Sensor_Control) {
        indication_motor.initialize(Off_Indication);
        SC_THREAD(Window_and_Sensor_Controlling);
        sensitive << car_unlocked << btn_up_pressed << btn_up_deep_pressed << btn_down_pressed << btn_down_deep_pressed << window_sensor;
    }

    void Window_and_Sensor_Controlling() {
        int allow_to_auto_up;
        int allow_to_auto_down;
        int back_to_manual_up;
        int back_to_manual_down;
        int back_to_none_up = 0;
        int manual_to_auto_up = 0;
        int back_to_none_down = 0;
        int manual_to_auto_down = 0;
        int time_auto_to_manual_up;
        int time_auto_to_manual_down;
    
        while (true) {
            wait();
            if (car_unlocked.read() == false & window_sensor.read() != TestBench::Blocked & window_sensor.read() != TestBench::Closed) indication_motor.write(Move_up_Indication);
            if (car_unlocked.read() == false & window_sensor.read() == TestBench::Blocked) { 
                indication_motor.write(Move_down_Indication); 
                wait(); //frag deinen Partner. Es ist wichtig, dass es nur ein Signal bekommt.  
                if (window_sensor.read() == TestBench::Open) indication_motor.write(Off_Indication);
            }
            if (car_unlocked.read() == false & window_sensor.read() == TestBench::Closed) indication_motor.write(Off_Indication);
            if (window_sensor.read() != TestBench::Intermediate & car_unlocked.read() == true) {
                if (window_sensor.read() == TestBench::Blocked) indication_motor.write(Move_down_Indication);
                if (window_sensor.read() == TestBench::Closed) indication_motor.write(Off_Indication);
                if (window_sensor.read() == TestBench::Open) indication_motor.write(Off_Indication);
            }
            if (car_unlocked.read() == true & btn_up_pressed.read() == true & btn_down_pressed.read() == false & btn_down_deep_pressed.read() == false & btn_up_deep_pressed.read() == false & window_sensor.read() != TestBench::Closed & window_sensor.read() != TestBench::Blocked) {
                indication_motor.write(Move_up_Indication);
                cout << "Manual Mode";
                while (car_unlocked.read() == true & back_to_none_up == 0 & manual_to_auto_up == 0) {
                    wait();
                    if (car_unlocked.read() == false) indication_motor.write(Move_up_Indication);
                    if (window_sensor.read() != TestBench::Intermediate & car_unlocked.read() == true) {
                        if (window_sensor.read() == TestBench::Blocked) indication_motor.write(Move_down_Indication);
                        if (window_sensor.read() == TestBench::Closed) indication_motor.write(Off_Indication);
                        if (window_sensor.read() == TestBench::Open) indication_motor.write(Off_Indication);
                    }
                    if (btn_up_pressed.read() == false & car_unlocked.read() == true & btn_down_pressed.read() == false & btn_down_deep_pressed.read() == false & btn_up_deep_pressed.read() == false & window_sensor.read() != TestBench::Blocked & window_sensor.read() != TestBench::Closed) {
                        cout << "None";
                        indication_motor.write(Off_Indication);
                        back_to_none_up = 1;
                        allow_to_auto_up = 0;
                            

                    }
                    if (btn_up_deep_pressed.read() == true & car_unlocked.read() == true & btn_down_pressed.read() == false & btn_down_deep_pressed.read() == false & window_sensor.read() != TestBench::Blocked & window_sensor.read() != TestBench::Closed) {
                        allow_to_auto_up = 1; 
                        back_to_manual_up = 0;
                        manual_to_auto_up = 1;
                    }
                }
                back_to_none_up = 0;
                manual_to_auto_up = 0;
            }

            if (car_unlocked.read() == true & allow_to_auto_up == 1 & btn_up_deep_pressed.read() == true & btn_down_deep_pressed.read() == false) {
                cout << "Auto Mode";
                while (car_unlocked.read() == true & allow_to_auto_up == 1 & back_to_manual_up == 0) {
                    wait();
                    if (car_unlocked.read() == false) indication_motor.write(Move_up_Indication);
                    if (window_sensor.read() != TestBench::Intermediate & car_unlocked.read() == true) {
                        if (window_sensor.read() == TestBench::Blocked) indication_motor.write(Move_down_Indication);
                        if (window_sensor.read() == TestBench::Closed) indication_motor.write(Off_Indication);
                        if (window_sensor.read() == TestBench::Open) indication_motor.write(Off_Indication);
                    }
                    if (btn_up_pressed.read() == true & car_unlocked.read() == true & btn_up_deep_pressed.read() == false & btn_down_pressed.read() == false & btn_down_deep_pressed.read() == false & window_sensor.read() != TestBench::Closed & window_sensor.read() != TestBench::Blocked) {
                        time_auto_to_manual_up = 1;
                        while (btn_up_pressed.read() == true & car_unlocked.read() == true & time_auto_to_manual_up <= 500 & window_sensor.read() != TestBench::Closed & window_sensor.read() != TestBench::Blocked) {
                            
                            wait(1, SC_MS);
                            if (car_unlocked.read() == false) indication_motor.write(Move_up_Indication);
                            if (window_sensor.read() != TestBench::Intermediate & car_unlocked.read() == true) {
                                if (window_sensor.read() == TestBench::Blocked) indication_motor.write(Move_down_Indication);
                                if (window_sensor.read() == TestBench::Closed) indication_motor.write(Off_Indication);
                                if (window_sensor.read() == TestBench::Open) indication_motor.write(Off_Indication);
                            }
                                time_auto_to_manual_up++;
                        }
                        if (time_auto_to_manual_up <= 500 & car_unlocked.read() == true & window_sensor.read() != TestBench::Closed & window_sensor.read() != TestBench::Blocked) {
                            cout << "Automatische Bewegung des Fensters";
                            allow_to_auto_up = 0;           

                        }
                        if (time_auto_to_manual_up > 500 & car_unlocked.read() == true & window_sensor.read() != TestBench::Closed & window_sensor.read() != TestBench::Blocked) {
                            cout<<"Manual Mode";
                            if (car_unlocked.read() == true & btn_up_pressed.read() == true & btn_down_pressed.read() == false & btn_down_deep_pressed.read() == false & btn_up_deep_pressed.read() == false & window_sensor.read() != TestBench::Closed & window_sensor.read() != TestBench::Blocked) {
                                indication_motor.write(Move_up_Indication);
                                while (car_unlocked.read() == true & back_to_none_up == 0 & manual_to_auto_up == 0) {
                                    wait();
                                    if (car_unlocked.read() == false) indication_motor.write(Move_up_Indication);
                                    if (window_sensor.read() != TestBench::Intermediate & car_unlocked.read() == true) {
                                        if (window_sensor.read() == TestBench::Blocked) indication_motor.write(Move_down_Indication);
                                        if (window_sensor.read() == TestBench::Closed) indication_motor.write(Off_Indication);
                                        if (window_sensor.read() == TestBench::Open) indication_motor.write(Off_Indication);
                                    }
                                    if (btn_up_pressed.read() == false & car_unlocked.read() == true & btn_down_pressed.read() == false & btn_down_deep_pressed.read() == false & btn_up_deep_pressed.read() == false & window_sensor.read() != TestBench::Blocked & window_sensor.read() != TestBench::Closed) {
                                        cout << "None";
                                        indication_motor.write(Off_Indication);
                                        back_to_none_up = 1;
                                        allow_to_auto_up = 0;

                                    }
                                    if (btn_up_deep_pressed.read() == true & car_unlocked.read() == true & btn_down_pressed.read() == false & btn_down_deep_pressed.read() == false & window_sensor.read() != TestBench::Blocked & window_sensor.read() != TestBench::Closed) {
                                        allow_to_auto_up = 1; 
                                        back_to_manual_up = 0;
                                        manual_to_auto_up = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            if (car_unlocked.read() == true & btn_down_pressed.read() == true & btn_up_pressed.read() == false & btn_up_deep_pressed.read() == false & btn_down_deep_pressed.read() == false & window_sensor.read() != TestBench::Open & window_sensor.read() != TestBench::Blocked) {
                indication_motor.write(Move_down_Indication);
                cout<<"Manual Mode";
                while (car_unlocked.read() == true & back_to_none_down == 0 & manual_to_auto_down == 0) {
                    wait();
                    if (car_unlocked.read() == false) indication_motor.write(Move_up_Indication);
                    if (window_sensor.read() != TestBench::Intermediate & car_unlocked.read() == true) {
                        if (window_sensor.read() == TestBench::Blocked) indication_motor.write(Move_down_Indication);
                        if (window_sensor.read() == TestBench::Closed) indication_motor.write(Off_Indication);
                        if (window_sensor.read() == TestBench::Open) indication_motor.write(Off_Indication);
                    }
                    if (btn_down_pressed.read() == false & car_unlocked.read() == true & btn_up_pressed.read() == false & btn_up_deep_pressed.read() == false & btn_down_deep_pressed.read() == false & window_sensor.read() != TestBench::Open & window_sensor.read() != TestBench::Blocked) {
                        cout<<"None";
                        indication_motor.write(Off_Indication);
                        back_to_none_down = 1;
                        allow_to_auto_down = 0;
                    }
                    if (btn_down_deep_pressed.read() == true & car_unlocked.read() == true & btn_up_pressed.read() == false & btn_up_deep_pressed.read() == false & window_sensor.read() != TestBench::Open & window_sensor.read() != TestBench::Blocked) {
                        allow_to_auto_down = 1;
                        back_to_manual_down = 0;
                        manual_to_auto_down = 1;
                    }
                }
                back_to_none_down = 0;
                manual_to_auto_down = 0;
            }
           
            if (car_unlocked.read() == true & allow_to_auto_down == 1 & btn_down_deep_pressed.read() == true & btn_up_deep_pressed.read() == false) {
                cout<<"Auto Mode";
                while (car_unlocked.read() == true & allow_to_auto_down == 1 & back_to_manual_down == 0) {
                    wait();
                    if (car_unlocked.read() == false) indication_motor.write(Move_up_Indication);
                    if (window_sensor.read() != TestBench::Intermediate & car_unlocked.read() == true) {
                        if (window_sensor.read() == TestBench::Blocked) indication_motor.write(Move_down_Indication);
                        if (window_sensor.read() == TestBench::Closed) indication_motor.write(Off_Indication);
                        if (window_sensor.read() == TestBench::Open) indication_motor.write(Off_Indication);
                    }
                    if (btn_down_pressed.read() == true & car_unlocked.read() == true & btn_down_deep_pressed.read() == false & btn_up_pressed.read() == false & btn_up_deep_pressed.read() == false & window_sensor.read() != TestBench::Open & window_sensor.read() != TestBench::Blocked) {
                        time_auto_to_manual_down = 1;
                        while (btn_down_pressed.read() == true & car_unlocked.read() == true & time_auto_to_manual_down <= 500 & window_sensor.read() != TestBench::Open & window_sensor.read() != TestBench::Blocked) {

                            wait(1, SC_MS);
                            if (car_unlocked.read() == false) indication_motor.write(Move_up_Indication);
                            if (window_sensor.read() != TestBench::Intermediate & car_unlocked.read() == true) {
                                if (window_sensor.read() == TestBench::Blocked) indication_motor.write(Move_down_Indication);
                                if (window_sensor.read() == TestBench::Closed) indication_motor.write(Off_Indication);
                                if (window_sensor.read() == TestBench::Open) indication_motor.write(Off_Indication);
                            }
                            time_auto_to_manual_down++;
                        }
                        if (time_auto_to_manual_down <= 500 & car_unlocked.read() == true & window_sensor.read() != TestBench::Open & window_sensor.read() != TestBench::Blocked) {
                            cout<<"Automatische Bewegung des Fensters";
                            allow_to_auto_down = 0;
            
                        }
                        if (time_auto_to_manual_down > 500 & car_unlocked.read() == true & window_sensor.read() != TestBench::Open & window_sensor.read() != TestBench::Blocked) {
                            cout << "Manual Mode";
                            if (car_unlocked.read() == true & btn_down_pressed.read() == true & btn_up_pressed.read() == false & btn_up_deep_pressed.read() == false & btn_down_deep_pressed.read() == false & window_sensor.read() != TestBench::Open & window_sensor.read() != TestBench::Blocked) {
                                indication_motor.write(Move_down_Indication);
                                while (car_unlocked.read() == true & back_to_none_down == 0 & manual_to_auto_down == 0) {
                                    wait();
                                    if (car_unlocked.read() == false) indication_motor.write(Move_up_Indication);
                                    if (window_sensor.read() != TestBench::Intermediate & car_unlocked.read() == true) {
                                        if (window_sensor.read() == TestBench::Blocked) indication_motor.write(Move_down_Indication);
                                        if (window_sensor.read() == TestBench::Closed) indication_motor.write(Off_Indication);
                                        if (window_sensor.read() == TestBench::Open) indication_motor.write(Off_Indication);
                                    }
                                    if (btn_down_pressed.read() == false & car_unlocked.read() == true & btn_up_pressed.read() == false & btn_up_deep_pressed.read() == false & btn_down_deep_pressed.read() == false & window_sensor.read() != TestBench::Open & window_sensor.read() != TestBench::Blocked) {
                                        cout<<"None";
                                        indication_motor.write(Off_Indication);
                                        back_to_none_down = 1;
                                        allow_to_auto_down = 0;
                                    }
                                    if (btn_down_deep_pressed.read() == true & car_unlocked.read() == true & btn_up_pressed.read() == false & btn_up_deep_pressed.read() == false & window_sensor.read() != TestBench::Open & window_sensor.read() != TestBench::Blocked) {
                                        allow_to_auto_down = 1;
                                        back_to_manual_down = 0;
                                        manual_to_auto_down = 1;
                                    }
                                }
                            } 
                        }
                    }

                }
            }

        }


    }
};

#endif

