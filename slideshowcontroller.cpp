#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/awt/Toolkit.hpp>
#include <com/sun/star/frame/XFrame.hpp>
#include <com/sun/star/frame/XFrames.hpp>
#include <com/sun/star/io/IOException.hpp>
#include <com/sun/star/awt/WindowDescriptor.hpp>
#include <com/sun/star/lang/SystemDependent.hpp>
#include <com/sun/star/frame/XFramesSupplier.hpp>
#include <com/sun/star/frame/FrameSearchFlag.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/awt/XSystemChildFactory.hpp>
#include <com/sun/star/presentation/XPresentation2.hpp>
#include <com/sun/star/beans/PropertyVetoException.hpp>
#include <com/sun/star/lang/IllegalArgumentException.hpp>
#include <com/sun/star/presentation/XPresentationSupplier.hpp>

#include "slideshowcontroller.h"

SlideShowController::SlideShowController() noexcept
    : m_xMultiComponentFactory { nullptr }
    , m_xComponentContext{ nullptr }
    , m_xComponent { nullptr }
    , m_xPresentation { nullptr }
    , m_xPresentation2 { nullptr }
{
    try {
        // Get the remote office context. If necessary a new process is started.
        m_xComponentContext = cppu::bootstrap();
        m_xMultiComponentFactory = m_xComponentContext->getServiceManager();
    } catch (cppu::BootstrapException& e) {
        printf("cppu::BootstrapException: %s.\n", e.getMessage().toUtf8().getStr());
    } catch (css::uno::DeploymentException& e) {
        printf("cppu::DeploymentException: %s.\n", e.Message.toUtf8().getStr());
    } catch (css::uno::Exception& e) {
        printf("css::uno::Exception: %s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}

css::uno::Reference<css::awt::XWindowPeer> SlideShowController::createWindowPeer(sal_uInt64 winid) noexcept
{
    auto xToolkit = qurey<css::awt::XToolkit>("com.sun.star.awt.Toolkit");

    css::uno::Reference<css::awt::XWindowPeer> xWindowPeer = nullptr;

    try {
        css::uno::Reference<css::awt::XSystemChildFactory> xSystemChildFactory(xToolkit, css::uno::UNO_QUERY);
        xWindowPeer = xSystemChildFactory->createSystemChild(css::uno::Any(sal_Int64(winid)),
                                                             css::uno::Sequence<sal_Int8>(),
                                                             css::lang::SystemDependent::SYSTEM_XWINDOW);
    } catch (css::uno::Exception& e) {
        printf("css::uno::Exception: %s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }

    return xWindowPeer;
}

void SlideShowController::load(const char *url, sal_uInt64 winid) noexcept
{
    try {
        auto xWindowPeer = createWindowPeer(winid);
        if (!xWindowPeer.is()) {
            printf("Could not instantiate XWindowPeer.\n");
            return;
        }

        css::uno::Reference<css::awt::XWindow> xWindow(xWindowPeer, css::uno::UNO_QUERY);
        if (!xWindow.is()) {
            printf("Could not instantiate XWindow.\n");
            return;
        }

        auto xFrame = qurey<css::frame::XFrame>("com.sun.star.frame.Frame");
        if (!xFrame.is()) {
            printf("Could not instantiate XFrame.\n");
            return;
        }
        xFrame->initialize(xWindow);

        auto xFramesSupplier = qurey<css::frame::XFramesSupplier>("com.sun.star.frame.Desktop");
        if (!xFramesSupplier.is()) {
            printf("Could not instantiate XFramsSupplier.\n");
            return;
        }

        auto xFrames = xFramesSupplier->getFrames();
        xFrames->append(xFrame);

        rtl::OUString oldName = xFrame->getName();
        xFrame->setName("qt_odk_slideplayer");

        auto xComponentLoader = qurey<css::frame::XComponentLoader>("com.sun.star.frame.Desktop");
        if (!xComponentLoader.is()) {
            printf("Could not instantiate XComponentLoader.\n");
            return;
        }

        css::uno::Sequence<css::beans::PropertyValue> loadProps(0);
        m_xComponent = xComponentLoader->loadComponentFromURL(rtl::OUString::createFromAscii(url),
                                                              "qt_odk_slideplayer",
                                                              css::frame::FrameSearchFlag::CHILDREN,
                                                              loadProps);
        if (!m_xComponent.is()) {
            printf("Could not load Component from URL.\n");
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
    } catch (css::io::IOException& e) {
        printf("css::io::IOException: %s.\n", e.Message.toUtf8().getStr());
    } catch (css::lang::IllegalArgumentException& e) {
        printf("css::lang::IllegalArgumentException: %s.\n", e.Message.toUtf8().getStr());
    } catch (css::beans::PropertyVetoException& e) {
        printf("css::beans::PropertyVetoException: %s.\n", e.Message.toUtf8().getStr());
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}

void SlideShowController::start() noexcept
{
    try {
        if (m_xPresentation2.is())
            m_xPresentation2->getController()->resume();
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}

void SlideShowController::pause() noexcept
{
    try {
        if (m_xPresentation2.is())
            m_xPresentation2->getController()->pause();
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}

void SlideShowController::gotoNextEffect() noexcept
{
    try {
        if (m_xPresentation2.is())
            m_xPresentation2->getController()->gotoNextEffect();
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}

void SlideShowController::gotoPreviousEffect() noexcept
{
    try {
        if (m_xPresentation2.is())
            m_xPresentation2->getController()->gotoPreviousEffect();
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}

void SlideShowController::gotoNextSlide() noexcept
{
    try {
        if (m_xPresentation2.is())
            m_xPresentation2->getController()->gotoNextSlide();
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}

void SlideShowController::gotoPreviousSlide() noexcept
{
    try {
        if (m_xPresentation2.is())
            m_xPresentation2->getController()->gotoPreviousSlide();
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}

void SlideShowController::gotoFirstSlide() noexcept
{
    try {
        if (m_xPresentation2.is())
            m_xPresentation2->getController()->gotoFirstSlide();
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}

void SlideShowController::gotoLastSlide() noexcept
{
    try {
        if (m_xPresentation2.is())
            m_xPresentation2->getController()->gotoLastSlide();
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}

int SlideShowController::getSlideCount() noexcept
{
    int count = 0;

    try {
        if (m_xPresentation2.is())
            count = m_xPresentation2->getController()->getSlideCount();
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }

    return count;
}

void SlideShowController::stopSound() noexcept
{
    try {
        if (m_xPresentation2.is())
            m_xPresentation2->getController()->stopSound();
    } catch (css::uno::Exception& e) {
        printf("%s.\n", e.Message.toUtf8().getStr());
    } catch (...) {
        printf("Unknown exception.\n");
    }
}
