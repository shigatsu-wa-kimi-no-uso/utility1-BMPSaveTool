#pragma once
#include "CAppConfig.h"
#define SUCCESS             0
#define CREATEDIR_FAILED    1
#define GETDIR_FAILED       2
#define OPENCB_FAILED       3
#define NOBMPDATA           4
#define GETNAME_FAILED      5
#define OPENFILE_FAILED     6
#define GETDATA_FAILED      7
#define WRITEFILE_FAILED    8

#define SZRESULTLENGTH      50


//实现从内存中提取BMP到文件的类

class CBMPExtractor : public CAppConfig
{
private:
    TCHAR m_tszPath[PATH_LENGTH];
    TCHAR m_tszFileName[FILENAME_LENGTH];
    TCHAR m_tszFinalPath[MAX_PATH];

    LPBITMAPINFOHEADER m_lpDIB;
    HANDLE m_hFile;

    SIZE_T m_sizeofData;

    DWORD WriteBMPFile()
    {
        /*简单记录下bmp文件组成:
        1.BITMAPFILEHEADER
        2.BITMAPINFOHEADER
        (有些需要调色板）
        3.DATA
        内存中用CF_BITMAP出错，CF_DIB可以,原因未知
        */

        SIZE_T writtenbCnt[3];
        BOOL flagChk[3];
        BOOL has12byteOffset = 0;
        const SIZE_T sizeofImage = m_lpDIB->biSizeImage;
        BITMAPFILEHEADER fileH;
        LPBYTE  lpData = (LPBYTE)m_lpDIB;

        lpData += sizeof(BITMAPINFOHEADER);     //指针指向位图数据区,定义为LPBYTE便于后期移动指针
        m_lpDIB->biCompression = 0;         //搞不懂为什么原始的压缩属性这里是3,一直都是这个数值导致文件打不开，改成0就好了
        fileH.bfType = 0x4d42;
        fileH.bfReserved1 = 0;
        fileH.bfReserved2 = 0;
        fileH.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);      //偏移
        fileH.bfSize = sizeofImage + fileH.bfOffBits;
        if (m_sizeofData - m_lpDIB->biSize - sizeofImage == 12)     //12字节偏移简单判断（PrtScr键截屏,在BMP信息后会有12字节的数据，如果不去掉，会导致图片错位）
            has12byteOffset = 1;

        flagChk[0] = WriteFile(m_hFile, &fileH,
            sizeof(BITMAPFILEHEADER),
            &writtenbCnt[0],
            NULL);
        //   SetFilePointer(m_hFile, 0, NULL, FILE_END);        //经测试写入文件后文件指针自动后移，不用手动操作

        flagChk[1] = WriteFile(m_hFile, m_lpDIB,
            sizeof(BITMAPINFOHEADER),
            &writtenbCnt[1],
            NULL);
        // SetFilePointer(m_hFile, 0, NULL, FILE_END);

        if (has12byteOffset)
        {
            lpData += 12;
        }

        flagChk[2] = WriteFile(m_hFile, lpData,
            sizeofImage,
            &writtenbCnt[2],
            NULL);

        if (flagChk[0] && flagChk[1] && flagChk[2])
            return SUCCESS;
        else
            return WRITEFILE_FAILED;
    }


    DWORD GetFileName()          //获得文件名同时获得路径，创建文件夹
    {
        if (GetSavePathCfg(m_tszPath))          //获取路径
        {
            TCHAR substr_Dir[PATH_LENGTH] = { 0 };
            //多级目录创建
            if (m_tszPath[_tcslen(m_tszPath) - 1] != '\\')
            {
                _tcscat_s(m_tszPath, PATH_LENGTH, _T("\\"));
            }

            for (register int i = 0; i < _tcslen(m_tszPath); ++i)
            {
                if (m_tszPath[i] == '\\')
                {
                    _tcsncpy_s(substr_Dir, PATH_LENGTH, m_tszPath, i + 1);
                    if (
                        !(PathIsDirectory(m_tszPath) || 
                        CreateDirectory(m_tszPath, NULL))
                        )                                       //检查目录是否存在 特别注意：如果目录已经存在，CreateDirectory返回值为0
                    {
                        return CREATEDIR_FAILED; 
                    }
                }
            }
          
            SYSTEMTIME time;
            GetLocalTime(&time);
            _stprintf_s(m_tszFileName, FILENAME_LENGTH, _T("%d-%d-%d_%d-%d-%02d.bmp"),
                time.wYear,
                time.wMonth,
                time.wDay,
                time.wHour,
                time.wMinute,
                time.wSecond);
            _tcscpy_s(m_tszFinalPath, MAX_PATH, m_tszPath);        //最终目录复制
            _tcscat_s(m_tszFinalPath, MAX_PATH, m_tszFileName);    //连接文件名
            return SUCCESS;
        }
        return GETDIR_FAILED;
    }



public:
    CBMPExtractor()
    {
        m_hFile = nullptr;
        m_lpDIB = nullptr;
        m_sizeofData = 0;
        memset(m_tszPath, 0, sizeof(m_tszPath));
        memset(m_tszFileName, 0, sizeof(m_tszFileName));
        memset(m_tszFinalPath, 0, sizeof(m_tszFinalPath));
    }

    ~CBMPExtractor()
    {
        CloseHandle(m_hFile);   
        CloseClipboard();
    }


    DWORD CreateBMPFile()
    {
        DWORD state = -1;
        if (!OpenClipboard(NULL))
            return OPENCB_FAILED;
        if (IsClipboardFormatAvailable(CF_DIB))        //打开剪切板操作会导致其他程序无法写入
        {
            state = GetFileName();
            if (state == SUCCESS)
            {
                HGLOBAL hBMPData = GetClipboardData(CF_DIB);            //获得剪切板数据
                m_lpDIB = (LPBITMAPINFOHEADER)GlobalLock(hBMPData);     //锁定剪切板内存 
                m_hFile = CreateFile(m_tszFinalPath, GENERIC_WRITE,        //创建文件
                    0,
                    NULL,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
                if (m_hFile)               //文件句柄和内存锁定状态
                {
                    if (m_lpDIB)
                    {
                        m_sizeofData = GlobalSize(hBMPData);
                        state = WriteBMPFile();
                        CloseHandle(m_hFile);
                        GlobalUnlock(m_lpDIB);
                    }
                    else
                        state = GETDATA_FAILED;
                }
                else
                    state = OPENFILE_FAILED;
            }
        }
        else
            state = NOBMPDATA;
        CloseClipboard();
        return state;
    }

};


