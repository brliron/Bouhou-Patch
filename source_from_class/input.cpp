DECLARE_INTERFACE_(IDirect3DSurface9, IDirect3DResource9)
{
    /*** IUnknown methods ***/
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

    /*** IDirect3DResource9 methods ***/
    STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9** ppDevice) PURE;
    STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) PURE;
    STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid,void* pData,DWORD* pSizeOfData) PURE;
    STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
    STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD PriorityNew) PURE;
    STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
    STDMETHOD_(void, PreLoad)(THIS) PURE;
    STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
    STDMETHOD(GetContainer)(THIS_ REFIID riid,void** ppContainer) PURE;
    STDMETHOD(GetDesc)(THIS_ D3DSURFACE_DESC *pDesc) PURE;
    STDMETHOD(LockRect)(THIS_ D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) PURE;
    STDMETHOD(UnlockRect)(THIS) PURE;
    STDMETHOD(GetDC)(THIS_ HDC *phdc) PURE;
    STDMETHOD(ReleaseDC)(THIS_ HDC hdc) PURE;
    
    #ifdef D3D_DEBUG_INFO
    LPCWSTR Name;
    UINT Width;
    UINT Height;
    DWORD Usage;
    D3DFORMAT Format;
    D3DPOOL Pool;
    D3DMULTISAMPLE_TYPE MultiSampleType;
    DWORD MultiSampleQuality;
    DWORD Priority;
    UINT LockCount;
    UINT DCCount;
    LPCWSTR CreationCallStack;
    #endif
};
