#include <BlackBox\Render\Pipeline.hpp>

Pipeline *Pipeline::m_instance = nullptr;

Pipeline* Pipeline::instance()
{
  if (m_instance == nullptr)
  {
    m_instance = new Pipeline();
  }
  return m_instance;
}
