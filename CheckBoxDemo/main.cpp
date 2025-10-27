// CheckBoxDemo.cpp : CheckBox 复选框控件演示程序
// 使用 WindowImplBase 基类

#include "DuiLib/UIlib.h"

using namespace DuiLib;


class CCheckBoxDemoWnd : public WindowImplBase
{
public:
    CCheckBoxDemoWnd() : m_pResultText(NULL) {};
    ~CCheckBoxDemoWnd() {};

private:
    CTextUI* m_pResultText;  // 结果显示文本控件

public:
    // 必须实现的纯虚函数
    virtual LPCTSTR GetWindowClassName(void) const
    {
        return _T("CheckBoxDemoWindow");
    }

    virtual CDuiString GetSkinFile()
    {
        return _T("CheckBoxDemo.xml");
    }

    virtual CDuiString GetSkinFolder()
    {
        return _T("assets\\");  // 需要包含反斜杠
    }

    // 窗口初始化
    virtual void InitWindow()
    {
        // 获取结果显示控件
        m_pResultText = dynamic_cast<CTextUI*>(m_PaintManager.FindControl(_T("label_result")));

        // 设置默认选中一些选项
        CCheckBoxUI* pCheck = dynamic_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chk_hobby2")));
        if (pCheck) pCheck->Selected(true);

        pCheck = dynamic_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chk_hobby5")));
        if (pCheck) pCheck->Selected(true);
    }

    // 消息响应
    virtual void Notify(TNotifyUI& msg)
    {
        if (msg.sType == _T("click"))
        {
            OnClick(msg);
        }
        else if (msg.sType == _T("selectchanged"))
        {
            OnSelectChanged(msg);
        }
    }

    // 处理点击事件
    void OnClick(TNotifyUI& msg)
    {
        CDuiString sName = msg.pSender->GetName();

        if (sName == _T("btn_get_selected"))
        {
            // 获取所有 CheckBox 的选中状态
            CDuiString strResult = _T("选中状态：");

            // 基础选项
            strResult += _T(" 基础选项：");
            for (int i = 1; i <= 4; i++) {
                CDuiString sCheckName;
                sCheckName.Format(_T("chk_option%d"), i);
                CCheckBoxUI* pCheck = dynamic_cast<CCheckBoxUI*>(m_PaintManager.FindControl(sCheckName));
                if (pCheck && pCheck->IsEnabled()) {
                    CDuiString sTemp;
                    sTemp.Format(_T("  选项 %d: %s "), i, pCheck->IsSelected() ? _T("☑ 选中") : _T("☐ 未选中"));
                    strResult += sTemp;
                }
            }

            // 兴趣爱好
            strResult += _T(" 兴趣爱好：");
            const TCHAR* hobbies[] = {_T("音乐"), _T("电影"), _T("阅读"), _T("运动"), _T("游戏")};
            for (int i = 1; i <= 5; i++) {
                CDuiString sCheckName;
                sCheckName.Format(_T("chk_hobby%d"), i);
                CCheckBoxUI* pCheck = dynamic_cast<CCheckBoxUI*>(m_PaintManager.FindControl(sCheckName));
                if (pCheck) {
                    strResult += _T("  ");
                    strResult += hobbies[i-1];
                    strResult += _T(": ");
                    strResult += pCheck->IsSelected() ? _T("☑ 选中") : _T("☐ 未选中");
                }
            }

            // 协议同意
            CCheckBoxUI* pAgree = dynamic_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chk_agree")));
            if (pAgree) {
                strResult += _T(" 协议同意：");
                strResult += pAgree->IsSelected() ? _T("☑ 已同意") : _T("☐ 未同意");
            }

            if (m_pResultText) m_pResultText->SetText(strResult);
        }
        else if (sName == _T("btn_select_all"))
        {
            // 全选兴趣爱好
            for (int i = 1; i <= 5; i++) {
                CDuiString sCheckName;
                sCheckName.Format(_T("chk_hobby%d"), i);
                CCheckBoxUI* pCheck = dynamic_cast<CCheckBoxUI*>(m_PaintManager.FindControl(sCheckName));
                if (pCheck) pCheck->Selected(true);
            }

            if (m_pResultText) m_pResultText->SetText(_T("选中状态：已全选兴趣爱好"));
        }
        else if (sName == _T("btn_clear_all"))
        {
            // 清除所有选项
            // 基础选项
            for (int i = 1; i <= 4; i++) {
                CDuiString sCheckName;
                sCheckName.Format(_T("chk_option%d"), i);
                CCheckBoxUI* pCheck = dynamic_cast<CCheckBoxUI*>(m_PaintManager.FindControl(sCheckName));
                if (pCheck && pCheck->IsEnabled()) pCheck->Selected(false);
            }

            // 兴趣爱好
            for (int i = 1; i <= 5; i++) {
                CDuiString sCheckName;
                sCheckName.Format(_T("chk_hobby%d"), i);
                CCheckBoxUI* pCheck = dynamic_cast<CCheckBoxUI*>(m_PaintManager.FindControl(sCheckName));
                if (pCheck) pCheck->Selected(false);
            }

            // 协议同意
            CCheckBoxUI* pAgree = dynamic_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("chk_agree")));
            if (pAgree) pAgree->Selected(false);

            if (m_pResultText) m_pResultText->SetText(_T("选中状态：已清除全部"));
        }
        else if (sName == _T("btn_close"))
        {
            PostQuitMessage(0);
        }
    }

    // 处理选中状态改变事件
    void OnSelectChanged(TNotifyUI& msg)
    {
        CDuiString sName = msg.pSender->GetName();
        CCheckBoxUI* pCheck = dynamic_cast<CCheckBoxUI*>(msg.pSender);

        if (pCheck) {
            CDuiString strInfo;
            strInfo.Format(_T("【%s】状态改变：%s"),
                pCheck->GetText().GetData(),
                pCheck->IsSelected() ? _T("☑ 已选中") : _T("☐ 已取消"));
            if (m_pResultText) m_pResultText->SetText(strInfo);
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

    CCheckBoxDemoWnd* pFrame = new CCheckBoxDemoWnd();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, _T("CheckBox 复选框控件演示"), UI_WNDSTYLE_FRAME, 0);
    pFrame->CenterWindow();
    pFrame->ShowWindow(true);
    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}

