//
// Created by Jonathan Moallem on 29/12/20.
//

#include "ServiceLocator.h"

// Define static members
raylib::Camera* ServiceLocator::camera = nullptr;
EditorController* ServiceLocator::editorController = nullptr;
MessageDisplay* ServiceLocator::messageDisplay = nullptr;