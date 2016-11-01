#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

class Optimizater
{
public:
    enum OPT_TYPE {
        OPT_DEFAULT,
        OPT_G2O,
        OPT_CERES
    };

    Optimizater(OPT_TYPE optType = OPT_DEFAULT);

private:

};

#endif // OPTIMIZATION_H
