#include "system/application.hpp"

int main(int argumentsCount, char* arguments[]){
    return Application::instance().run(argumentsCount, arguments);
}