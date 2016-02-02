#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

#include "../numeric/value.h"

namespace atoms {

template <class T>
class Pid {
public:
	struct Config {
		T p, i, d;
		T bottom, top;
	};
    
    struct State {
        Value<T, Clamped> integrator;
        T last_input;
        
        State(const T& bottom, const T& top)
            : integrator(0, { bottom, top }), last_input(0) { }
    };
    
    Pid(const Config& params)
        : params(params), state(params.bottom, params.top) {}
    
    Config get_params() const {
        return params;
    }
    
    void set_params(Config& p) {
        params = p;
        reset();
    }
    
    State get_state() const {
        return state;
    }
    
    void reset() {
        state = State(params.bottom, params.top);
    }
    
    T step(T input, T desired_value) {
        T error(desired_value - input);
        state.integrator = state.integrator.get() + params.i * error;
            
        Value<T, Clamped> output(params.p * error + state.integrator
            - params.d * (input - state.last_input),
            { params.bottom, params.top });
            
        state.last_input = input;
        
        return output;            
    }
    
    T step(T input, T desired_value, T time_step) {
        T error(desired_value - input);
        state.integrator += params.i * error * time_step;

        Value<T, Clamped> output(params.p * error * time_step + state.integrator
            - params.d * (input - state.last_input) * time_step,
            { params.bottom, params.top });

        state.last_input = input;

        return output;
    }
private:
    Config params;
    State  state;
};

}