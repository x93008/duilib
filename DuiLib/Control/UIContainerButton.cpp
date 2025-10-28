#include "stdafx.h"
#include "UIContainerButton.h"

namespace DuiLib
{
	CContainerButtonUI::CContainerButtonUI()
    : m_bHot(false)
    , m_bPushed(false)
    , m_bFocused(false)
    , m_dwNormalTextColor(0xFF000000)
    , m_dwHotTextColor(0xFF000000)
    , m_dwPushedTextColor(0xFF000000)
    , m_dwDisabledTextColor(0xFFAAAAAA)
{
    // 设置默认内边距，让内容不会紧贴边缘
    SetInset(CDuiRect(8, 4, 8, 4));
}

LPCTSTR CContainerButtonUI::GetClass() const
{
    return DUI_CTR_CONTAINERBUTTON;
}

LPVOID CContainerButtonUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_CONTAINERBUTTON) == 0)
        return static_cast<CContainerButtonUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CContainerButtonUI::GetControlFlags() const
{
    return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}

void CContainerButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    // 文本颜色属性（用于子控件）
    if (_tcscmp(pstrName, _T("normaltextcolor")) == 0) {
        SetNormalTextColor(ParseColor(pstrValue));
    }
    else if (_tcscmp(pstrName, _T("hottextcolor")) == 0) {
        SetHotTextColor(ParseColor(pstrValue));
    }
    else if (_tcscmp(pstrName, _T("pushedtextcolor")) == 0) {
        SetPushedTextColor(ParseColor(pstrValue));
    }
    else if (_tcscmp(pstrName, _T("disabledtextcolor")) == 0) {
        SetDisabledTextColor(ParseColor(pstrValue));
    }
    else {
        // 其他属性（包括 hotbkcolor、pushedbkcolor、disabledbkcolor）传递给基类
        CContainerUI::SetAttribute(pstrName, pstrValue);
    }
}

void CContainerButtonUI::DoEvent(TEventUI& event)
{
    if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
        if (m_pParent) m_pParent->DoEvent(event);
        return;
    }
    
    switch (event.Type) {
    case UIEVENT_SETFOCUS:
        m_bFocused = true;
        Invalidate();
        break;
        
    case UIEVENT_KILLFOCUS:
        m_bFocused = false;
        Invalidate();
        break;
        
    case UIEVENT_KEYDOWN:
        if (IsKeyboardEnabled()) {
            if (event.chKey == VK_SPACE || event.chKey == VK_RETURN) {
                m_bPushed = true;
                UpdateChildrenState();
                Invalidate();
            }
        }
        break;
        
    case UIEVENT_KEYUP:
        if (IsKeyboardEnabled()) {
            if (event.chKey == VK_SPACE || event.chKey == VK_RETURN) {
                m_bPushed = false;
                UpdateChildrenState();
                Invalidate();
                
                // 触发点击事件
                if (m_pManager) {
                    m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
                }
            }
        }
        break;
        
    case UIEVENT_BUTTONDOWN:
        if (IsEnabled()) {
            m_bPushed = true;
            UpdateChildrenState();
            Invalidate();
        }
        break;
        
    case UIEVENT_BUTTONUP:
        if (m_bPushed) {
            m_bPushed = false;
            UpdateChildrenState();
            Invalidate();
            
            // 触发点击事件
            if (::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled()) {
                Activate();
            }
        }
        break;
        
    case UIEVENT_MOUSEMOVE:
        if (IsEnabled()) {
            bool bInRect = ::PtInRect(&m_rcItem, event.ptMouse);
            if (bInRect && !m_bHot) {
                m_bHot = true;
                UpdateChildrenState();
                Invalidate();

                // 发送鼠标进入消息
                if (m_pManager) {
                    m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSEENTER);
                }
            }
            else if (!bInRect && m_bHot) {
                m_bHot = false;
                UpdateChildrenState();
                Invalidate();

                // 发送鼠标离开消息
                if (m_pManager) {
                    m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSELEAVE);
                }
            }
        }
        break;

    case UIEVENT_MOUSELEAVE:
        if (IsEnabled()) {
            m_bHot = false;
            m_bPushed = false;
            UpdateChildrenState();
            Invalidate();

            // 发送鼠标离开消息
            if (m_pManager) {
                m_pManager->SendNotify(this, DUI_MSGTYPE_MOUSELEAVE);
            }
        }
        break;
        
    case UIEVENT_SETCURSOR:
        if (IsEnabled()) {
            ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
            return;
        }
        break;
    }
    
    // 重要：调用基类的事件处理，确保子控件能收到事件
    CContainerUI::DoEvent(event);
}

