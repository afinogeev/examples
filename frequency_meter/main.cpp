#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>

using namespace std;

#define PI 3.14159265
#define SIZE 10000
#define F_SAMPLE 1000
#define ACCURACY 1


void generator(float amplitude, float frequency, float phase, int dc, int noise_amplitude, float* data){

    frequency = frequency/F_SAMPLE;
    if(frequency<0.5){ //kotelnikov
        int noise;
        for(int i=0; i<SIZE; i++){
            noise = rand()%(noise_amplitude*2) - noise_amplitude;
            *data = amplitude*sin(2*PI*i*frequency + phase) + dc + noise;
            data++;
        }
    }
    return;
}

void remove_dc(float* data){

    float dc = 0;

    //calculate direct current
    for(int i=0; i<SIZE; i++){
        dc += (*data)/SIZE;
        data++;
    }

    data -= SIZE;

    //remove dc from data
    for(int i=0; i<SIZE; i++){
        *data -= dc;
        data++;
    }
}

float frequency_meter(float* data){

    vector <int> fronts;

    //finding fronts of signal or negative to positive jump
    float d0, d1;
    for(int i=0; i<SIZE-1; i++){
        d0 = *data;
        d1 = *(data+1);
        if(d0<=0 && d1>0)
            fronts.push_back(i);
        data++;
    }

    float frequency = 0;

    // calculating frequency by difference between two fronts of signal
    int fronts_size = fronts.size();
    if(fronts_size>1){
        for(int i=0; i<fronts_size-1; i++){
            frequency += fronts[i+1]-fronts[i]; //mean
        }
        frequency /= fronts_size-1;
        return F_SAMPLE/frequency;
    }

    return 0;
}

bool test_task(float frequency){
    cout << "==============" << endl << "TEST" << endl;
    cout << "frequency: " << frequency << endl;
    float amplitude = 50 + rand()%100;
    cout << "amplitude: " << amplitude << endl;
    float phase = rand()%90;
    cout << "phase: " << phase << endl;
    float dc = 50 + rand()%50;
    cout << "dc: " << dc << endl;
    float noise_amplitude = 1 + rand()%50;
    cout << "noise_amplitude: " << noise_amplitude << endl;
    cout << endl;

    float generated_signal[SIZE];
    generator(amplitude,frequency, phase, dc, noise_amplitude, generated_signal);
    remove_dc(generated_signal);
    float calculated_frequency = frequency_meter(generated_signal);

    cout << "calculated_frequency: " << calculated_frequency << endl;
    float difference = abs(calculated_frequency-frequency);
    cout << "difference: " << difference << endl;
    cout << "snr: " << amplitude/noise_amplitude << endl;
    cout << endl;

    if(difference<ACCURACY)
        return true;

    return false;
}

void test(){

    srand(time(0));

    for(int i=40; i<=400; i+=40){
        if(test_task(i))
            cout << "TEST DONE" << endl;
        else
            cout << "TEST FAILED" << endl;
    }
    return;
}

int main()
{
    cout << "FREQUENCY METER" << endl;
    test();
    return 0;
}
