// RotateImageDemo.cpp : RotateImage 旋转图片控件演示程序
// 使用 WindowImplBase 基类

#include "DuiLib/UIlib.h"

using namespace DuiLib;


class CRotateImageDemoWnd : public WindowImplBase
{
public:
    CRotateImageDemoWnd() {};
    ~CRotateImageDemoWnd() {};

public:
    // 必须实现的纯虚函数
    virtual LPCTSTR GetWindowClassName(void) const
    {
        return _T("RotateImageDemoWindow");
    }

    virtual CDuiString GetSkinFile()
    {
        return _T("RotateImageDemo.xml");
    }

    virtual CDuiString GetSkinFolder()
    {
        return _T("assets\\");  // 需要包含反斜杠
    }

    // 窗口初始化
    virtual void InitWindow()
    {
        // 初始化旋转图片控件
        CRotateImageUI* pRotateImage = static_cast<CRotateImageUI*>(m_PaintManager.FindControl(_T("rotateimage_main")));
        if (pRotateImage) {
            // 设置默认图片（如果XML中没有设置）
            LPCTSTR pstrImage = pRotateImage->GetBkImage();
            if (pstrImage == NULL || _tcslen(pstrImage) == 0) {
                pRotateImage->SetBkImage(_T("loading.png"));
            }
        }

        // 更新状态显示
        UpdateStatus();
    }

    // 消息响应
    virtual void Notify(TNotifyUI& msg)
    {
        if (msg.sType == _T("click"))
        {
            OnClick(msg);
        }
    }

    // 处理点击事件
    void OnClick(TNotifyUI& msg)
    {
        CDuiString sName = msg.pSender->GetName();
        CRotateImageUI* pRotateImage = static_cast<CRotateImageUI*>(m_PaintManager.FindControl(_T("rotateimage_main")));

        if (sName == _T("btn_start_rotate"))
        {
            // 开始旋转
            if (pRotateImage) {
                pRotateImage->StartRotation();
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_stop_rotate"))
        {
            // 停止旋转并复位角度
            if (pRotateImage) {
                pRotateImage->StopRotation();
                pRotateImage->SetAngle(0.0f);
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_image_loading"))
        {
            // 切换到loading.png
            if (pRotateImage) {
                pRotateImage->SetBkImage(_T("loading.png"));
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_image_success"))
        {
            // 切换到success.png
            if (pRotateImage) {
                pRotateImage->SetBkImage(_T("success.png"));
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_image_fail"))
        {
            // 切换到fail.png
            if (pRotateImage) {
                pRotateImage->SetBkImage(_T("fail.png"));
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_speed_slow"))
        {
            // 设置慢速旋转（30度/秒）
            if (pRotateImage) {
                pRotateImage->SetRotationSpeed(30.0f);
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_speed_normal"))
        {
            // 设置正常速度（90度/秒）
            if (pRotateImage) {
                pRotateImage->SetRotationSpeed(90.0f);
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_speed_fast"))
        {
            // 设置快速旋转（180度/秒）
            if (pRotateImage) {
                pRotateImage->SetRotationSpeed(180.0f);
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_speed_increase"))
        {
            // 增加速度（+10度/秒）
            if (pRotateImage) {
                float fCurrentSpeed = pRotateImage->GetRotationSpeed();
                pRotateImage->SetRotationSpeed(fCurrentSpeed + 10.0f);
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_speed_decrease"))
        {
            // 减少速度（-10度/秒）
            if (pRotateImage) {
                float fCurrentSpeed = pRotateImage->GetRotationSpeed();
                float fNewSpeed = fCurrentSpeed - 10.0f;
                // 确保速度不小于0
                if (fNewSpeed < 0.0f) fNewSpeed = 0.0f;
                pRotateImage->SetRotationSpeed(fNewSpeed);
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_angle_0"))
        {
            // 设置角度为0度
            if (pRotateImage) {
                pRotateImage->SetAngle(0.0f);
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_angle_90"))
        {
            // 设置角度为90度
            if (pRotateImage) {
                pRotateImage->SetAngle(90.0f);
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_angle_180"))
        {
            // 设置角度为180度
            if (pRotateImage) {
                pRotateImage->SetAngle(180.0f);
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_angle_270"))
        {
            // 设置角度为270度
            if (pRotateImage) {
                pRotateImage->SetAngle(270.0f);
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_change_image1"))
        {
            // 切换图片1
            if (pRotateImage) {
                pRotateImage->SetBkImage(_T("Icon\\checked.png"));
                UpdateStatus();
            }
        }
        else if (sName == _T("hide_btn"))
        {
            // 显示/隐藏
            if (pRotateImage) {
                pRotateImage->SetVisible(!pRotateImage->IsVisible());
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_change_image2"))
        {
            // 切换图片2
            if (pRotateImage) {
                pRotateImage->SetBkImage(_T("Icon\\unchecked.png"));
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_change_image3"))
        {
            // 切换图片3
            if (pRotateImage) {
                pRotateImage->SetBkImage(_T("Icon\\list_icon_a.png"));
                UpdateStatus();
            }
        }
        else if (sName == _T("btn_close"))
        {
            PostQuitMessage(0);
        }
    }

    // 更新状态显示
    void UpdateStatus()
    {
        CRotateImageUI* pRotateImage = static_cast<CRotateImageUI*>(m_PaintManager.FindControl(_T("rotateimage_main")));
        if (!pRotateImage) return;

        CLabelUI* pStatusLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_status")));
        if (pStatusLabel) {
            CDuiString strStatus;
            strStatus.Format(_T("状态：旋转启用=%s, 当前角度=%.1f°, 旋转速度=%.1f°/秒"),
                pRotateImage->IsRotating() ? _T("是") : _T("否"),
                pRotateImage->GetAngle(),
                pRotateImage->GetRotationSpeed());
            pStatusLabel->SetText(strStatus);
        }

        CLabelUI* pImageLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("label_image")));
        if (pImageLabel) {
            CDuiString strImage;
            LPCTSTR pstrImage = pRotateImage->GetBkImage();
            strImage.Format(_T("当前图片：%s"), pstrImage ? pstrImage : _T(""));
            pImageLabel->SetText(strImage);
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

    CRotateImageDemoWnd* pFrame = new CRotateImageDemoWnd();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, _T("RotateImage 旋转图片控件演示"), UI_WNDSTYLE_FRAME, 0);
    pFrame->CenterWindow();
    pFrame->ShowWindow(true);
    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}

