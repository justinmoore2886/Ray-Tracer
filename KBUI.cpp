#include "KBUI.h"
#include <iomanip>

Variable::Variable(const string& name,
                   float *value,
                   float min,
                   float max,
                   float increment,
                   void (*callback)(float)) {
    _name = name;
    _value = value;
    _min = min;
    _max = max;
    _increment = increment;
    _callback = callback;
}

void KBUI::prev_variable() {
    if (cur_variable > 0) {
        cur_variable--;
        cout << "--------------\n";
        Variable var = variables[cur_variable];
        cout << var._name << ": " << *var._value << "\n";
    }
}

void KBUI::next_variable() {
    if (cur_variable < variables.size()-1) {
        cur_variable++;
        cout << "--------------\n";
        Variable& var = variables[cur_variable];
        cout << var._name << ": " << *var._value << "\n";
    }
}

void KBUI::increment() {
    Variable& var = variables[cur_variable];
    *var._value += var._increment;
    if (*var._value > var._max)
        *var._value = var._max;
    else {
        show_variable(var);
        if (var._callback != NULL) {
            (*var._callback)(*var._value);
        }
    }
}

void KBUI::decrement() {
    Variable& var = variables[cur_variable];
    *var._value -= var._increment;
    if (*var._value < var._min)
        *var._value = var._min;
    else {
        show_variable(var);
        if (var._callback != NULL) {
            (*var._callback)(*var._value);
        }
    }
}

void KBUI::show_variable(const Variable& var) {
    std::cout << std::setw(max_name_len)
              << var._name << ": " << *var._value << "\n";
}

KBUI::KBUI() {
    cur_variable = 0;
    max_name_len = 0;
}

bool KBUI::handle_key(int key) {
//        cout << "KBUI::handle_key: key=" << key << "\n";

    if (key == GLFW_KEY_LEFT) {
        prev_variable();
        return true;
    }
    else if (key == GLFW_KEY_RIGHT) {
        next_variable();
        return true;
    }
    else if (key == GLFW_KEY_UP) {
        increment();
        return true;
    }
    else if (key == GLFW_KEY_DOWN) {
        decrement();
        return true;
    }
    else if (key == GLFW_KEY_SPACE) {
        for (int i = 0; i < (int)variables.size(); i++)
            show_variable(variables[i]);
    }

    return false;
}

float KBUI::get_float(const string& name) {
    if (var_from_name.find(name) != var_from_name.end()) {
        Variable& var = *var_from_name[name];
        return *var._value;
    }
    else {
        cerr << "Can't find variable with name \"" << name << "\","
             << " returning 0\n";
        return 0;
    }
}

float KBUI::get_bool(const string& name) {
    if (var_from_name.find(name) != var_from_name.end()) {
        Variable& var = *var_from_name[name];
        return *var._value == 0.0;
    }
    else {
        cerr << "Can't find variable with name \"" << name << "\","
             << " returning 0\n";
        return 0;
    }
}

void KBUI::add_variable(const string& name,
                        float *value,
                        float min,
                        float max,
                        float increment,
                        void (*callback)(float)) {
    variables.push_back(Variable(name, value, min, max,
                                 increment,callback));
    var_from_name[name] = &variables.back();
    if (name.length() > max_name_len)
        max_name_len = name.length();

}

void KBUI::done_init() {
    Variable& var = variables[cur_variable];
    show_variable(var);
}

