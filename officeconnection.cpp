#include <cppuhelper/bootstrap.hxx>

#include "officeconnection.h"

OfficeConnection::OfficeConnection()
{
    // Get the remote office context.
    // If necessary a new process is started.
    m_xComponentContext = cppu::bootstrap();
    m_xMultiComponentFactory = m_xComponentContext->getServiceManager();
}
