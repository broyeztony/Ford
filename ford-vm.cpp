//
//  ford-vm.cpp
//  Ford-VM
//
//  Created by Tony on 07/03/2023.
//

#include "src/vm/Logger.h"
#include "src/vm/FordVM.h"
#include <iostream>

int main (int argc, char const *argv[]) {

    FordVM vm;
    auto result = vm.exec(R"(

        (< 10 (+ 5 5))

    )");

    std::cout << "result: " << result << std::endl;
    std::cout << "All done!\n";

    return 0;
}
