#include "vasException.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <execinfo.h>

#ifdef __cplusplus
}
#endif

bool          VAS::g_isTraceEnabled = false;

VAS::vasException::vasException(const char *pFileName, int lineNum, const char *pClassName, const char *pFuncName, const std::string &description) noexcept
    : m_pClassName(pClassName),
      m_pFuncName(pFuncName),
      m_pFileName(pFileName),
      m_lineNum(lineNum),
      m_description(description)
{
    m_description.append("\n");
    m_description.append(std::string("[FILE] ") + m_pFileName + " [LINE] " + std::to_string(m_lineNum) + "\n");
    m_description.append(std::string("[CLASS] ") + m_pClassName + "\n");
    m_description.append(std::string("[FUNCTION] ") + m_pFuncName + "\n");

    if (g_isTraceEnabled) {
        ::backtrace(m_traceArray, VAS_TRACE_MAX_SIZE);
    }
}

VAS::vasException::~vasException() noexcept
{
    
}

const char *VAS::vasException::what() const noexcept
{
    if (g_isTraceEnabled && m_traceArray) {
        char **pStrList = ::backtrace_symbols(m_traceArray, VAS_TRACE_MAX_SIZE);
        for (int i = 0; i < VAS_TRACE_MAX_SIZE; ++i) {
            std::string tempStr(pStrList[i]);
            if (!tempStr.empty()) {
                m_description.append(tempStr);
                m_description.append("\n");
            }
        }
    }
    return m_description.c_str();
}