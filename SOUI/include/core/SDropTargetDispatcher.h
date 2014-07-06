#pragma once
#include "wtl.mini\souicoll.h"
#include "swnd.h"

namespace SOUI{

    class SDropTargetDispatcher : public IDropTarget
    {
    public:
        SDropTargetDispatcher(SWindow * pFrame);
        ~SDropTargetDispatcher(void);

        BOOL RegisterDragDrop(SWND swnd,IDropTarget *pDropTarget);
        BOOL RevokeDragDrop(SWND swnd);


        //////////////////////////////////////////////////////////////////////////
        // IUnknown
        virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */  void __RPC_FAR *__RPC_FAR *ppvObject);

        virtual ULONG STDMETHODCALLTYPE AddRef( void){return 1;}

        virtual ULONG STDMETHODCALLTYPE Release( void) {return 1;}

        //////////////////////////////////////////////////////////////////////////
        // IDropTarget

        virtual HRESULT STDMETHODCALLTYPE DragEnter( 
            /* [unique][in] */ IDataObject *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD *pdwEffect);

        virtual HRESULT STDMETHODCALLTYPE DragOver( 
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD *pdwEffect);

        virtual HRESULT STDMETHODCALLTYPE DragLeave( void);

        virtual HRESULT STDMETHODCALLTYPE Drop( 
            /* [unique][in] */  IDataObject *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */  DWORD *pdwEffect);

    protected:
        POINT PointL2FrameClient(const POINTL & pt);


        typedef SMap<SWND,IDropTarget *> DTMAP;
        DTMAP m_mapDropTarget;
        IDataObject *m_pDataObj;
        SWND         m_hHover;
        SWindow     *m_pOwner;
    };

}