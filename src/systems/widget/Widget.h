#ifndef A_DARK_DISCOMFORT_WIDGET_H
#define A_DARK_DISCOMFORT_WIDGET_H

class Widget
{
public:

    /**
     * A virtual update method to be overridden for widget
     * drawing instructions
     */
    virtual void OnDraw() {};

    /**
     * Queues the widget to be freed from storage safely at
     * the end of a frame
     */
    void QueueFree();

    /**
     * Unsafely removes the widget from storage immediately
     * @warning This method is very unsafe and QueueFree()
     *          should be used wherever possible instead
     */
    void Free();

protected:

    // Constructors

    /**
     * Zero-param constructor for Widget, initialises
     * z-index to zero
     */
    Widget() :
    Widget(0)
    {};

    /**
     * Delegate constructor for Widget
     * @param zIndex
     */
    explicit Widget(int zIndex) :
    zIndex(zIndex)
    {};

    /**
     * The widget z-index for render order
     */
    int zIndex;
};


#endif //A_DARK_DISCOMFORT_WIDGET_H
