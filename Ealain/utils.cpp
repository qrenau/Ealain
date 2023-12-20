#include <sstream>
#include <algorithm>

#include "utils.h"

namespace ealain {

    std::string format(unsigned int number) {
        std::ostringstream nbo; nbo << number;
        std::string nb = nbo.str();
        std::string msg;
        std::ostringstream rmsg;

        unsigned int count = 0;
        for(auto it = std::rbegin(nb); it != std::rend(nb); ++it) {
            if(count > 0 and count % 3 == 0) {
                rmsg << " ";
            }
            rmsg << *it;
            count++;
        }
        msg = rmsg.str();
        std::reverse(ALL(msg));
        return msg;
    }

} // ealain

