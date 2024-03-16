#ifndef SLIDWSHOWCONTROLLER_H
#define SLIDWSHOWCONTROLLER_H

#include <com/sun/star/lang/XComponent.hpp>
#include <com/sun/star/presentation/XPresentation2.hpp>

class SlideShowController
{
public:
    SlideShowController();

public:
    void load(const char* url, unsigned long long winid);
    void start();
    void pause();
    void gotoNextEffect();
    void gotoPreviousEffect();
    void gotoNextSlide();
    void gotoPreviousSlide();
    void gotoFirstSlide();
    void gotoLastSlide();
    int  getSlideCount();
    void stopSound();

private:
    css::uno::Reference<css::lang::XComponent>             m_xComponent;
    css::uno::Reference<css::presentation::XPresentation>  m_xPresentation;
    css::uno::Reference<css::presentation::XPresentation2> m_xPresentation2;
};

#endif // SLIDWSHOWCONTROLLER_H
