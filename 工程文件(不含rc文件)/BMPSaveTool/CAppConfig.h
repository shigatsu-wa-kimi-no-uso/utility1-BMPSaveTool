#pragma once
#include "framework.h"

#define FILENAME_LENGTH 40
#define PATH_LENGTH (MAX_PATH - FILENAME_LENGTH)


//实现程序配置,读取/写入配置的类
class CAppConfig
{
private:
	WORD m_cfg_nKey[2];
	HKEY m_cfg_hKey;
	TCHAR m_cfg_tszPath[PATH_LENGTH];

protected:		//仅允许子类访问,限制创建对象,对象创建时,目录和按键为默认的
	CAppConfig()
	{
		memset(m_cfg_tszPath, 0, sizeof(m_cfg_tszPath));
		m_cfg_hKey = NULL;
		LPITEMIDLIST dirID;
		if (SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &dirID) == S_OK)  //获得桌面路径
		{
			if (SHGetPathFromIDList(dirID, m_cfg_tszPath))
			{
				wcscat_s(m_cfg_tszPath, PATH_LENGTH, _T("\\screenshots\\"));	//初始化存储路径和热键
				m_cfg_nKey[0] = VK_NUMPAD9;
				m_cfg_nKey[1] = HOTKEYF_CONTROL;
				return;
			}
		}
		MessageBox(NULL, TEXT("程序初始化失败, 该程序可能有BUG或不适合此系统的运行环境, 您可以通过本程序的\"关于\"窗口获取作者的联系方式, 反馈问题"), TEXT("错误"), MB_OK | MB_ICONERROR);
	}

public:
	BOOL __fastcall GetSavePathCfg(const PTCHAR _Out_ strBuf)
	{
		if (_stprintf_s(strBuf, PATH_LENGTH, _T("%s"), m_cfg_tszPath) > 0)
			return TRUE;
		return FALSE;
	}

	BOOL __fastcall GetHotKeyCfg(PWORD _Out_ nKey)
	{
		nKey[0] = m_cfg_nKey[0];
		nKey[1] = m_cfg_nKey[1];
		return TRUE;
	}

	BOOL __fastcall SetHotKeyCfg(const PWORD _In_ nNewKey)
	{
		m_cfg_nKey[0] = nNewKey[0];
		m_cfg_nKey[1] = nNewKey[1];
		return TRUE;
	}

	BOOL __fastcall SetSavePathCfg(const PTCHAR _In_ strNewPath)
	{
		if (_stprintf_s(m_cfg_tszPath, PATH_LENGTH, _T("%s"), strNewPath) > 0)
			return TRUE;
		return FALSE;
	}

	//读取配置信息
	BOOL ReadConfig()		//从注册表中读取配置信息到成员变量,如果没有则会把默认配置信息保存到注册表
	{
		DWORD size_keyPath = sizeof(m_cfg_tszPath);
		DWORD size_keyKeys = sizeof(m_cfg_nKey);
		TCHAR szPathSize[9];
		TCHAR szKeySize[9];
		if (RegCreateKey(HKEY_CURRENT_USER, _T("Software\\ClipboardBMPSaveTool"), &m_cfg_hKey) == ERROR_SUCCESS)
		{
			if (RegGetValue(m_cfg_hKey, NULL, _T("Path"), RRF_RT_REG_BINARY, NULL, m_cfg_tszPath, &size_keyPath) == ERROR_SUCCESS &&	//获取失败不会改变原值(原值已在构造函数里初始化)
				RegGetValue(m_cfg_hKey, NULL, _T("Keys"), RRF_RT_DWORD, NULL, m_cfg_nKey, &size_keyKeys) == ERROR_SUCCESS)  //返回size值,对于WCHAR需要x2
			{
					RegCloseKey(m_cfg_hKey);
					return TRUE;
				
			}
			else
			{
				if (RegSetKeyValue(m_cfg_hKey, NULL, _T("Path"), REG_BINARY, m_cfg_tszPath, sizeof(m_cfg_tszPath)) == ERROR_SUCCESS &&
					RegSetKeyValue(m_cfg_hKey, NULL, _T("Keys"), REG_DWORD, m_cfg_nKey, sizeof(m_cfg_nKey)) == ERROR_SUCCESS)
				{
					RegCloseKey(m_cfg_hKey);
					return TRUE;
				}
			}

			_stprintf_s(szPathSize, 9, _T("%d"), size_keyPath);
			MessageBox(NULL, szPathSize, TEXT("GH"), MB_OK);
		}
		return FALSE;
	}

	//保存配置信息到注册表(保存成员变量的值)
	BOOL SaveConfig()
	{
		if (RegCreateKey(HKEY_CURRENT_USER, _T("Software\\ClipboardBMPSaveTool"), &m_cfg_hKey) == ERROR_SUCCESS)	//正常情况下都为成功
		{
			if (RegSetKeyValue(m_cfg_hKey, NULL, _T("Path"), REG_BINARY, m_cfg_tszPath, sizeof(m_cfg_tszPath)) == ERROR_SUCCESS &&	//没有键会创建键
				RegSetKeyValue(m_cfg_hKey, NULL, _T("Keys"), REG_DWORD, m_cfg_nKey, sizeof(m_cfg_nKey)) == ERROR_SUCCESS)
			{
				RegCloseKey(m_cfg_hKey);
				return TRUE;
			}
		}
		return FALSE;
	}

};

