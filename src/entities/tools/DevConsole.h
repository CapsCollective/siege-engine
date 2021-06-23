#ifndef A_DARK_DISCOMFORT_DEVCONSOLE_H
#define A_DARK_DISCOMFORT_DEVCONSOLE_H

#include "../../systems/entity/Tool.h"
#include "../../utils/ServiceLocator.h"

class DevConsole : public Tool
{
public:

    // 'Structors

    explicit DevConsole(bool isEditorMode) :
    isEditorMode(isEditorMode),
    isActive(false),
    messageDisplay(ServiceLocator::GetMessageDisplay()),
    profiler(ServiceLocator::GetProfiler())
    {
        Entity::SetName("DevConsole");
    };

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw2D() override;

private:

    // Private methods

    bool CheckEditorMode();

    bool CheckArgs(const std::string& command, const std::string& args);

    // Private fields

    bool isEditorMode;

    bool isActive;

    std::string inputText;

    std::string lastInput;

    MessageDisplay* messageDisplay;

    Profiler* profiler;
};

#endif //A_DARK_DISCOMFORT_DEVCONSOLE_H
