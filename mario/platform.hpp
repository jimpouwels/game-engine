#ifndef platform_hpp
#define platform_hpp

#include "animatedGraphic.hpp"

class Platform : public jimp::AnimatedGraphic {
  
private:
    
public:
    Platform();
    void doOnInit() override;
    void doOnUpdate(float elapsedTime) override;
};


#endif
