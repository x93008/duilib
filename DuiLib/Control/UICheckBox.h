#ifndef __UICHECKBOX_H__
#define __UICHECKBOX_H__

#pragma once

namespace DuiLib
{
	/// Common checkbox control, supports checked/unchecked states
	/// Unlike COptionUI where only one button can be selected per group, checkboxes are independent by default
	/// <CheckBox name="CheckBox" value="height='20' align='left' textpadding='24,0,0,0' normalimage='file='sys_check_btn.png' source='0,0,20,20' dest='0,0,20,20'' selectedimage='file='sys_check_btn.png' source='20,0,40,20' dest='0,0,20,20'' disabledimage='file='sys_check_btn.png' source='40,0,60,20' dest='0,0,20,20''"/>

	class DUILIB_API CCheckBoxUI : public COptionUI
	{
	public:
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetCheck(bool bCheck, bool bTriggerEvent=true);
		bool GetCheck() const;
	};
}

#endif // __UICHECKBOX_H__
