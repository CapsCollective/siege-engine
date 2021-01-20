#ifndef A_DARK_DISCOMFORT_DEVCONSOLE_H
#define A_DARK_DISCOMFORT_DEVCONSOLE_H

#include "../../systems/entity/Entity2D.h"
#include "../../utils/ServiceLocator.h"

class DevConsole : public Entity2D
{
public:

    // Constructors

    explicit DevConsole(bool isEditorMode) :
    messageDisplay(ServiceLocator::GetMessageDisplay()),
    profiler(ServiceLocator::GetProfiler()),
    isEditorMode(isEditorMode),
    isActive(false)
    {
        Entity::SetName("DevConsole");
    };

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

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
