#include "ServiceLocator.h"

// Define static members
raylib::Camera* ServiceLocator::camera = nullptr;
EditorController* ServiceLocator::editorController = nullptr;
MessageDisplay* ServiceLocator::messageDisplay = nullptr;
Profiler* ServiceLocator::systemProfiler = nullptr;