#ifndef VOSYSTEM_H
#define VOSYSTEM_H

#include <memory>

class ImgIO;

class voSystem {
public:
    voSystem();

private:
    std::shared_ptr<ImgIO> imgIO;
};

#endif // VOSYSTEM_H
