#ifndef SLIDWSHOWCONTROLLER_H
#define SLIDWSHOWCONTROLLER_H

#include <com/sun/star/lang/XComponent.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/presentation/XPresentation2.hpp>

class SlideShowController final
{
public:
    SlideShowController() noexcept;
    SlideShowController(const SlideShowController&) = delete;
    SlideShowController& operator=(const SlideShowController &) = delete;
    SlideShowController(SlideShowController&&) = default;
    SlideShowController & operator=(SlideShowController&&) = default;

public:
    void load(const char* url, sal_Int64 winid)    noexcept;
    void start()                                   noexcept;
    void pause()                                   noexcept;
    void gotoNextEffect()                          noexcept;
    void gotoPreviousEffect()                      noexcept;
    void gotoNextSlide()                           noexcept;
    void gotoPreviousSlide()                       noexcept;
    void gotoFirstSlide()                          noexcept;
    void gotoLastSlide()                           noexcept;
    int  getSlideCount()                           noexcept;
    void stopSound()                               noexcept;

private:
    template <typename T>
    css::uno::Reference<T> qurey(rtl::OUString spec) noexcept
    {
        try {
            css::uno::Reference<T> inst(m_xMultiComponentFactory->createInstanceWithContext(spec, m_xComponentContext),
                                        css::uno::UNO_QUERY);
            return inst;
        } catch (...) {
            printf("Could not create service of type.\n");
            return nullptr;
        }
    }

private:
    css::uno::Reference<css::lang::XMultiComponentFactory> m_xMultiComponentFactory;
    css::uno::Reference<css::uno::XComponentContext>       m_xComponentContext;
    css::uno::Reference<css::lang::XComponent>             m_xComponent;
    css::uno::Reference<css::presentation::XPresentation>  m_xPresentation;
    css::uno::Reference<css::presentation::XPresentation2> m_xPresentation2;
};

#endif // SLIDWSHOWCONTROLLER_H
