
#ifndef PHRAKTAL_LOG_H
#define PHRAKTAL_LOG_H

class Log
{
public:
    Log();

    ~Log();

    static inline void log(std::ostream& os, const std::string& msg);
};

void Log::log(std::ostream& os, const std::string& msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

#endif //PHRAKTAL_LOG_H