bool CContainerButtonUI::Activate()
{
    if (!CContainerUI::Activate()) return false;
    
    // 触发点击事件
    if (m_pManager) {
        m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
    }
    return true;
}

DWORD CContainerButtonUI::ParseColor(LPCTSTR pstrColor)
{
    if (pstrColor == NULL) return 0;
    
    if (*pstrColor == _T('#')) pstrColor = ::CharNext(pstrColor);
    LPTSTR pstr = NULL;
    DWORD clrColor = _tcstoul(pstrColor, &pstr, 16);
    return clrColor;
}

void CContainerButtonUI::SetNormalTextColor(DWORD dwColor)
{
    m_dwNormalTextColor = dwColor;
    UpdateChildrenState();
}

void CContainerButtonUI::SetHotTextColor(DWORD dwColor)
{
    m_dwHotTextColor = dwColor;
    UpdateChildrenState();
}

void CContainerButtonUI::SetPushedTextColor(DWORD dwColor)
{
    m_dwPushedTextColor = dwColor;
    UpdateChildrenState();
}

void CContainerButtonUI::SetDisabledTextColor(DWORD dwColor)
{
    m_dwDisabledTextColor = dwColor;
    UpdateChildrenState();
}

void CContainerButtonUI::PaintText(HDC hDC)
{
    // 调用基类的PaintText，确保文本绘制逻辑正常
    CContainerUI::PaintText(hDC);
}

void CContainerButtonUI::UpdateChildrenState()
{
    // 确定当前状态对应的颜色
    DWORD dwTextColor = m_dwNormalTextColor;
    
    if (!IsEnabled()) {
        dwTextColor = m_dwDisabledTextColor;
    }
    else if (m_bPushed) {
        dwTextColor = m_dwPushedTextColor;
    }
    else if (m_bHot) {
        dwTextColor = m_dwHotTextColor;
    }
    
    // 更新所有子控件的文字颜色
    for (int i = 0; i < GetCount(); i++) {
        CControlUI* pControl = GetItemAt(i);
        if (pControl) {
            CLabelUI* pLabel = dynamic_cast<CLabelUI*>(pControl);
            if (pLabel) {
                pLabel->SetTextColor(dwTextColor);
            }
            
            // 递归处理容器内的控件
            CContainerUI* pContainer = dynamic_cast<CContainerUI*>(pControl);
            if (pContainer) {
                UpdateContainerTextColor(pContainer, dwTextColor);
            }
        }
    }
}

void CContainerButtonUI::UpdateContainerTextColor(CContainerUI* pContainer, DWORD dwColor)
{
    if (!pContainer) return;
    
    for (int i = 0; i < pContainer->GetCount(); i++) {
        CControlUI* pControl = pContainer->GetItemAt(i);
        if (pControl) {
            CLabelUI* pLabel = dynamic_cast<CLabelUI*>(pControl);
            if (pLabel) {
                pLabel->SetTextColor(dwColor);
            }
            
            // 继续递归
            CContainerUI* pSubContainer = dynamic_cast<CContainerUI*>(pControl);
            if (pSubContainer) {
                UpdateContainerTextColor(pSubContainer, dwColor);
            }
        }
    }
}


}	// namespace DuiLib
