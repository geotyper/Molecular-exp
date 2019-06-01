#pragma once

#include <string>


namespace logp {
    void open(int bot_id);
    void log(const std::string& message);
}

