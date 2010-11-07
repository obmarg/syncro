// dllmain.h : Declaration of module class.

class CExplorerExtensionModule : public ATL::CAtlDllModuleT< CExplorerExtensionModule >
{
public :
	DECLARE_LIBID(LIBID_ExplorerExtensionLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_EXPLOREREXTENSION, "{F5B64E6F-5E91-4EFF-BB10-DE9CA8B64B7D}")
};

extern class CExplorerExtensionModule _AtlModule;
