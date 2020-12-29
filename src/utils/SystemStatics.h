//
// Created by Jonathan Moallem on 29/12/20.
//

#ifndef A_DARK_DISCOMFORT_SYSTEMSTATICS_H
#define A_DARK_DISCOMFORT_SYSTEMSTATICS_H


class SystemStatics
{
public:

    // Editor Mode functions

    static bool IsEditorMode()
    {
        return isEditorMode;
    }

    static void SetIsEditorMode(bool isEditor)
    {
        isEditorMode = isEditor;
    }

private:

    // Private fields

    static bool isEditorMode;
};


#endif //A_DARK_DISCOMFORT_SYSTEMSTATICS_H
