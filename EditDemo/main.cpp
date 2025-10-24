// EditDemo.cpp : Edit 控件演示程序
// 使用 WindowImplBase 基类

#include "DuiLib/UIlib.h"

using namespace DuiLib;


class CEditDemoWnd : public WindowImplBase
{
public:
    CEditDemoWnd() {};
    ~CEditDemoWnd() {};

public:
    // 必须实现的纯虚函数
    virtual LPCTSTR GetWindowClassName(void) const
    {
        return _T("EditDemoWindow");
    }

    virtual CDuiString GetSkinFile()
    {
        return _T("EditDemo.xml");
    }

    virtual CDuiString GetSkinFolder()
    {
        return _T("assets\\");  // 需要包含反斜杠
    }

    // 窗口初始化
    virtual void InitWindow()
    {
        // 初始化一些默认值
        CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_normal")));
        if (pEdit) {
            pEdit->SetText(_T("普通编辑框"));
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
            // 获取编辑框文本并显示
            CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_normal")));
            CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_result")));
            if (pEdit && pLabel) {
                CDuiString strText = pEdit->GetText();
                CDuiString strResult;
                strResult.Format(_T("您输入的内容是：%s"), strText.GetData());
                pLabel->SetText(strResult);
            }
        }
        else if (sName == _T("btn_clear"))
        {
            // 清空所有编辑框
            CEditUI* pEdit1 = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_normal")));
            CEditUI* pEdit2 = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_password")));
            CEditUI* pEdit3 = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_number")));
            CEditUI* pEdit5 = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edit_maxchar")));

            if (pEdit1) pEdit1->SetText(_T(""));
            if (pEdit2) pEdit2->SetText(_T(""));
            if (pEdit3) pEdit3->SetText(_T(""));
            if (pEdit5) pEdit5->SetText(_T(""));

            CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_result")));
            if (pLabel) pLabel->SetText(_T("结果显示区域"));

            CLabelUI* pCharCount = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_charcount")));
            if (pCharCount) pCharCount->SetText(_T("当前字符数：0 / 20"));
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

        if (sName == _T("edit_maxchar"))
        {
            // 实时显示字符数
            CEditUI* pEdit = static_cast<CEditUI*>(msg.pSender);
            CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_charcount")));
            if (pEdit && pLabel) {
                CDuiString strText = pEdit->GetText();
                CDuiString strCount;
                strCount.Format(_T("当前字符数：%d / 20"), strText.GetLength());
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

    // 重写 OnClose，使用 PostQuitMessage 退出（参考 test_duilib）
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

    CEditDemoWnd* pFrame = new CEditDemoWnd();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, _T("Edit 控件演示"), UI_WNDSTYLE_FRAME, 0);
    pFrame->CenterWindow();
    pFrame->ShowWindow(true);
    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}
