#include "Application.hpp"
#include "Config.hpp"

bool Application::init(const Config& /*cfg*/) { return true; }
void Application::run() {}
void Application::shutdown() {}
void Application::processEvents() {}
void Application::fixedUpdate(float /*dt*/) {}
void Application::variableUpdate(float /*dt*/) {}
void Application::render(float /*alpha*/) {}
