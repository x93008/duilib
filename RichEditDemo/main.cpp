// RichEditDemo.cpp : RichEdit 多行文本编辑控件演示程序
// 使用 WindowImplBase 基类

#include "DuiLib/UIlib.h"

using namespace DuiLib;


class CRichEditDemoWnd : public WindowImplBase
{
public:
    CRichEditDemoWnd() {};
    ~CRichEditDemoWnd() {};

public:
    // 必须实现的纯虚函数
    virtual LPCTSTR GetWindowClassName(void) const
    {
        return _T("RichEditDemoWindow");
    }

    virtual CDuiString GetSkinFile()
    {
        return _T("RichEditDemo.xml");
    }

    virtual CDuiString GetSkinFolder()
    {
        return _T("assets\\");  // 需要包含反斜杠
    }

    // 窗口初始化
    virtual void InitWindow()
    {
        // 初始化一些默认值
        CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richedit_main")));
        if (pRichEdit) {
            pRichEdit->SetText(_T("这是一个多行文本编辑器示例。\r\n\r\n支持换行、复制、粘贴等功能。"));
        }
    }

    // 消息响应
    virtual void Notify(TNotifyUI& msg)
    {
        if (msg.sType == _T("click"))
        {
            OnClick(msg);
        }
        else if (msg.sType == _T("textchanged"))
        {
            OnTextChanged(msg);
        }
    }

    // 处理点击事件
    void OnClick(TNotifyUI& msg)
    {
        CDuiString sName = msg.pSender->GetName();

        if (sName == _T("btn_get_text"))
        {
            // 获取RichEdit文本并显示
            CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richedit_main")));
            CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_length")));
            if (pRichEdit && pLabel) {
                CDuiString strText = pRichEdit->GetText();
                CDuiString strResult;
                strResult.Format(_T("文本长度：%d 字符，行数：%d"), strText.GetLength(), pRichEdit->GetLineCount());
                pLabel->SetText(strResult);
            }
        }
        else if (sName == _T("btn_clear"))
        {
            // 清空RichEdit
            CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richedit_main")));
            if (pRichEdit) pRichEdit->SetText(_T(""));

            CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_length")));
            if (pLabel) pLabel->SetText(_T("文本长度：0 字符，行数：0"));
        }
        else if (sName == _T("btn_readonly"))
        {
            // 切换只读状态
            CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richedit_main")));
            CButtonUI* pBtn = static_cast<CButtonUI*>(msg.pSender);
            if (pRichEdit && pBtn) {
                bool bReadOnly = !pRichEdit->IsReadOnly();
                pRichEdit->SetReadOnly(bReadOnly);
                pBtn->SetText(bReadOnly ? _T("设为可编辑") : _T("设为只读"));
            }
        }
        else if (sName == _T("btn_append"))
        {
            // 追加文本
            CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("richedit_main")));
            if (pRichEdit) {
                pRichEdit->AppendText(_T("\r\n这是追加的一行文本。"));
            }
        }
        else if (sName == _T("btn_close"))
        {
            PostQuitMessage(0);
        }
    }

    // 处理文本改变事件
    void OnTextChanged(TNotifyUI& msg)
    {
        CDuiString sName = msg.pSender->GetName();

        if (sName == _T("richedit_main"))
        {
            // 实时显示文本长度
            CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(msg.pSender);
            CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_length")));
            if (pRichEdit && pLabel) {
                CDuiString strText = pRichEdit->GetText();
                CDuiString strCount;
                strCount.Format(_T("文本长度：%d 字符，行数：%d"), strText.GetLength(), pRichEdit->GetLineCount());
                pLabel->SetText(strCount);
            }
        }
    }

    // 响应默认键盘事件（如 ESC 关闭窗口）
    virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam)
    {
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
            return TRUE;
        }
        return FALSE;
    }

    // 重写 OnClose，使用 PostQuitMessage 退出
    virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
    {
        PostQuitMessage(0);
        return 0;
    }
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);

    HRESULT Hr = ::CoInitialize(NULL);
    if (FAILED(Hr)) return 0;

    CRichEditDemoWnd* pFrame = new CRichEditDemoWnd();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, _T("RichEdit 多行文本编辑器演示"), UI_WNDSTYLE_FRAME, 0);
    pFrame->CenterWindow();
    pFrame->ShowWindow(true);
    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}


