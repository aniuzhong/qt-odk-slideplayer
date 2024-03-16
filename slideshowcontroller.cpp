#include <com/sun/star/awt/Toolkit.hpp>
#include <com/sun/star/frame/XFrame.hpp>
#include <com/sun/star/frame/XFrames.hpp>
#include <com/sun/star/awt/WindowDescriptor.hpp>
#include <com/sun/star/frame/XFramesSupplier.hpp>
#include <com/sun/star/frame/FrameSearchFlag.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/awt/XSystemChildFactory.hpp>
#include <com/sun/star/presentation/XPresentation2.hpp>
#include <com/sun/star/presentation/XPresentationSupplier.hpp>

#include "officeconnection.h"
#include "slideshowcontroller.h"

SlideShowController::SlideShowController()
{
}

void SlideShowController::load(const char *url, unsigned long long winid)
{
    try {
        auto& connect = OfficeConnection::instance();

        auto xToolkit = connect.qurey<css::awt::XToolkit>("com.sun.star.awt.Toolkit");
        css::uno::Reference<css::awt::XSystemChildFactory> xSystemChildFactory(xToolkit, css::uno::UNO_QUERY);
        css::uno::Reference<css::awt::XWindowPeer> xWindowPeer = xSystemChildFactory->createSystemChild(css::uno::Any(sal_Int64(winid)),
                                                                                    css::uno::Sequence<sal_Int8>(),
                                                                                    6);
        if (!xWindowPeer.is()) {
            printf("Could not instantiate XWindowPeer\n");
            return;
        }

        css::uno::Reference<css::awt::XWindow> xWindow(xWindowPeer, css::uno::UNO_QUERY);
        if (!xWindow.is()) {
            printf("Could not instantiate XWindow\n");
            return;
        }

        auto xFrame = connect.qurey<css::frame::XFrame>("com.sun.star.frame.Frame");
        if (!xFrame.is()) {
            printf("Could not instantiate XFrame\n");
            return;
        }
        xFrame->initialize(xWindow);

        auto xFramesSupplier = connect.qurey<css::frame::XFramesSupplier>("com.sun.star.frame.Desktop");
        if (!xFramesSupplier.is()) {
            printf("Could not instantiate XFramsSupplier\n");
            return;
        }

        auto xFrames = xFramesSupplier->getFrames();
        xFrames->append(xFrame);

        rtl::OUString oldName = xFrame->getName();
        xFrame->setName("qt_odk_slideplayer");

        auto xComponentLoader = connect.qurey<css::frame::XComponentLoader>("com.sun.star.frame.Desktop");
        if (!xComponentLoader.is()) {
            printf("Could not instantiate XComponentLoader\n");
            return;
        }

        css::uno::Sequence<css::beans::PropertyValue> loadProps(0);
        m_xComponent = xComponentLoader->loadComponentFromURL(rtl::OUString::createFromAscii(url),
                                                              "qt_odk_slideplayer",
                                                              css::frame::FrameSearchFlag::CHILDREN,
                                                              loadProps);
        if (!m_xComponent.is()) {
            printf("Could not load Component from URL\n");
            return;
        }

        xFrame->setName(oldName);
        xWindow->setVisible(true);

        css::uno::Reference<css::presentation::XPresentationSupplier> xPresentationSupplier(m_xComponent, css::uno::UNO_QUERY);
        m_xPresentation = xPresentationSupplier->getPresentation();

        css::uno::Reference<css::beans::XPropertySet> props(m_xPresentation, css::uno::UNO_QUERY);
        props->setPropertyValue("AllowAnimations", css::uno::Any(true));
        props->setPropertyValue("IsAlwaysOnTop", css::uno::Any(false));
        props->setPropertyValue("IsAutomatic", css::uno::Any(false));
        props->setPropertyValue("IsEndless", css::uno::Any(false));
        props->setPropertyValue("IsFullScreen", css::uno::Any(false));
        props->setPropertyValue("IsMouseVisible", css::uno::Any(false));
        props->setPropertyValue("StartWithNavigator", css::uno::Any(false));
        props->setPropertyValue("UsePen", css::uno::Any(false));

        m_xPresentation2 = css::uno::Reference<css::presentation::XPresentation2>(m_xPresentation, css::uno::UNO_QUERY);
        m_xPresentation2->start();

        pause();

    } catch (css::uno::Exception &e) {
        printf("%s\n", e.Message.toUtf8().getStr());
        return;
    }
}

void SlideShowController::start()
{
    m_xPresentation2->getController()->resume();
}

void SlideShowController::pause()
{
    m_xPresentation2->getController()->pause();
}

void SlideShowController::gotoNextEffect()
{
    m_xPresentation2->getController()->gotoNextEffect();
}

void SlideShowController::gotoPreviousEffect()
{
    m_xPresentation2->getController()->gotoPreviousEffect();
}

void SlideShowController::gotoNextSlide()
{
    m_xPresentation2->getController()->gotoNextSlide();
}

void SlideShowController::gotoPreviousSlide()
{
    m_xPresentation2->getController()->gotoPreviousSlide();
}

void SlideShowController::gotoFirstSlide()
{
    m_xPresentation2->getController()->gotoFirstSlide();
}

void SlideShowController::gotoLastSlide()
{
    m_xPresentation2->getController()->gotoLastSlide();
}

int SlideShowController::getSlideCount()
{
    return m_xPresentation2->getController()->getSlideCount();
}

void SlideShowController::stopSound()
{
    return m_xPresentation2->getController()->stopSound();
}