#ifndef SLIME_SIMULATION_ARGUMENTS_H
#define SLIME_SIMULATION_ARGUMENTS_H

#include <string>

template<typename T>
struct Argument {
    std::string name;
    T value;
};

template<typename T, typename... Arguments>
std::string generate_arguments(T arg, Arguments... args) {
    return generate_arguments(arg) + generate_arguments(args...);
}

template<typename T>
std::string generate_arguments(T arg) {
    return "#define " + arg.name + " " + std::to_string(arg.value) + "\n";
}

template<typename U=std::string>
std::string generate_arguments(Argument<std::string> arg) {
    return "#define " + arg.name + " " + arg.value + "\n";
}

template<typename... Arguments>
std::string generate_arguments_with_default_marker(Arguments... arguments) {
    return "#define default_consts\n" + generate_arguments(arguments...);
}

#endif //SLIME_SIMULATION_ARGUMENTS_H
