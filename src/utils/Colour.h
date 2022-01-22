#ifndef A_DARK_DISCOMFORT_COLOUR_H
#define A_DARK_DISCOMFORT_COLOUR_H

struct Colour
{
    // Public constants

    static const Colour Black;

    static const Colour White;

    static const Colour Red;

    static const Colour Green;

    static const Colour Blue;

    static const Colour Pink;

    // 'Structors

    Colour() : Colour(0, 0, 0, 0) {}

    Colour(int r, int g, int b) : r(r), g(g), b(b), a(255) {}

    Colour(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

    // Public members

    int r;
    int g;
    int b;
    int a;
};

#endif // A_DARK_DISCOMFORT_COLOUR_H
