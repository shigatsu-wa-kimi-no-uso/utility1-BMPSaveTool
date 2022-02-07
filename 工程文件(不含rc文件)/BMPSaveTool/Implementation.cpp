#include "pch.h"
#include "CBmpExtractor.h"


CBMPExtractor g_myObj;

DWORD WINAPI Implementation(TCHAR* _Out_ result)
{
	switch (g_myObj.CreateBMPFile())
	{
	case SUCCESS:
	case NOBMPDATA:
		_stprintf_s(result, SZRESULTLENGTH, _T("NOERROR"));
		break;
	case CREATEDIR_FAILED:
		_stprintf_s(result, SZRESULTLENGTH, _T("创建目录失败"));
		break;
	case GETDIR_FAILED:
		_stprintf_s(result, SZRESULTLENGTH, _T("获取保存目录失败"));
		break;
	case OPENCB_FAILED:
		_stprintf_s(result, SZRESULTLENGTH, _T("打开剪切板失败"));
		break;
	case GETNAME_FAILED:
		_stprintf_s(result, SZRESULTLENGTH, _T("获取文件名失败"));
		break;
	case OPENFILE_FAILED:
		_stprintf_s(result, SZRESULTLENGTH, _T("创建文件失败"));
		break;
	case GETDATA_FAILED:
		_stprintf_s(result, SZRESULTLENGTH, _T("获取剪切板位图数据失败"));
		break;
	case WRITEFILE_FAILED:
		_stprintf_s(result, SZRESULTLENGTH, _T("写入文件失败"));
		break;
	default:
		_stprintf_s(result, SZRESULTLENGTH, _T("程序出现未定义的错误, 您可以通过此程序的\"关于\"窗口获得作者的联系方式, 反馈问题"));
	}

	return 0;
}

