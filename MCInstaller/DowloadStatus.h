#pragma once
#include <windows.h>
#include <iostream>

class DowloadStatus : public IBindStatusCallback
{
    bool isPrinted = false;
    const int with = 140;
public:
    bool printBar;
    HRESULT __stdcall QueryInterface(const IID&, void**)
    {
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef(void) 
    {
        return 1;
    }

    ULONG STDMETHODCALLTYPE Release(void)
    {
        return 1;
    }

    HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, __RPC__in_opt IBinding* pib)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE GetPriority( __RPC__out LONG* pnPriority)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, __RPC__in_opt LPCWSTR szStatusText)
    {
        if (!printBar) return S_OK;
        int progressPerc = int((float(ulProgress) / ulProgressMax) * 100);
        if (ulProgressMax and (!isPrinted or progressPerc != 100) and
                (
                ulStatusCode == BINDSTATUS_DOWNLOADINGDATA 
                or ulStatusCode == BINDSTATUS_BEGINDOWNLOADDATA 
                or ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA
                )
           )
        {
            isPrinted = true;
            int realProgress = progressPerc * with / 100;
            std::cout << '\r';
            if (progressPerc < 100) std::cout << ' ';
            if (progressPerc < 10) std::cout << ' ';
            std::cout << progressPerc << "% [";
            for (int i = 0; i < with; ++i)
            {
                if (i < realProgress and i + 1 == realProgress and i+1 != with) std::cout << '>';
                else std::cout << ((i < realProgress) ? '=' : '-');
            }
                
            std::cout << ']';
        }
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, __RPC__in_opt LPCWSTR szError)
    {
        if (isPrinted)
        {
            std::cout << '\r';
            for (int i = 0; i < with+7; ++i) std::cout << ' ';
            std::cout << '\r';
        }
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD* grfBINDF, BINDINFO* pbindinfo)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pformatetc, STGMEDIUM* pstgmed)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE OnObjectAvailable(__RPC__in REFIID riid, __RPC__in_opt IUnknown* punk)
    {
        return E_NOTIMPL;
    }
};

