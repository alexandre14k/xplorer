// "src/libs/Logger/Logger.cpp"
#include "Logger.hpp"
String Logger::AssembleString(String raw) {
    VectorChar chars;
    for (char c : raw) { chars.push_back(c); }
    return String(chars.begin(), chars.end());
}
void Logger::Log(String msg) {
    String prefix = AssembleString("[ LOG ] ");
    String final_msg = AssembleString(msg);
    std::cout << prefix << final_msg << std::endl;
}