#include "FE_Animation.h"

FE_Animation::FE_Animation()
{
    //ctor
}

FE_Animation::~FE_Animation()
{
    //dtor
}

bool FE_Animation::init(){
    return true;
}
bool FE_Animation::destroy(){
    for(auto x: transforms){
        delete x;
    }
    transforms.clear();
    return true;
}
