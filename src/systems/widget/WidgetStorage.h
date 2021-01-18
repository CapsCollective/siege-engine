#ifndef A_DARK_DISCOMFORT_WIDGETSTORAGE_H
#define A_DARK_DISCOMFORT_WIDGETSTORAGE_H

#include <vector>

class WidgetStorage
{
public:

    // TODO implement widget storage
    static void Register(class Widget* widget);

    static void Remove(Widget* widget);

    static const std::vector<Widget*>& GetWidgets()
    {
        return widgets;
    }

private:

    static std::vector<Widget*> widgets;
};


#endif //A_DARK_DISCOMFORT_WIDGETSTORAGE_H
